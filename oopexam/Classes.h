#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <ctime>
#include <iomanip>
#include <conio.h>

using namespace std;

static int dayMax[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

static string Date()
{
	string date;
	time_t t = time(0);
	tm now;
	localtime_s(&now, &t);
	date.append(to_string(now.tm_year + 1900));
	date.append((to_string(now.tm_mon + 1).size() == 1 ? "0" + to_string(now.tm_mon + 1) : to_string(now.tm_mon + 1)));
	date.append(to_string(now.tm_mday));
	date.append(" ");
	date.append((to_string(now.tm_hour).size() == 1 ? "0" + to_string(now.tm_hour) : to_string(now.tm_hour)));
	date.append((to_string(now.tm_min).size() == 1 ? "0" + to_string(now.tm_min) : to_string(now.tm_min)));
	date.append((to_string(now.tm_sec).size() == 1 ? "0" + to_string(now.tm_sec) : to_string(now.tm_sec)));
	return date;
}

static string EnterDate()
{
	int year = 0, month = 0, day = 0;
	do
	{
		cout << "Enter year: ";
		cin >> year;
		if (year < 1 || year > stoi(Date().substr(0, 4)))
			cout << "Invalid input. Try again." << endl;
	} while (year < 1 || year > stoi(Date().substr(0, 4)));
	do
	{
		cout << "Enter month: ";
		cin >> month;
		if (month < 1 || month > 12)
			cout << "Invalid input. Try again." << endl;
	} while (month < 1 || month > 12);
	int dayM = dayMax[month - 1];
	if (year % 4 == 0)
		dayM = 29;
	do
	{
		cout << "Enter day: ";
		cin >> day;
		if (day < 1 || day > dayM)
			cout << "Invalid input. Try again." << endl;
	} while (day < 1 || day > dayM);
	string result;
	if (to_string(year).size() < 4)
	{
		for (size_t i = 0; i < 4 - to_string(year).size(); i++)
			result.append("0");
	}
	result.append(to_string(year));
	result.append(to_string(month).size() == 1 ? "0" + to_string(month) : to_string(month));
	result.append(to_string(day).size() == 1 ? "0" + to_string(day) : to_string(day));
	return result;
}

class Item
{
	size_t code = 0;
	string barcode = "0";
	string name = "";
	double price = 0;
	bool dividible = false;
public:
	Item() {}
	Item(size_t code, string barcode, string name, double price, bool dividible) :
		code(code), barcode(barcode), name(name), price(price), dividible(dividible) {}
	void Info()
	{
		cout << setw(5) << "Code" << setw(15) << "Barcode" << setw(30) << "Name"
			<< setw(10) << "Price" << setw(7) << "Div" << endl;
		cout << setw(5) << GetCode() << setw(15) << GetBarcode() << setw(30) << GetName()
			<< setw(10) << GetPrice() << setw(7) << GetDiv() << endl;
	}
	void SetCode(size_t code)
	{
		this->code = code;
	}
	void SetBarcode(string barcode)
	{
		this->barcode = barcode;
	}
	void SetName(string name)
	{
		if (name.size() > 0)
			this->name = name;
	}
	void SetPrice(double price)
	{
		if (price > 0)
			this->price = price;
	}
	void SetDiv(bool div)
	{
		dividible = div;
	}
	size_t GetCode() const { return code; }
	string GetBarcode() const { return barcode; }
	string GetName() const { return name; }
	double GetPrice() const { return price; }
	bool GetDiv() const { return dividible; }
};

class Receipt abstract
{
protected:
	const size_t number;
	map<Item*, double> list;
	double total = 0;
public:
	Receipt(size_t number) :number(number) {}
	map<Item*, double>& GetList()
	{
		return list;
	}
	virtual void AddItem(Item* item, double quantity)
	{
		list[item] += quantity;
	}
	void Show()
	{
		if (list.size() > 0)
		{
			for (auto el : list)
			{
				cout << el.first->GetName() << endl;
				cout << "x " << el.second << "\tCost: " << el.first->GetPrice() * el.second << endl;
			}
			cout << endl;
			cout << "TOTAL: " << Total() << endl;
		}
		else
		{
			cout << " ---- Receipt is empty ---- " << endl;
		}
	}
	virtual double Total()
	{
		total = 0;
		for (auto item : list)
			total += (item.first->GetPrice() * item.second);
		return total;
	}
	double Pay()
	{
		Total();
		ofstream out("Receipts.txt", ios::app);
		if (out.is_open())
		{
			out << Date() << "\t" << number << "\t" << total << endl;
		}
		else
			cerr << "Error writing 'Receipts.txt'. Please contact your tech support." << endl;
		return total;
	}
	bool isEmpty()
	{
		return list.size() == 0;
	}
	size_t GetNumber() const { return number; }
};

class FiscalR : public Receipt
{
public:
	FiscalR(size_t number) :Receipt(number) {}
};

class ReturnR : public Receipt
{
public:
	ReturnR(size_t number) :Receipt(number) {}
	void AddItem(Item* item, double quantity)
	{
		list[item] -= quantity;
	}
};

class Storage
{
private:
	Storage()
	{
		Load();
	}
	static Storage* instance;
	map<size_t, pair<Item*, double>> items;
	vector<Item*> items_vector;
public:
	void Highlight(bool state)
	{
		HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
		if (state)
			SetConsoleTextAttribute(hStdOut, 240);
		else
			SetConsoleTextAttribute(hStdOut, 15);
	}
	void Print(int highlightLine = -1)
	{
		if (items_vector.size() == 0)
			cout << "Storage is empty." << endl;
		else
		{
			cout << setw(5) << "Code" << setw(15) << "Barcode" << setw(30) << "Name"
				<< setw(10) << "Price" << setw(7) << "Div" << setw(10) << "Quantity" << endl;
			for (size_t i = 0; i < items_vector.size(); i++)
			{
				if (i == highlightLine)
					Highlight(1);
				cout << setw(5) << items_vector[i]->GetCode() << setw(15) << items_vector[i]->GetBarcode()
					<< setw(30) << items_vector[i]->GetName() << setw(10) << items_vector[i]->GetPrice()
					<< setw(7) << items_vector[i]->GetDiv() << setw(10) << items[items_vector[i]->GetCode()].second << endl;
				//cout << endl;
				//cout << __FUNCTION__ << endl;
				Highlight(0);
			}
		}
	}
	Item* GetItem(size_t code)
	{
		auto item = items.find(code);
		if (item == items.end())
		{
			cerr << "Error. No such code." << endl;
			return nullptr;
		}
		else
			return items[code].first;
	}
	Item* GetItem(string barcode)
	{
		//TODO
	}
	Item* GetItem()
	{
		int code = -1, value = 0;
		while (code != 13) {
			system("cls");
			code = -1;
			value = (value + items_vector.size()) % items_vector.size();
			cout << "Choose item and press 'Enter' button." << endl;
			Print(value);
			while (code != 72 && code != 80 && code != 13)
			{
				code = _getch();
				if (code == 72)
					value--;
				else if (code == 80)
					value++;
			}
		}
		return items_vector[value];
	}
	static Storage* GetInstance()
	{
		if (instance == nullptr)
			instance = new Storage();
		return instance;
	}
	void Load()
	{
		ifstream in("storage.txt");
		if (!in.is_open())
		{
			cerr << "Error. Can't open storage.txt." << endl;
			return;
		}
		else
		{
			size_t code;
			string barcode = "";
			string name = "";
			double price = 0;
			bool dividible = false;
			size_t quantity;
			while (in.peek() != EOF/*cin.fail()*//*!cin.eof()*/)
			{
				in >> code >> barcode;
				in.get();
				getline(in, name);
				in >> price >> dividible >> quantity;
				Item* tmp = new Item(code, barcode, name, price, dividible);
				items_vector.push_back(tmp);
				items.insert({ code, { tmp,quantity } });
			}
		}
	}
	void Save()
	{
		ofstream out("storage.txt");
		if (!out.is_open())
		{
			cerr << "Error. Can't open storage.txt." << endl;
			return;
		}
		else
		{
			if (items.size() > 0)
			{
				for (auto el : items)
				{
					out << el.second.first->GetCode() << endl;
					out << el.second.first->GetBarcode() << endl;
					out << el.second.first->GetName() << endl;
					out << el.second.first->GetPrice() << endl;
					out << el.second.first->GetDiv() << endl;
					out << el.second.second << endl;
				}
			}
		}
	}
	void AddItem()
	{
		size_t code;
		string barcode = "";
		string name = "";
		double price = 0;
		bool dividible = false;
		size_t quantity;
		cout << "Enter code: ";
		cin >> code;
		if (items.find(code) != items.end())
		{
			cerr << "Error. Code is already used." << endl;
			return;
		}
		cout << "Enter name: ";
		cin.get();
		getline(cin, name);
		cout << "Enter barcode (enter 0 if none): ";
		cin >> barcode;
		cout << "Enter price: ";
		cin >> price;
		cout << "Dividible (1/0): ";
		cin >> dividible;
		cout << "Enter quantity: ";
		cin >> quantity;
		Item* tmp = new Item(code, barcode, name, price, dividible);
		items_vector.push_back(tmp);
		items.insert({ code, {tmp,quantity} });
	}
	void EditItem()
	{
		Item* tmp = GetItem();
		string tmp_str;
		double tmp_double = -1;
		int selection = -1;
		tmp->Info();
		cout << "Remaining in storage: " << items[tmp->GetCode()].second << endl;
		cout << "Edit: (1) Barcode  (2) Name  (3) Price  (4) Div type  (5) Quantity  (0) Exit" << endl;
		cout << "Choose option: ";
		cin >> selection;
		switch (selection)
		{
		case 1:
			cout << "Enter barcode: ";
			cin >> tmp_str;
			tmp->SetBarcode(tmp_str);
			break;
		case 2:
			cout << "Enter name: ";
			getline(cin, tmp_str);
			tmp->SetName(tmp_str);
			break;
		case 3:
			cout << "Enter price: ";
			cin >> tmp_double;
			tmp->SetPrice(tmp_double);
			break;
		case 4:
			while (true)
			{
				cout << "Enter item type (if dividible - 1, if not - 0): ";
				cin >> tmp_double;
				if (tmp_double == 1 || tmp_double == 0)
					break;
				cout << "Invalid input. Try again." << endl;
			}
			tmp->SetDiv(tmp_double);
			break;
		case 5:
			while (true)
			{
				cout << "Enter quantity: ";
				cin >> tmp_double;
				if (tmp_double < 0)
					cout << "Error. Quantity must be bigger or equal zero." << endl;
				else if (int(tmp->GetDiv()) < (tmp_double - int(tmp_double)))
					cout << "Error. This item is not dividible." << endl;
				else
					break;
			}
			items[tmp->GetCode()].second = tmp_double;
			break;
		case 0:
			cout << "Have a nice day" << endl;
			break;
		default:
			cout << "Invalid input. Try again." << endl;
			break;
		}
	}
	void DeleteItem(size_t code = 0)
	{
		if (code == 0)
		{
			cout << "Enter code to delete: ";
			cin >> code;
		}
		auto del = items.find(code);
		if (del != items.end())
		{
			items_vector.erase(find(items_vector.begin(), items_vector.end(), del->second.first));
			items.erase(del);
		}
		else
			cerr << "Error. No such code." << endl;
	}
	void ParseReceipt(Receipt* rec)
	{
		ofstream out("Storage_movement.txt", ios::app);
		if (out.is_open())
		{
			for (auto el : rec->GetList())
			{
				items[el.first->GetCode()].second -= el.second;
				out << Date() << "\t" << rec->GetNumber() << "\t" << el.first->GetCode() << "\t"
					<< el.first->GetPrice() << "\t" << -el.second << "\t" << el.first->GetName() << endl;
			}
		}
		else
			cerr << "Error writing 'Storage_movement.txt'. Please contact your tech support." << endl;
	}
	void PeriodInfo()
	{
		map<string, double> income;
		map<string, double> outcome;
		int quantity = 0;
		cout << "Enter period start date." << endl;
		string start = EnterDate();
		cout << "Enter period end date." << endl;
		string end = EnterDate();
		string tmp;
		if (start > end)
		{
			tmp = start;
			start = end;
			end = tmp;
		}
		string date;
		ifstream in("Storage_movement.txt");
		if (!in.is_open())
			cerr << "Error. Records file is unavailable." << endl;
		else
		{
			while (in.peek() != EOF)
			{
				in >> date;
				if (date > end)
					break;
				if (date >= start)
				{
					in >> tmp >> tmp >> tmp >> tmp >> tmp;
					in.get();
					quantity = stoi(tmp);
					getline(in, tmp);
					if (quantity > 0)
						income[tmp] += quantity;
					else
						outcome[tmp] += quantity;
				}
				else
					getline(in, tmp);
			}
			system("cls");
			cout << "Storage period report (" << start.substr(0, 4) << "-" << start.substr(4, 2) << "-" << start.substr(6) << " : ";
			cout << end.substr(0, 4) << "-" << end.substr(4, 2) << "-" << end.substr(6) << ")" << endl;
			cout << "Income:" << endl;
			if (income.size() > 0)
				for (auto el : income)
					cout << setw(5) << el.second << "\t" << el.first << endl;
			else
				cout << " - " << endl;
			cout << "Outcome:" << endl;
			if (outcome.size() > 0)
				for (auto el : outcome)
					cout << setw(5) << el.second << "\t" << el.first << endl;
			else
				cout << " - " << endl;
		}
	}
	~Storage()
	{
		Save();
	}
};

Storage* Storage::instance = nullptr;

class Shop
{
private:
	Shop()
	{
		ifstream in("shop.txt");
		if (in.is_open())
		{
			getline(in, shopName);
			in >> lastReceipt;
		}
		else
		{
			string owner, name, location;
			cout << "Enter shop info." << endl;
			cout << "Owner name: ";
			getline(cin, owner);
			cout << "Shop name:	";
			getline(cin, name);
			cout << "Location: ";
			getline(cin, location);
			shopName = owner + " :: " + name + " :: " + location;
			ofstream out("shop.txt");
			if (out.is_open())
			{
				cout << shopName << endl;
				cout << lastReceipt;
			}
			else
				cerr << "Error writing 'shop.txt'. Please contact your tech support." << endl;
		}
	}
	static Shop* instance;
	string shopName = "";
	size_t lastReceipt = 0;
	Storage* storage = nullptr;
	Receipt* receipt = nullptr;
public:
	static Shop* GetInstance()
	{
		if (instance == nullptr)
			instance = new Shop();
		return instance;
	}
	void SetStorage(Storage* storage)
	{
		this->storage = storage;
	}
	void OpenFiscalRec()
	{
		if (receipt == nullptr)
			receipt = new FiscalR(lastReceipt + 1);
		else
			cerr << "Error. Close opened receipt first." << endl;
	}
	void OpenReturnRec()
	{
		if (receipt == nullptr)
			receipt = new ReturnR(lastReceipt + 1);
		else
			cerr << "Error. Close opened receipt first." << endl;
	}
	void CloseRec()
	{
		if (receipt != nullptr)
		{
			if (!receipt->isEmpty())
			{
				receipt->Pay();
				storage->ParseReceipt(receipt);
				receipt = nullptr;
				lastReceipt++;
				cout << "Receipt closed." << endl;
			}
			else
				cerr << "Error. Receipt is empty. You can cancel empty receipt." << endl;
		}
		else
			cerr << "Error. No open receipt." << endl;
	}
	void CancelRec()
	{
		if (receipt != nullptr)
		{
			delete receipt;
			receipt = nullptr;
			cout << "Receipt was canceled." << endl;
		}
	}
	Receipt* GetRec()
	{
		return receipt;
	}
	void XReport()
	{
		PeriodInfo(Date().substr(0, 8));
	}
	void PeriodInfo(string date = "0")
	{
		double summ = 0;
		string start, end;
		if (date == "0")
		{
			cout << "Enter period start date." << endl;
			start = EnterDate();
			cout << "Enter period end date." << endl;
			end = EnterDate();
		}
		else
			start = end = date;
		string tmp;
		if (start > end)
		{
			tmp = start;
			start = end;
			end = tmp;
		}
		ifstream in("Receipts.txt");
		if (!in.is_open())
			cerr << "Error. Records file is unavailable." << endl;
		else
		{
			while (in.peek() != EOF)
			{
				in >> date;
				if (date > end)
					break;
				if (date >= start)
				{
					in >> tmp >> tmp >> tmp;
					in.get();
					summ += stoi(tmp);
				}
				else
					getline(in, tmp);
			}
			system("cls");
			cout << "Shop period report (" << start.substr(0, 4) << "-" << start.substr(4, 2) << "-" << start.substr(6) << " : ";
			cout << end.substr(0, 4) << "-" << end.substr(4, 2) << "-" << end.substr(6) << ")" << endl;
			cout << "Income: ";
			if (summ > 0)
				cout << summ << endl;
			else
				cout << " - " << endl;
		}
	}
	void ReceiptManip()
	{
		int selection = -1;
		double quantity = 0;
		Item* tmp = nullptr;
		while (receipt != nullptr)
		{
			system("cls");
			cout << shopName << endl;
			cout << endl;
			receipt->Show();
			cout << endl;
			cout << "(1) Add item  (2) Add item by code  (3) Close  (4) Cancel" << endl;
			cout << "Choose operation: ";
			cin >> selection;
			switch (selection)
			{
			case 1:
				tmp = storage->GetItem();
			case 2:
				if (tmp == nullptr)
					do
					{
						cout << "Enter item code: ";
						cin >> selection;
						tmp = storage->GetItem(selection);
					} while (tmp == nullptr);
					system("cls");
					while (true)
					{
						tmp->Info();
						cout << "Enter quantity: ";
						cin >> quantity;
						if (quantity <= 0)
							cout << "Error. Quantity must be bigger then zero." << endl;
						else if (int(tmp->GetDiv()) < (quantity - int(quantity)))
							cout << "Error. This item is not dividible." << endl;
						else
							break;
					}
					receipt->AddItem(tmp, quantity);
					tmp = nullptr;
					break;
			case 3:
				CloseRec();
				break;
			case 4:
				CancelRec();
				break;
			default:
				cout << "Invalid input. Try again." << endl;
				break;
			}
			system("pause");
		}
	}
	void Interface()
	{
		int selection = -1;
		while (selection != 0)
		{
			system("cls");
			cout << shopName << endl;
			cout << endl;
			if (receipt != nullptr)
				receipt->Show();
			else
				cout << "There is no open receipt found." << endl;
			cout << endl;
			cout << "(1) Fiscal receipt  (2) Return receipt  (3) X report  (4) Period report  (0) Exit" << endl;
			cout << "Choose operation: ";
			cin >> selection;
			switch (selection)
			{
			case 1:
				OpenFiscalRec();
				ReceiptManip();
				break;
			case 2:
				OpenReturnRec();
				ReceiptManip();
				break;
			case 3:
				XReport();
				system("pause");
				break;
			case 0:
				cout << "Have a nice day." << endl;
				system("pause");
				break;
			default:
				cout << "Invalid input. Try again." << endl;
				system("pause");
				break;
			}
		}
	}
};

Shop* Shop::instance = nullptr;