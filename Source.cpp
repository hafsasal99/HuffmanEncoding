#include<iostream>
#include<fstream>
#include<list>
#include<iterator>
#include<vector>
#include "Header.h"
#include <stdio.h>
using namespace std;

void main()
{
	HashTable H1(59);//hash table for characters & their frequency
	fstream fin;
	fin.open("sampleFile.txt");
	char input;
	while (!fin.eof())
	{
		fin >> input;
		H1.insert(input);
	}
	BinaryTree** forests;//trees with single nodes of each distinct character
	forests = new BinaryTree*[59];
	for (int j = 0; j < 59; j++)
	{
		forests[j] = 0;
	}
	H1.createTree(forests);//creates single node tree of all elements in hash table
	int numOFCharacters = 0;
	for (int i = 0; i < 59; i++)
	{
		if (forests[i] != 0)
			numOFCharacters++;
		else
			break;
	}
	minHeap codes(numOFCharacters);
	for (int j = 0; j < numOFCharacters; j++)//inserting characters in heap
	{
		codes.insert((forests[j]));
	}
	codes.BuildHeap();
	HashTable2 H2(59);//hash table for characters and their respective Huffman codes
	BinaryTree tree((codes.generateCodes()));
	tree.hashCodes(H2);
	H2.printCodes();
	fin.close();
	fin.open("sampleFile.txt");
	char c = 0;//c is the 8 bit packet containing packed codes
	int bitCount=0;
	int index = 0;
	unsigned char nextChar;
	char codedCharacters[100] = { 0 };//stores the stream of encoded characters to be stored in file
	while (!fin.eof())
	{
		fin >> nextChar;
		char * code = H2.getCode(nextChar);
		for (int j=0; code[j] != '\0'; j++, bitCount++)
		{
			if (bitCount == 8)//a packet of 8 bits is formed
			{
				codedCharacters[index] = c;
				bitCount = 0;//initialising count for another 8 bit packet
				index++;
				c = 0;//all 8 bits of a packet set to 0
			}
			if (code[j] == '1')
			{
				c = c | 0x01 << bitCount;//the code is stored from right to left
			
			}
		}
		/*if (bitCount != 0)
		{
			codedCharacters[index] = c;
			index++;
		}*/
	}
	fin.close();
	ofstream fout;
	fout.open("sampleFile.txt");
	for (int x = 0; x < index; x++)//encoding/writing to file the characters obtained after encoding
	{
		fout << codedCharacters[x];
	}
	fout.close();
	fin.open("sampleFile.txt");
	string Hcode;//string to extract huffman code from the character code of each of the encoded characters
	char originalChars[100];
	char ref;//to get the original character corresonding to the huffman code obtained
	index = 0;
	while (!fin.eof())
	{
		fin >> nextChar;

		for (int x = 0; x < 8; x++)
		{
			if ((nextChar >> x) & 0x1)//right shifting moves the bit we are interested in to the extreme right,its value is then determined by AND
				Hcode += '1';
			else
				Hcode += '0';
			if (H2.searchCode(Hcode, ref)==true)
			{
				originalChars[index] = ref;
				index++;
				Hcode.clear();
			}

		}

	}
	fin.close();
	fout.open("sampleFile.txt");
	for (int j = 0; j < index; j++)
	{
		fout << originalChars[j];
	}
	fout.close();
	system("pause");

}