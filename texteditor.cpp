#include "graphics.h"   
#include <stdlib.h>
#include <fstream>
#include <Windows.h>
#include <iostream>

using namespace std;

struct textNode {
	char ch;
	textNode *prev, *next;
};

void color()
{
	setbkcolor(BLUE);
	setfillstyle(1, 1);
	floodfill(0, 0, 1);
}

void addCharacter(textNode* &head, textNode* &tail, textNode* &current, char data, int& size)
{
	if (head == NULL)
	{
		head = current = tail = new textNode;
		head->ch = data;
		head->prev = NULL;
		head->next = NULL;
		size = 1;
	}
	else if (tail == current)
	{
		textNode* newnode = new textNode;
		tail->next = current->next = newnode;
		newnode->prev = tail;
		tail = tail->next;
		current = current->next;
		tail->ch = data;
		tail->next = NULL;
		size++;
	}

	else
	{
		textNode* newnode = new textNode;
		newnode->ch = data;
		newnode->prev = current;
		newnode->next = current->next;
		current->next = newnode;
		if (newnode->next != NULL)
			newnode->next->prev = newnode;
		current = current->next;
		size++;
	}
}

void removeCharacter(textNode* &head, textNode* &current, int &size)
{
	if (head == current && head->next == NULL)
	{
		delete current;
		current = head = NULL;
	}
	else if (head == current && head->next != NULL)
	{
		head = head->next;
		head->prev = NULL;
		delete current;
		current = NULL;
	}
	else
	{
		textNode* cursor = current;
		current->prev->next = current->next;
		if (current->next != NULL)
			current->next->prev = current->prev;

		cursor = current->prev;
		delete current;
		current = cursor;
	}
	size--;
}

int printList(textNode* head, textNode* cursor, int &x, int &y, textNode* start = nullptr, textNode* end = nullptr)
{

	settextstyle(DEFAULT_FONT, HORIZ_DIR, 0);
	setcolor(WHITE);
	rectangle(5, 5, getwindowwidth() - 25, getwindowheight() - 45);
	color();
	int currentx = x;
	if (head == NULL)
	{
		setcolor(3);
		outtextxy(10, 10, "|");
		x -= textwidth("|");
		setcolor(WHITE);

	}

	else {

		x = 10;
		y = 10;
		textNode* current = head;
		char data[2] = { current->ch, '\0' };
		while (current != NULL &&  y <= getwindowheight() - 60)
		{

			if (current == start) {
				setbkcolor(BLACK);
				while (current != NULL && current->prev != end) {
					data[0] = current->ch;
					if (x < getwindowwidth() - 50 && data[0] != '\n' && data[0] != '\t') {
						outtextxy(x, y, data);
						x += textwidth(data);
					}
					else if (data[0] == '\n' || x > getwindowwidth() - 40) {
						x = 10;
						y += textheight(data);
					}
					else
						x += textwidth("        ");


					if (current != NULL)
						current = current->next;

				}
				if (current != NULL)
					current = current->prev;
			}
			else if (current != NULL) {
				setbkcolor(BLUE);
				data[0] = current->ch;
				if (x < getwindowwidth() - 40 && data[0] != '\n' && data[0] != '\t') {
					outtextxy(x, y, data);
					x += textwidth(data);
				}
				else if (data[0] == '\n' || x > getwindowwidth() - 40) {
					x = 10;
					y += textheight(data);
				}
				else
					x += textwidth("        ");
			}

			if (current == cursor) {
				setcolor(3);
				outtextxy(x, y, "|");
				currentx = x;
				if (data[0] != '\n' && current->next != nullptr) {
					x += textwidth("|");
				}
				setcolor(WHITE);
			}


			if (current != NULL)
				current = current->next;
		}
	}

	return currentx;
}


void moveTheCursor(textNode* &Text, textNode* &cursor, char ch, int &x, int &y)
{
	if (ch == KEY_LEFT) {
		if (cursor == NULL)
			cursor = Text;
		else
			cursor = (cursor->prev);
		cleardevice();
		printList(Text, cursor, x, y);
	}
	else if (ch == KEY_RIGHT) {
		if (cursor == NULL)
			cursor = Text;
		else
			cursor = (cursor->next);
		cleardevice();
		printList(Text, cursor, x, y);
	}
	else if (ch == KEY_UP) {

		if (y - textheight("A") < 10)
			return;

		if (cursor->ch == '\n' && cursor->prev->ch == '\n') {
			cursor = cursor->prev;
			y -= textwidth("A");
			cleardevice();
			printList(Text, cursor, x, y);
			return;
		}

		int count = 0;
		if (cursor->next != nullptr)
			x = printList(Text, cursor, x, y);

		while (x >= 10) {
			char data[2] = { cursor->ch, '\0' };
			cursor = cursor->prev;
			x -= textwidth(data);
			count++;
		}
		if (cursor->ch == '\n' && cursor->prev->ch != '\n')
			cursor = cursor->prev->prev;


		x = printList(Text, cursor, x, y);
		while (x > 10 && cursor != Text) {
			char data[2] = { cursor->ch, '\0' };
			cursor = cursor->prev;
			x -= textwidth(data);
		}
		if (cursor == Text)
			count--;
		for (int i = 1; i < count && cursor->next->ch != '\n'; i++)
			cursor = cursor->next;
		cleardevice();
		printList(Text, cursor, x, y);
		y -= textheight("A");

	}
	else if (ch == KEY_DOWN) {

		if (y + textheight("A") > getwindowheight() - 70)
			return;
		if (cursor->next == nullptr)
			return;
		if (cursor->ch == '\n' && cursor->next->ch == '\n') {
			cursor = cursor->next;
			y += textwidth("A");
			cleardevice();
			printList(Text, cursor, x, y);
			return;
		}

		int count = 0;
		if (cursor->next != nullptr)
			x = printList(Text, cursor, x, y);

		while (x >= 10) {
			char data[2] = { cursor->ch, '\0' };
			x -= textwidth(data);
			count++;
		}
		x = printList(Text, cursor, x, y);
		while (x < getwindowwidth() - 50 && cursor->ch != '\n' && cursor->next != nullptr) {
			char data[2] = { cursor->ch, '\0' };
			cursor = cursor->next;
			x += textwidth(data);
		}
		if (cursor->ch != '\n')
			count += 2;

		for (int i = 1; i < count && cursor->next != nullptr && cursor->next->ch != '\n'; i++)
			cursor = cursor->next;

		cleardevice();
		printList(Text, cursor, x, y);
		y += textheight("A");
	}

}

char* getName(int x, int y)
{
	char *name = new char[20];
	int i = 0;
	name[i] = getch();
	name[i + 1] = '\0';
	while (name[i] != 13)
	{
		outtextxy(x, y, name);
		i++;
		name[i] = getch();
		name[i + 1] = '\0';
	}
	name[i] = '\0';

	return name;
}

void Copy(textNode* start, textNode* end, textNode* &newhead, textNode* &newtail, bool& headAssigned)
{
	textNode* current = start;
	if (newhead != nullptr && newtail != nullptr)
	{
		delete newhead;
		delete newtail;
		newhead = newtail = nullptr;
	}

	if (start == end)
	{
		newhead = newtail = new textNode;
		newhead->prev = NULL;
		newhead->ch = current->ch;
		newhead->next = NULL;
	}
	else {
		while (current != end) {

			if (!headAssigned) {
				newhead = newtail = new textNode;
				newhead->prev = NULL;
				newhead->ch = current->ch;
				newhead->next = NULL;
				headAssigned = true;
			}
			else {
				textNode *temp = newtail;
				newtail = new textNode;
				newtail->prev = temp;
				temp->next = newtail;
				newtail->next = NULL;
				newtail->ch = current->ch;
			}
			current = current->next;
		}

		textNode *temp = newtail;
		newtail = new textNode;
		newtail->prev = temp;
		temp->next = newtail;
		newtail->next = NULL;
		newtail->ch = current->ch;
	}

}


void replaceText(textNode* start, textNode* end, char* find, char* replace, textNode* &cursor)
{
	int sizefind;
	int sizereplace;
	int i, j;
	for (i = 0; find[i]; i++);
	sizefind = i;

	for (j = 0; replace[j]; j++);
	sizereplace = j;
	textNode* index = start;
	textNode* temp;
	i = 0;
	if (sizereplace < sizefind) {
		int diff = sizefind - sizereplace;
		while (replace[i]) {
			index->ch = replace[i];
			i++;
			index = index->next;
		}
		temp = index;
		index = index->prev;
		if (end != nullptr)
			index->next = end->next;
		else
			index->next = nullptr;
		cursor = index;
		if (end != nullptr && end->next != nullptr) {
			end->next->prev = index;
		}
		while (temp != end)
		{
			if (temp->next != nullptr) {
				temp = temp->next;
				delete temp->prev;
			}
			else {
				delete temp;
				break;
			}
			if ((end == nullptr && temp->next == nullptr) || temp == end) {
				delete temp;
				break;
			}
		}
	}
	else if (sizefind == sizereplace) {
		while (replace[i]) {
			index->ch = replace[i];
			i++;
			index = index->next;
		}
		cursor = end;
	}
	else {

		textNode* newnode;
		textNode* newend;

		if (end != nullptr)
			newend = end->next;
		else
			newend = nullptr;
		i = 0;
		int diff = sizereplace - sizefind;
		while (index->next != nullptr  && index != end)
		{
			index->ch = replace[i];
			i++;
			if (index->next != nullptr)
				index = index->next;
		}
		index = index->prev;

		for (int k = 0; k <= diff; k++)
		{
			newnode = new textNode;
			index->next = newnode;
			newnode->prev = index;
			newnode->ch = replace[i++];
			if (index != nullptr)
				index = index->next;
		}
		newnode->next = newend;
		if (newend != nullptr) {
			newend->prev = newnode;
		}
		if (newend == nullptr)
			cursor = newnode;
		else
			cursor = newend;
	}
}

int main() {


	int window = initwindow(600, 500, "First Page");
	color();
	settextstyle(DEFAULT_FONT, HORIZ_DIR, 0);
	rectangle(5, 5, getwindowwidth() - 25, getwindowheight() - 45);
	int w2;    //for next windows
	int flag = 0;
	textNode* OriginalHead = NULL;

	int x = 10, y = 10;
	textNode * Text = NULL;
	textNode* Tail = NULL;
	textNode * cursor = NULL;
	int size = 0;

	//for selecting
	textNode* start = nullptr;
	textNode* end = nullptr;
	bool rightStart = false;
	bool leftEnd = false;

	//for second list(used for copying and cutting)
	textNode* newhead = NULL;
	textNode* newtail = NULL;
	bool headAssigned = false;


	char ch[2] = { '\0','\0' };
	while (true) //keep displaying        
	{


		ch[0] = getch();//read the key-press 

		bool shift = false;
		if (GetKeyState(VK_SHIFT) < 0)
			shift = true;


		if (!shift) {


			if (ch[0] >= 32 && ch[0] <= 126) // 1) adding characters
			{
				addCharacter(Text, Tail, cursor, ch[0], size); //add to the list

				if (Tail != cursor) {
					cleardevice();
					printList(Text, cursor, x, y);
				}
				else
				{

					if (x < getwindowwidth() - 40 && ch[0] != '\n' && ch[0] != '\t') {

						outtextxy(x, y, ch);
						x += textwidth(ch);
					}
					else if (ch[0] == '\n' || x > getwindowwidth() - 40) {
						outtextxy(x, y, " ");
						y += textheight(ch);
						x = 10;
						outtextxy(x, y, ch);
						x += textwidth(ch);
					}
					else
						x += textwidth("        ");


					setcolor(3);
					outtextxy(x, y, "|");
					setcolor(WHITE);
				}
			}
			else if (ch[0] == 8) // 2) backspace
			{
				if (Text != NULL && cursor != NULL) {
					removeCharacter(Text, cursor, size);
					cleardevice();
					printList(Text, cursor, x, y);
				}

			}
			else if (ch[0] == 13) // 3) enter
			{
				addCharacter(Text, Tail, cursor, '\n', size);
				y += textheight(ch);
				x = 10;
				cleardevice();
				printList(Text, cursor, x, y);
			}
			else if (ch[0] == 9) //tab
			{
				addCharacter(Text, Tail, cursor, '\t', size);
				x += textwidth("        ");

			}
			else if (ch[0] == 0) // 4) arrow keys
			{
				ch[0] = getch();
				if (ch[0] == KEY_LEFT || ch[0] == KEY_RIGHT || ch[0] == KEY_UP || ch[0] == KEY_DOWN)
					moveTheCursor(Text, cursor, ch[0], x, y);
			}
			else if (ch[0] == 15)    //open from txt file
			{
				int wind;
				wind = initwindow(200, 200);
				outtextxy(10, 10, "Enter the name of file \n\n");
				char name[20];
				char* file = getName(10, 30);
				int i = 0;
				for (i = 0; file[i]; i++)
					name[i] = file[i];
				name[i] = '\0';
				closegraph(wind);
				setcurrentwindow(window);

				char data;
				ifstream input;
				input.open(name);
				char ch = input.peek();
				if (input) {
					while (!input.eof())
					{
						input.get(data);
						addCharacter(Text, Tail, cursor, data, size);
						ch = input.peek();
					}
					printList(Text, cursor, x, y);
				}
				else
					outtextxy(10, 10, "No such file found");
			}
			else if (ch[0] == 19)    //save to txt file
			{

				int wind;
				wind = initwindow(200, 200);
				outtextxy(10, 10, "Enter the name of file \n\n");
				char name[20];
				char* file = getName(10, 30);
				int i = 0;
				for (i = 0; file[i]; i++)
					name[i] = file[i];
				name[i] = '\0';
				closegraph(wind);
				setcurrentwindow(window);

				ofstream output;
				output.open(name);
				textNode* current;
				if (OriginalHead == NULL)
					current = Text;
				else
					current = OriginalHead;

				char data[2] = { current->ch, '\0' };
				while (current != NULL)
				{
					data[0] = current->ch;
					if (data[0] != '\n')
						output << data;
					else
						output << endl;

					current = current->next;
				}
				output.close();

			}
			else if (ch[0] == 6)  //finding text
			{
				int wind = initwindow(200, 200);
				outtextxy(10, 10, "Enter the text to find");
				char* find = getName(10, 30);

				closegraph(wind);
				setcurrentwindow(window);
				textNode* index;
				if (OriginalHead == NULL)
					index = Text;
				else
					index = OriginalHead;
				int i = 0;
				int flag = 0;
				textNode* temp;

				while (index != NULL)
				{
					if (OriginalHead != nullptr)
						if (index == Text)
							setcurrentwindow(w2);

					i = 0;
					if (index->ch == find[i]) {
						temp = index;
						while (temp != NULL && temp->ch == find[i]) {
							temp = temp->next;
							i++;
						}
						if (find[i] == '\0') {
							if (temp != nullptr)
								temp = temp->prev;
							cleardevice();
							printList(Text, cursor, x, y, index, temp);
							Sleep(500);

						}
					}
					index = index->next;
				}
			}


			else if (ch[0] == 18) { //finding and replacing text

				int wind = initwindow(200, 200);
				outtextxy(10, 10, "Enter the text to find");
				char* find = getName(10, 30);
				outtextxy(10, 50, "Enter text to replace");
				char* replace = getName(10, 70);

				closegraph(wind);
				setcurrentwindow(window);
				textNode* index;
				if (OriginalHead == NULL)
					index = Text;
				else
					index = OriginalHead;

				int i = 0;
				int flag = 0;
				textNode* temp;

				while (index != NULL)
				{
					if (OriginalHead != nullptr)
						if (index == Text)
							setcurrentwindow(w2);

					i = 0;
					if (index->ch == find[i]) {
						temp = index;
						while (temp != NULL && temp->ch == find[i]) {
							temp = temp->next;
							i++;
						}
						if (find[i] == '\0') {
							if (temp != nullptr)
								temp = temp->prev;
							replaceText(index, temp, find, replace, cursor);
							cleardevice();
							if (OriginalHead == nullptr)
								printList(Text, cursor, x, y);
							else
								printList(OriginalHead, cursor, x, y);
						}
					}
					index = index->next;
				}
				cleardevice();
				printList(Text, cursor, x, y);
			}

			else if (ch[0] == 3)   //copy 
			{
				Copy(start, end, newhead, newtail, headAssigned);
				headAssigned = false;
				rightStart = false;
				leftEnd = false;
			}
			else if (ch[0] == 24)   //cut
			{
				textNode* current = start;

				Copy(start, end, newhead, newtail, headAssigned);
				headAssigned = false;
				rightStart = false;
				leftEnd = false;

				current = start;
				if (end != nullptr && end->next != nullptr)
					end->next->prev = start->prev;

				if (start == Text &&end != nullptr && end->next != nullptr)
					Text = end->next;
				else if (start == Text &&end->next == nullptr)
					Text = nullptr;


				if (start->prev != nullptr) {
					start->prev->next = end->next;
					cursor = start->prev;
				}
				else
					cursor = end->next;



				while (current != end)
				{
					current = current->next;

					delete current->prev;
				}

				delete end;

				cleardevice();
				printList(Text, cursor, x, y);
			}

			else if (ch[0] == 22)     //paste
			{
				textNode *temphead = nullptr, *temptail = nullptr;
				bool flag = false;
				Copy(newhead, newtail, temphead, temptail, flag);

				if (cursor != nullptr) {
					temptail->next = cursor->next;
					if (cursor->next != nullptr)
						cursor->next->prev = temptail;
					cursor->next = temphead;
					temphead->prev = cursor;
				}
				cleardevice();
				printList(Text, cursor, x, y);



			}

			if (ch[0] != 24 && ch[0] != 3) //24==cut and 3==copy
			{
				rightStart = false;
				leftEnd = false;
				headAssigned = false;
			}

		}
		else
		{
			if (ch[0] == 0)
				ch[0] = getch();


			if (ch[0] == KEY_RIGHT)
			{
				if (cursor != nullptr && cursor->next != nullptr) {
					cursor = cursor->next;
					if (!rightStart) {
						start = end = cursor;
						rightStart = true;
					}
					else
						end = cursor;

					cleardevice();
					printList(Text, cursor, x, y, start, end);
				}

			}
			else if (ch[0] == KEY_LEFT)
			{
				if (cursor != nullptr) {
					cursor = cursor->prev;

					if (!leftEnd) {
						cursor = cursor->next;
						start = end = cursor;
						leftEnd = true;
					}
					else
						start = cursor;

					cleardevice();
					printList(Text, cursor, x, y, start, end);

				}

			}
			else if (ch[0] == KEY_UP)
			{
				if (!leftEnd) {
					start = end = cursor;
					leftEnd = true;
				}

				moveTheCursor(Text, cursor, ch[0], x, y);

				if (leftEnd)
					start = cursor;

				cleardevice();
				printList(Text, cursor, x, y, start, end);
			}

			else if (ch[0] == KEY_DOWN)
			{

				if (!rightStart) {
					start = end = cursor;
					rightStart = true;
				}

				moveTheCursor(Text, cursor, ch[0], x, y);

				if (rightStart)
					end = cursor;

				cleardevice();
				printList(Text, cursor, x, y, start, end);
			}

		}

		if (x > getwindowwidth() - 40 && y > getwindowheight() - 70)   //creating a new page
		{

			w2 = initwindow(600, 500, "Next Page");
			setcurrentwindow(w2);
			color();
			if (!flag)
			{
				OriginalHead = Text;
				flag = 1;
			}

			Text = cursor;
			Text->prev = cursor->prev;

			printList(Text, cursor, x, y);

		}
	}


	return 0;
}