#pragma once

#include<iostream>
#include<fstream>
#include<list>
#include<iterator>
#include<vector>
#include <stdio.h>
using namespace std;
class tnode;
class BinaryTree;
class HashTable;
class HashTable
{
	int tableSize;
	vector<list<pair<int, char> > > table;
public:
	HashTable(int m)
	{
		tableSize = m;
		
		//table = new vector<list<pair<int, char> > >;
		table.resize(tableSize);
	}
	int hash(char c)
	{
		return c % tableSize;
	}
	void insertKeyPair(int code, char c)
	{
		int index = hash(c);
		table[index].push_back(make_pair(code, c));

	}
	void insert(char c)
	{
		int index = hash(c);//computes hash
		if (!(table[index].empty()))
		{
			list<pair<int, char> >::iterator it = (table[index].begin());
			for (it; it != (table[index].end()); ++it)//checks whether character already stored
			{
				if (it->second == c)
				{
					it->first++;//frequency incremented
					break;
				}
			}
			if (it == table[index].end())//character not present in hash table
			{
				table[index].push_back(make_pair(1, c));
			}

		}
		else
		{
			/*list<pair<int, char> > l;
			table[index][0] = l;*/
			table[index].push_back(make_pair(1, c));
		}
	}
	void createTree(BinaryTree**& forests);
	
	friend class minHeap;
};
class HashTable2
{
	int tableSize2;
	vector<list<pair<char*, char> > > table;
public:
	HashTable2(int m)
	{
		tableSize2 = m;

		//table = new vector<list<pair<int, char> > >;
		table.resize(tableSize2);
	}
	int hash(char c)
	{
		return c % tableSize2;
	}
	void insertKeyPair(char* code, char c)
	{
		int index = hash(c);
		int j,x;
		for (j = 0; code[j] != '\0'; j++);
		j++;
		char* temp = new char[j];
		for (x = 0; code[x] != '\0'; x++)
		{
			temp[x] = code[x];
		}
		temp[x] = '\0';
		table[index].push_back(make_pair(temp, c));

	}
	void printCodes()
	{
		for (int i = 0; i < tableSize2; i++)
		{
			if (!table[i].empty())
			{
				list<pair<char*, char> >:: iterator it = (table[i].begin());
				for (it; it != table[i].end(); ++it)
				{
					cout << it->second << "  ";
					for (int j = 0; (it->first)[j] != '\0'; j++)
					{
						cout << it->first[j];
					}
					cout << endl;
				}
			}
		}
	}
	char* getCode(char c)
	{
		int index = hash(c);
		list<pair<char*, char> >::iterator it = (table[index].begin());//iterator to traverse the required list
		for (it; it != (table[index].end()); ++it)
		{
			if (it->second == c)
			{
				return it->first;
			}
		}
	}
	bool searchCode(string s, char& c)
	{
		for (int x = 0; x < tableSize2; x++)
		{
			if (!table[x].empty())
			{
				list<pair<char*, char> > ::iterator it = table[x].begin();
				for (it; it != table[x].end(); ++it)
				{
					if (strcmp(it->first, s.c_str()) == 0)
					{
						c = it->second;
						return true;
					}

				}
			}
		
		}
		return false;
	}
	friend class minHeap;
};
class tnode
{
	tnode* left;
	tnode* right;
	char character;
	int freq;
public:
	tnode(char c, int f, tnode* l = 0, tnode* r = 0)
	{
		character = c;
		freq = f;
		left = l;
		right = r;
	}
	tnode(tnode* x, tnode* y)
	{
		freq = x->freq;
		freq += y->freq;
		left = x;
		right = y;
		character = '-';
	}
	friend class BinaryTree;
	friend class minHeap;
};

class BinaryTree
{
	tnode* root;
public:
	friend class tnode;
	friend class minHeap;
	BinaryTree(tnode* p)
	{
		root = p;
	}
	BinaryTree(char c, int f, tnode* l = 0, tnode* s = 0)
	{
	
		root = new tnode(c, f, l, s);
		
	}
	void hashCodes(HashTable2& obj)
	{
		char array[30];
		int count = 0;
		hashCodes(root, array, count, obj);
	}
	void hashCodes(tnode* root, char array[], int count, HashTable2& myHash)
	{
		if (root->left == nullptr && root->right == nullptr)
		{
			array[count] = '\0';
			myHash.insertKeyPair(array, root->character);
			return;
		}
		if (root->left != nullptr)
		{
			array[count] = '0';
			hashCodes(root->left, array, count + 1, myHash);

		}
		if (root->right != nullptr)
		{
			array[count] = '1';
			hashCodes(root->right, array, count + 1, myHash);
		}

	}
	
};

class minHeap
{
	tnode** array;
	int maxSize;
	int heapSize;
public:
	minHeap(int s = 10)
	{
		maxSize = s;
		heapSize = 0;
		array = new tnode*[s];
	}
	void BuildHeap()
	{
		for (int i = heapSize / 2; i >= 1; i--)
		{
			MinHeapify(i);
		}
	}
	void MinHeapify(int i)
	{
		int left = 2 * i;
		int right = 2 * i + 1;
		int smallest = i;
		if (left <= heapSize && array[left]->freq < array[smallest]->freq)
			smallest = left;
		if (right <= heapSize&& array[right]->freq < array[smallest]->freq)
			smallest = right;
		if (smallest != i)
		{
			swap(array[i], array[smallest]);
			MinHeapify(smallest);
		}
	}
	void insert(BinaryTree* ptr)
	{
		heapSize++;
		array[heapSize] = ptr->root;
	}
	void insertNewNode(tnode* myNode)
	{
		heapSize++;
		array[heapSize] = myNode;
		int i = heapSize;
		while (i > 1)
		{
			if (((array[i])->freq) <  ((array[i / 2])->freq))
			{
				swap(array[i], array[i / 2]);
				i = i / 2;
			}
			else
				break;
		}


	}
	tnode* extractNode()
	{
		tnode* ptr = array[1];
		array[1] = array[heapSize];
		heapSize--;
		MinHeapify(1);
		return ptr;
	}
	tnode* generateCodes()
	{
		while (heapSize > 1)
		{
			tnode* X = extractNode();
			tnode* Y = extractNode();
			tnode* Z = new tnode(X, Y);
			insertNewNode(Z);

		}
		return extractNode();
	}
	friend class BinaryTree;
};


void HashTable::createTree(BinaryTree**& forests)
{
	int count = 0;
	for (int x = 0; x < tableSize; x++)
	{
		if (!table[x].empty())
		{
			list<pair<int, char> >::iterator it = (table[x].begin());
			for (it; it != (table[x].end()); ++it)
			{
				int f = it->first;
				char c = it->second;
				forests[count] = new BinaryTree(c, f);
				count++;
			}

		}


	}
}