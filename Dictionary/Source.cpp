#include"Header.h"

int main()
{
    TrieTree SearchEngine;

    //Reading data 
    SearchEngine.readDataBase();

    int choice = -1;

    while (true)
    {
        system("cls");

        cout << "\tSearch Engine\n1.Search\n2.View History\n3.Credits\n4.Exit\n\n"
            << "Enter Your Choice : ";

        cin >> choice;

        switch (choice)
        {
        case 1:
            SearchEngine.search();
            system("pause");
            break;

        case 2:
            SearchEngine.history();
            system("pause");
            break;

        case 3:
            Credits();
            system("pause");
            break;

        case 4:
            exit(0);

        default:
            cout << "\a\aInvalid Choice!\n";
            cin.clear();
            cin.ignore();
            system("cls");
            break;
        }
    }
}