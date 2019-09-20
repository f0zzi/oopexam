#include <iostream>
#include <windows.h>
#include "Classes.h"
using namespace std;

int main()
{
	setlocale(LC_ALL, "Rus");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	Storage* storage = Storage::GetInstance();
	Shop* shop = Shop::GetInstance();
	shop->SetStorage(storage);
	int selection = -1;
	while (selection != 0)
	{
		system("cls");
		cout << R"(Welcome to "Shop assistant"(c) 0.013a)" << endl << endl;
		cout << "(1) Shop  (2) Storage  (0) Exit" << endl;
		cout << "Choose interface to work with: ";
		cin >> selection;
		switch (selection)
		{
		case 1:
			shop->Interface();
			break;
		case 2:
			storage->Interface();
			break;
		case 0:
			cout << "Have a nice day." << endl;
			break;
		default:
			cout << "Invalid input. Try again." << endl;
			break;
		}
	}

	system("pause");
	return 0;
}