#include <iostream>
#include "Classes.h"
using namespace std;

int main()
{
	Storage* s = Storage::GetInstance();
	//s->Load();
	//s->DeleteItem(2);
	//s->Save();
	time_t t = time(0);
	tm now;
	localtime_s(&now, &t);
	cout << (now.tm_year + 1900) << '-' << (now.tm_mon + 1) << '-' << now.tm_mday;
	Receipt* r = new FiscalR("test", 1);
	r->Show();
	system("pause");
	return 0;
}
//
//time_t now = time(0);
//tm ltm;
//localtime_s(&ltm, &now);