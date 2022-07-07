#include<iostream>
#include<pthread.h>
#include<string>
#include<fstream>
#include<regex>

using namespace std;

string website;
int count = 0;

struct Node
{
    int number;
    string link;
    Node* next;

    Node(int, string);
};

class LinkedList
{
public:
    Node* head;
    int count;

    LinkedList();

    void insert(int, string);
    string search(int);
    void display();
};


void* parentLinkData(void*);
void* childLinkData(void*);
LinkedList extractLinks();

int main()
{
    pthread_t parent;
    LinkedList list;
    int choice;

    cout << "Enter A Website To Crawl : ";
    cin >> website;

    //Downloading HTML file
    pthread_create(&parent, NULL, parentLinkData, NULL);
    pthread_join(parent, NULL);
    //getting links from file
    list = extractLinks();

again:

    cout << "\n\n1.Crawl To A Link Of Your Choice\n2.Crawl All Links\n3.Exit\nEnter Your Choice : \n";
    cin >> choice;

    switch (choice)
    {
    case 1:
    {
        /*The program will crawl to the link of user's choice and after that user can 
        crawl to the links extracted from that link*/

        cout << "Enter A Link Number To Crawl : ";
        cin >> choice;

        if (choice <= list.count)
        {
            website = list.search(choice);
            pthread_t crawl;

            pthread_create(&crawl, NULL, parentLinkData, NULL);
            pthread_join(crawl, NULL);

            system("clear");
            list = extractLinks();
        }

        else 
            cout << "Invalid Input.\n";

        goto again;
        break;
    }

    case 2:
    {
        /*The program will crawl to all the links extracted from the link user entered*/
        pthread_t crawler[list.count];
        Node* curr = list.head;

        for (int i = 0; i < list.count; i++)
        {
            website = curr->link;

            pthread_create(&crawler[i], NULL, childLinkData, NULL);
            pthread_join(crawler[i], NULL);

            curr = curr->next;
        }
        break;
    }

    case 3:
        exit(0);

    default:
        cout << "Invalid Input.\n";
        goto again;
        break;
    }

    return 0;
}

void* parentLinkData(void*)
{
    string browser, link;

    browser = "' Mozilla/5.0 (X11; Linux x86_64; rv:60.0) Gecko/20100101 Firefox/60.0' ";

    //using 'wget --user-agent' to download html file 
    link = "wget -q -O htmlFile.html --user-agent=" + browser + website;

    //Downloading html file
    system(link.c_str());

    return 0;
}

void* childLinkData(void*)
{
    string browser, link;
    
    browser = "' Mozilla/5.0 (X11; Linux x86_64; rv:60.0) Gecko/20100101 Firefox/60.0' ";
    link = "wget -q -O htmlFile.html --user-agent=" + browser + website;

    system(link.c_str());

    extractLinks();
    return 0;
}

LinkedList extractLinks()
{
    LinkedList list;
    ifstream searchHtml;

    searchHtml.open("htmlFile.html");

    if (!searchHtml)
    {
        cout << "File Not Stored.\n";
        exit(0);
        return list;
    }

    string linkFinder;
    int index, end;

    //To check if link is valid
    regex regPattern("((http|https)://)(www.)?[a-zA-Z0-9@:%._\\+~#?&//=]{2,256}\\.[a-z]{2,6}\\b([-a-zA-Z0-9@:%._\\+~#?&//=]*)");

    while (!searchHtml.eof())
    {
        searchHtml >> linkFinder;

        if (linkFinder.find("href=")!=-1)
        {
            //Sorting link from the tag
            index = linkFinder.find("=");
            linkFinder = linkFinder.substr(index + 2);
            //Removing extra symbols
            end = linkFinder.find("?");
            linkFinder = linkFinder.substr(0, end);

            //Storing in linked list
            if (regex_match(linkFinder, regPattern))
                list.insert(++list.count, linkFinder);
        }
    }

    if (list.count == 0)
        cout << "No Links Found.\n";
    else
    {
        cout << "\nFollowing Are The Links Fetched From The Website : \n\n";
        list.display();
    }

    return list;
}

Node::Node(int n, string l)
{
    number = n;
    link = l;
    next = NULL;
}

LinkedList::LinkedList()
{
    head = NULL;
    count = 0;
}

void LinkedList::insert(int n, string link)
{
    if (head == NULL)
    {
        head = new Node(n, link);
        return;
    }

    Node* curr = head;
    while (curr->next != NULL)
        curr = curr->next;

    curr->next = new Node(n, link);
}

string LinkedList::search(int n)
{
    Node* curr = head;

    while (curr != NULL)
    {
        if (n == curr->number)
            return curr->link;
        curr = curr->next;
    }

    return "";
}

void LinkedList::display()
{
    Node* curr = head;

    while (curr != NULL)
    {
        cout << curr->number << "  " << curr->link << endl;
        curr = curr->next;
    }
}
