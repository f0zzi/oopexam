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
	//double Total()
	//{
	//	cout << "ret" << endl;
	//	for (auto item : list)
	//		total += (item.first->GetPrice() * item.second);
	//	return total;
	//}
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
		ofstream out("storage1.txt");
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
		cout << "Enter barcode (or 0): ";
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
				out << Date() << "\t" << rec->GetNumber() << "\t" << -el.second << "\t" << el.first->GetCode() << "\t" 
					<< el.first->GetName() << "\t" << el.first->GetPrice() << endl;
			}
		}
		else
			cerr << "Error writing 'Storage_movement.txt'. Please contact your tech support." << endl;
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
			in >> Zreport;
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
			shopName = owner + "\n" + name + "\n" + location;
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
	double revenue = 0; //виручка
	size_t receiptCounter = 0; //к-ть чеків за день
	size_t Zreport = 0;
	Receipt* receipt = nullptr;
	//Storage* storage = Storage::GetInstance();
public:
	static Shop* GetInstance()
	{
		if (instance == nullptr)
			instance = new Shop();
		return instance;
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
				receipt = nullptr;
				lastReceipt++;
				receiptCounter++;
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
		}
	}
	Receipt* GetRec()
	{
		return receipt;
	}
	void XReport()
	{
		cout << "Revenue: " << revenue << "\t" << "Number of receipts: " << receiptCounter << endl;
	}
};

Shop* Shop::instance = nullptr;