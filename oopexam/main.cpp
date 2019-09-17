#include <iostream>
#include <windows.h>
#include "Classes.h"
using namespace std;

int main()
{
	setlocale(LC_ALL, "Rus");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	Storage* s = Storage::GetInstance();
	//cout << s->GetItem()->GetName() << endl;
	//cout << Date() << endl;
	//s->Load();
	//s->DeleteItem(2);
	//s->Save();

	//time_t result = time(NULL);
	//char str[26];
	//ctime_s(str, sizeof str, &result);
	//cout << str << endl;

	/*time_t t = time(0);
	tm now;
	localtime_s(&now, &t);
	cout << (now.tm_year + 1900) << '-' << (to_string(now.tm_mon + 1).size() == 1? "0" + to_string(now.tm_mon + 1):to_string(now.tm_mon + 1)) << '-' << now.tm_mday;*/
	
	//time_t t = time(0);
	//tm now;
	//localtime_s(&now, &t);
	//cout << (now.tm_year + 1900) << '-' << (now.tm_mon + 1) << '-' << now.tm_mday;

	//Receipt* r = new FiscalR(1);
	//Receipt* r2 = new ReturnR(2);
	//r->Show();

	//cout << s->GetItem(1)->GetName() << endl;
	//s->AddItem();
	//s->Save();

	Shop* shop = Shop::GetInstance();
	//shop->OpenFiscalRec();
	//shop->GetRec()->AddItem(s->GetItem(1), 1);
	//shop->GetRec()->AddItem(s->GetItem(2), 2);
	//shop->GetRec()->AddItem(s->GetItem(2), 0.6);
	//shop->GetRec()->AddItem(s->GetItem(), 3);
	//cout << shop->GetRec()->Total() << endl;

	//shop->GetRec()->Show();
	//s->ParseReceipt(shop->GetRec());
	//shop->CloseRec();
	s->PeriodInfo();

	system("pause");
	return 0;
}
//
//time_t now = time(0);
//tm ltm;
//localtime_s(&ltm, &now);