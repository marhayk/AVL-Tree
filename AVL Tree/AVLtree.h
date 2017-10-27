#pragma once
#include <iostream>
#include <fstream>
#include<string>
#include <windows.h>

using namespace std;

struct Node
{
	int data;
	int bal;
	Node *left;
	Node *right;
};

class AVLtree
{
private:
	int newitem;
	Node * root;
	int count, index;
	int replace;
	int *nums;
	bool done = false;
	bool balancing_happened;
	bool stop;
	bool stopPull;
	bool found;

	void recinorder(Node *temp)
	{
		if (temp == NULL)
		{
			return;
		}
		else
		{
			recinorder(temp->left);
			cout << temp->data << "   ";
			recinorder(temp->right);
		}
	}

	void recpreorder(Node *temp, int min, int mid, int max, int y)
	{
		if (temp == NULL)
		{
			return;
		}
		else
		{
			goTo(mid, y);
			cout << temp->data;
			cout << " (" << temp->bal << ") ";
			recpreorder(temp->left, min, (min + mid) / 2, mid, y + 1);
			recpreorder(temp->right, mid, (mid + max) / 2, max, y + 1);
		}
	}
	void recpostorder(Node *temp)
	{
		if (temp == NULL)
		{
			return;
		}
		else
		{
			recpostorder(temp->left);
			recpostorder(temp->right);
			delete temp;
		}
	}

	void addrec(int item, Node *& current)
	{
		if (current == NULL)
		{
			current = new Node;
			current->data = item;
			current->bal = 0;
			current->left = NULL;
			current->right = NULL;
			done = true;
		}
		else if (item > current->data)
		{
			addrec(item, current->right);
			if (balancing_happened) return;
			if (done)
			{
				if (!stop)
				{
					if (current->bal == -1)
					{
						current->bal++;
						stop = true;
					}
					else
					{
						current->bal++;
					}
				}

				if (current->right->bal > 0 && current->bal == 2)
				{
					current->bal = 0;
					current->right->bal = 0;
					Node *temp = current;
					current = temp->right;
					temp->right = current->left;
					current->left = temp;
					balancing_happened = true;
				}
				else if ((current->right->bal > 0 && current->bal == -2) || (current->right->bal < 0 && current->bal == 2))
				{
					Node *temp = current;
					if (temp->right->left->bal == 0)
					{
						current->bal = 0;
						current->right->bal = 0;
					}
					else if (temp->right->left->bal == 1)
					{
						temp->right->left->bal = 0;
						current->bal = -1;
						current->right->bal = 0;
					}
					else if (temp->right->left->bal == -1)
					{
						temp->right->left->bal = 0;
						current->bal = 0;
						current->right->bal = 1;
					}
					current = temp->right->left;
					temp->right->left = current->right;
					current->right = temp->right;
					temp->right = current->left;
					current->left = temp;
					balancing_happened = true;
				}
			}
		}
		else
		{
			addrec(item, current->left);
			if (balancing_happened) return;
			if (done)
			{
				if (!stop)
				{
					if (current->bal == 1)
					{
						current->bal--;
						stop = true;
					}
					else
					{
						current->bal--;
					}
				}

				if (current->left->bal < 0 && current->bal == -2)
				{
					current->bal = 0;
					current->left->bal = 0;
					Node *temp = current;
					current = temp->left;
					temp->left = current->right;
					current->right = temp;
					balancing_happened = true;
				}

				else if ((current->left->bal < 0 && current->bal == 2) || (current->left->bal > 0 && current->bal == -2))
				{
					Node *temp = current;
					if (temp->left->right->bal == 0)
					{
						current->bal = 0;
						current->left->bal = 0;
					}
					else if (temp->left->right->bal == 1)
					{
						temp->left->right->bal = 0;
						current->bal = 0;
						current->left->bal = -1;
					}
					else if (temp->left->right->bal == -1)
					{
						temp->left->right->bal = 0;
						current->bal = 1;
						current->left->bal = 0;
					}
					current = temp->left->right;
					temp->left->right = current->left;
					current->left = temp->left;
					temp->left = current->right;
					current->right = temp;
					balancing_happened = true;
				}
			}
		}
	}

	int pullup(Node *&trav)
	{
		if (trav->right == NULL)
		{
			replace = trav->data;
			delete trav;
			trav = NULL;
			return replace;
		}
		else
		{
			replace = pullup(trav->right);
			if (!stopPull)
			{
				trav->bal--;

				if (trav->bal == -2 && trav->left->bal == -1)
				{
					trav->bal = 0;
					trav->left->bal = 0;
					Node *temp = trav;
					trav = temp->left;
					temp->left = trav->right;
					trav->right = temp;
				}
				else if (trav->bal == -2 && trav->left->bal == 0)
				{
					trav->bal = -1;
					trav->left->bal = 1;
					Node *temp = trav;
					trav = temp->left;
					temp->left = trav->right;
					trav->right = temp;
				}
				else if ((trav->left->bal < 0 && trav->bal == 2) || (trav->left->bal > 0 && trav->bal == -2))
				{
					Node *temp = trav;
					if (temp->left->right->bal == 0)
					{
						trav->bal = 0;
						trav->left->bal = 0;
					}
					else if (temp->left->right->bal == 1)
					{
						temp->left->right->bal = 0;
						trav->bal = 0;
						trav->left->bal = -1;
					}
					else if (temp->left->right->bal == -1)
					{
						temp->left->right->bal = 0;
						trav->bal = 1;
						trav->left->bal = 0;
					}
					trav = temp->left->right;
					temp->left->right = trav->left;
					trav->left = temp->left;
					temp->left = trav->right;
					trav->right = temp;
				}
				if (trav->bal == 1 || trav->bal == -1)
				{
					stopPull = true;
				}
			}
			return replace;
		}
	}
	void delrec(int item, Node *&trav)
	{
		if (trav == NULL)
		{
			return;
		}
		else if (item > trav->data)
		{
			delrec(item, trav->right);
			if (!stop && !stopPull)
			{
				trav->bal--;

				if (trav->bal == -2 && trav->left->bal == -1)
				{
					trav->bal = 0;
					trav->left->bal = 0;
					Node *temp = trav;
					trav = temp->left;
					temp->left = trav->right;
					trav->right = temp;
				}
				else if (trav->bal == -2 && trav->left->bal == 0)
				{
					trav->bal = -1;
					trav->left->bal = 1;
					Node *temp = trav;
					trav = temp->left;
					temp->left = trav->right;
					trav->right = temp;
				}
				else if ((trav->left->bal < 0 && trav->bal == 2) || (trav->left->bal > 0 && trav->bal == -2))
				{
					Node *temp = trav;
					if (temp->left->right->bal == 0)
					{
						trav->bal = 0;
						trav->left->bal = 0;
					}
					else if (temp->left->right->bal == 1)
					{
						temp->left->right->bal = 0;
						trav->bal = 0;
						trav->left->bal = -1;
					}
					else if (temp->left->right->bal == -1)
					{
						temp->left->right->bal = 0;
						trav->bal = 1;
						trav->left->bal = 0;
					}
					trav = temp->left->right;
					temp->left->right = trav->left;
					trav->left = temp->left;
					temp->left = trav->right;
					trav->right = temp;
				}
				if (trav->bal == 1 || trav->bal == -1)
				{
					stop = true;
				}
			}
		}
		else if (item<trav->data)
		{
			delrec(item, trav->left);
			if (!stop && !stopPull)
			{
				trav->bal++;

				if (trav->bal == 2 && trav->right->bal == 1)
				{
					trav->bal = 0;
					trav->right->bal = 0;
					Node *temp = trav;
					trav = temp->right;
					temp->right = trav->left;
					trav->left = temp;
				}
				else if (trav->bal == 2 && trav->right->bal == 0)
				{
					trav->bal = 1;
					trav->right->bal = -1;
					Node *temp = trav;
					trav = temp->right;
					temp->right = trav->left;
					trav->left = temp;
				}
				else if ((trav->right->bal > 0 && trav->bal == -2) || (trav->right->bal < 0 && trav->bal == 2))
				{
					Node *temp = trav;
					if (temp->right->left->bal == 0)
					{
						trav->bal = 0;
						trav->right->bal = 0;
					}
					else if (temp->right->left->bal == 1)
					{
						temp->right->left->bal = 0;
						trav->bal = -1;
						trav->right->bal = 0;
					}
					else if (temp->right->left->bal == -1)
					{
						temp->right->left->bal = 0;
						trav->bal = 0;
						trav->right->bal = 1;
					}
					trav = temp->right->left;
					temp->right->left = trav->right;
					trav->right = temp->right;
					temp->right = trav->left;
					trav->left = temp;
				}
				if (trav->bal == 1 || trav->bal == -1)
				{
					stop = true;
				}
			}
		}
		else
		{
			found = true;
			if (trav->left == NULL && trav->right == NULL)
			{
				delete trav;
				trav = NULL;
			}
			else if (trav->left != NULL && trav->right != NULL)
			{
				trav->data = pullup(trav->left);
			}
			else if (trav->left == NULL && trav->right != NULL)
			{
				Node *temp = trav;
				trav = temp->right;
				delete temp;
			}
			else if (trav->right == NULL && trav->left != NULL)
			{
				Node *temp = trav;
				trav = temp->left;
				delete temp;
			}
		}
	}

	void goTo(int x, int y)
	{
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		COORD pos;
		pos.X = x; //Column
		pos.Y = y; //Row
		SetConsoleCursorPosition(hConsole, pos);
	}


public:
	AVLtree()
	{
		root = NULL;
		count = 0;
		index = 0;
	}

	~AVLtree()
	{
		clear();
		delete[] nums;
	}
	void add(int item)
	{
		done = false;
		balancing_happened = false;
		stop = false;
		stopPull = false;
		addrec(item, root);
		count++;
	}

	void deleteItem(int item)
	{
		stop = false;
		found = false;
		delrec(item, root);
		if (!found)
		{
			cout << "No such item found" << endl;
		}
	}

	void show()
	{
		system("cls");
		int minX = 0;
		int midX = 40;
		int maxX = 80;
		int y = 0;
		recpreorder(root, minX, midX, maxX, y);
		system("pause");
	}

	void inorder()
	{
		index = 0;
		recinorder(root);
		cout << endl;
		system("pause");
	}

	void clear()
	{
		recpostorder(root);
		root = NULL;
		count = 0;
	}
};