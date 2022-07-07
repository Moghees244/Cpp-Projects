#include"Header.h"

//Constructors
Alphabet::Alphabet()
{
    name = '\0';
    explanation = '\0';
    endOfWord = false;

    for (int i = 0; i < alphabets; i++)
        child[i] = NULL;
}

Alphabet::Alphabet(char n,bool eow)
{
    name = n;
    endOfWord = eow;
    explanation = '\0';

    for (int i = 0; i < alphabets; i++)
        child[i] = NULL;
}

Node::Node(string n, string e)
{
    word = n;
    explanation = e;
    next = NULL;
}

TrieTree::TrieTree()
{
    root = new Alphabet();
}

TrieTree::~TrieTree()
{
}

//This Function will read the data from file
void TrieTree::readDataBase()
{
    string word, explanation;

    ifstream read("DataBase.txt");

    if (!read.is_open())
    {
        cout << "\a\a\nERROR !!!\nUnable To Open File.\n";
        exit(0);
    }

    while (!read.eof())
    {
        //reading data
        getline(read, word, ',');
        getline(read, explanation, '\n');

        //storing in tree
        insert(word, explanation);
    }

    read.close();
    cin.clear();
}

//This function will insert data in trie tree
void TrieTree::insert(string word, string explanation)
{
    Alphabet* currentAlphabet = root;       //Iterator
    word=convertToLowerCase(word);      //converting word to lower case to store easily

    int index = 0;

    //This loop will continue unless the full word is stored
    for (int i = 0; i < word.length(); i++)
    {
        //sort of linear probing(Hashing)
        (word[i] == ' ') ? index = 26 : (word[i] == '#') ? index = 27 : (word[i] == '$') ?
            index = 28 : index = word[i] % 97;

        /* If node for next character doesnot exist create it */
        if (!currentAlphabet->child[index])
            currentAlphabet->child[index] = new Alphabet(word[i],false);

        //moving to next character
        currentAlphabet = currentAlphabet->child[index];
    }

    //when the word is stored, marking its ending point
    currentAlphabet->endOfWord = true;
    //storing content in string
    currentAlphabet->explanation = explanation;
}


/* This Function will Search what user enters in following ways : 
   -> If user enters correct word,then display it
   -> If user enters incomplete word, it will give suggestions
   -> If user enters wrong spellings it will suggest words closest to it
   -> If user enters '$' before word it will display its price if available
   -> If user enters '#' then it will display hashtags
   -> If user enters numbers it will return nums
*/
void TrieTree::search()
{
    string query, sugg = "";

    //taking input
    cin.ignore();
    cout << "Search : ";
    getline(cin, query);

    //storing history
    History.Push(query);

    //iterator and temp (to store previous value of iterator just in case it becomes null)
    Alphabet* iterator = root, * temp = iterator;

    //converting in lowercase
    query = convertToLowerCase(query);

    int index = 0, i = 0;

    //serching
    for (i = 0; i < query.length(); i++)
    {
        (query[i] == ' ') ? index = 26 : (query[i] == '#') ? index = 27 : (query[i] == '$') ?
            index = 28 : index = query[i] % 97;

        //if word not found
        if (!iterator->child[index])
            break;
        
        //moving to next character
        temp = iterator;
        iterator = iterator->child[index];
        //storing characters(this will help in suggestions)
        sugg += query[i];
    }

    //if word found
    if (iterator->endOfWord && i == query.length())
    {
        cout << "\nExplanation :\n" << iterator->explanation << endl;
        return;
    }

    //if word not found
    cout << "\n\aThe Word You Entered Not Found !\n\nData Relevent To Your Search : \n";

    //Looking for suggestions
    for (int i = 0; i < 29; i++)
    {
        if (iterator->child[i])
            suggestions(iterator->child[i], sugg);
    }

    //displaying suggestions and storing history
    History.Push(Suggestions.displayList());
}

//this function will display last 5 searches
void TrieTree::history()
{
    cout << "\n\tHISTORY\n";

    if (History.isEmpty())
        cout << "\nNo Recent History.\n";

    else
        History.Pop();
}

//This function will find suggesions and store them in linked list
void TrieTree::suggestions(Alphabet* currAlphabet,string sugg)
{
    //storing word to be suggested
    if (currAlphabet == NULL)
    {
        cout << "No Suggestions Available.\n";
        return;
    }

    sugg += currAlphabet->name;

    //if complete word is found it is inserted in Suggestions list
    if (currAlphabet->endOfWord == true)
        Suggestions.insert(sugg, currAlphabet->explanation);

    //sort of recurrsion to check child of child
    for (int i = 0; i < 29; i++)
        if (currAlphabet->child[i])
            suggestions(currAlphabet->child[i], sugg);

    return;
}

//constructor
Stack::Stack()
{
    stackArray = new string[5];
    stackSize = 5;
    top = -1;
}

Stack::~Stack(){}

//stroing last 5 searches  in stack
void Stack::Push(string history)
{
    //if stack is full the oldest search will be removed and latest will be added at top
    if (isFull())
    {
        for (int i = 0; i < 4; i++)
            stackArray[i] = stackArray[i+1];

        stackArray[4] = history;
    }

    else
        stackArray[++top] = history;
}

//displaying history
void Stack::Pop()
{
    for (int i = 0; i <= top; i++)
        cout << stackArray[top - i] << endl;
}

//checking if stack is empty
bool Stack::isEmpty()
{
    if (top == -1)
        return true;
    return false;
}

//checking if stack is full
bool Stack::isFull()
{
    if (top == stackSize - 1)
        return true;
    return false;
}

//constructor
LinkedList::LinkedList()
{
    head = NULL;
    size = 1;
}

LinkedList::~LinkedList(){}

//inserting data at end of linked list
void LinkedList::insert(string word, string explanation)
{
    if (head == NULL)
    {
        head = new Node(word,explanation);
        size++;
        return;
    }

    if (size > 100)
        return;

    Node* curr = head;

    while (curr->next != NULL)
        curr = curr->next;

    curr->next = new Node(word, explanation);
    size++;
}

//displaying suggestions and showing details of the one user selects
string LinkedList::displayList()
{
    Node* curr = head;
    int count = 1;

    //displaying suggestions
    while (curr != NULL)
    {
        cout << count << "- " << curr->word << "\t";
        curr = curr->next;
        count++;
    }

    cout << "\nWhich One You Want To See In Detail : ";
    cin >> count;

    //in case user enters greater number 
    if (count > size)
    {
        cout << "Invalid Input.\n";
        system("pause"); system("cls");
        displayList();
    }

    //traversing to word user wants to see
    int count2 = 1;
    curr = NULL;
    curr = head;

    while (curr->next != NULL && count2 != count)
    {
        curr = curr->next;
        count2++;
    }

    //displaying word
    cout << "\n" << curr->word << "\nExplanation :\n" << curr->explanation << endl;

    head = NULL;

    //returning word to store in history
    return curr->word;
}

//This function will remove symbols and convert query to lower case
string convertToLowerCase(string query)
{
   int count = 0;

   //checking if user entered numbers
    for (int i = 0; i < query.length(); i++)
        if (query[i] > 47 && query[i] < 58)
            count++;

    if (count == query.length())
        return "nums";

    count = 0;

    //checking if user enters spaces only
    for (int i = 0; i < query.length(); i++)
        if (query[i] == ' ')
            count++;

    if (count == query.length())
        return "spaces";

    //removing symbols and converting word to lowercase
    for (int i = 0; i < query.length(); i++)
    {
        //these symbols will not be removed
        if (i == 0 && query[i] == '#' || query[i] == '$')
            continue;

        //converting uppercase to lowercase
        if (query[i] > 64 && query[i] < 97 && query[i] != ' ' && query[i] != '\n')
            query[i] += 32;

        else if (query[i] > 96 && query[i] < 123)
            continue;

        //removing symbols
        else
        {
            query[i] = ',';
            query.erase(remove(query.begin(), query.end(), ','));
            i--;
        }
    }

    if (query.length() == 0)
        return "qwdfgbnm";

    return query;
}

void Credits()
{
    cout << "\tCredits\nMoghees Ahmad\n\n";
}
