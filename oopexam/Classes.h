#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <ctime>
using namespace std;

class Item
{
	size_t code;
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
	string header = "";
	const size_t number;
	map<Item*, double> list;
	double total = 0;
public:
	Receipt(string header, size_t number) :number(number), header(header) {}
	void AddItem(Item* item)
	{
		list[item] += 1;
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
			out << number << "\t" << total << endl;
		}
		else
			cerr << "Error writing 'Receipts.txt'. Please contact your tech support." << endl;
	}
};

class FiscalR : public Receipt
{
public:
	FiscalR(string header, size_t number):Receipt(header, number){}
};

class ReturnR : public Receipt
{
public:
	double Total()
	{
		for (auto item : list)
			total -= (item.first->GetPrice() * item.second);
		return total;
	}
};

//size_t Receipt::number = 1;

class Storage
{
private:
	Storage()
	{
		Load();
	}
	static Storage* instance;
	map<size_t, pair<Item*, int>> items;
public:
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
				items.insert({ code, {tmp,quantity} });
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
		cout << "Enter name: ";
		cin >> name;
		cout << "Enter barcode (or 0): ";
		cin >> barcode;
		cout << "Enter price: ";
		cin >> price;
		cout << "Dividible (true/false): ";
		cin >> dividible;
		cout << "Enter quantity: ";
		cin >> quantity;
		Item* tmp = new Item(code, barcode, name, price, dividible);
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
			items.erase(del);
		}
		else
			cerr << "Error. No such code." << endl;
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
			shopName = owner + "\n" + name + "\n" + location;
		}
	}
	static Shop* instance;
	string shopName = "";
	size_t lastReceipt = 1;
	double revenue = 0; //виручка
	size_t receiptCounter = 0; //к-ть чеків за день
public:
	static Shop* GetInstance()
	{
		if (instance == nullptr)
			instance = new Shop();
		return instance;
	}

};

Shop* Shop::instance = nullptr;