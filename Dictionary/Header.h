#ifndef HEADER_H
#define HEADER_H

#include <iostream>
#include<string>
#include<fstream>

using namespace std;

//size of array
const int alphabets = 29;

/* This struct will be used as a node of trie tree */
struct Alphabet
{
	char name;		//name of character
	string explanation;		//Its content if endOfWord==true
	bool endOfWord;			//to check if the word ends here
	Alphabet* child[alphabets];		//child nodes of character

	Alphabet();
	Alphabet(char, bool);
};

/* This Struct is a node for linked list made to store suggestions*/
struct Node
{
	string word, explanation;
	Node* next;

	Node(string , string );
};

/* Stack is made to store history*/
class Stack
{
private:
	string* stackArray;
	int stackSize;
	int top;

public:
	Stack();
	~Stack();

	bool isEmpty();
	bool isFull();

	void Push(string);
	void Pop();
};

/*Linked List to store suggestions*/
class LinkedList
{
public:
	LinkedList();
	~LinkedList();

	void insert(string, string);
	string displayList();

private:
	Node* head;
	int size;
};

/*Trie Tree To store all dataand search*/
class TrieTree
{
public:
	TrieTree();
	~TrieTree();

	void readDataBase();
	void insert(string, string);

	void search();
	void history();
	void suggestions(Alphabet*,string);

private:
	Alphabet* root;
	Stack History;
	LinkedList Suggestions;
};

//This function will remove symbols and convert query to lower case
string convertToLowerCase(string);

void Credits();

#endif // !HEADER_H