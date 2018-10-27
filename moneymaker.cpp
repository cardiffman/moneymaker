#include "moneymaker.h"
#include "json.h"
#include <memory>
#include <sstream>
#include <windows.h>
#include <iostream>

using namespace std;
shared_ptr<Money> moneyptr;
void foo() {
	moneyptr->accounts;
}

shared_ptr<Value> money;
void LoadAccounts() {
	money = parse_value_from_file("money.json");
	cout << "Accounts loaded" << endl;
	print_value(money);
}
string as_string(shared_ptr<Value>& v) {
	return dynamic_cast<String&>(*v).value;
}
int as_int(shared_ptr<Value>& v) {
	return dynamic_cast<Number&>(*v).value;
}
double as_double(shared_ptr<Value>& v) {
	return dynamic_cast<Number&>(*v).value;
}
void DrawMoney(HDC hdc) {
	int y=0;
	try {
	auto m = dynamic_cast<Object&>(*money).values;
	auto budget = m["budget"];
	auto plans = dynamic_cast<Array&>(*budget).values;
	auto journal = m["journal"];
	auto entries = dynamic_cast<Array&>(*journal).values;
	for (auto entry : entries) {
		ostringstream os;
		auto e = dynamic_cast<Object&>(*entry).values;
		os << "Date " << as_int(e["time"]) << " $" << as_double(e["amount"]) << " " << as_string(e["subject"]) << " " << as_string(e["counter"]) << " " << as_string(e["transfer"]) << " " << as_string(e["type"]);
		auto t = os.str();
		ExtTextOutA(hdc, 0, y, ETO_OPAQUE, nullptr, t.data(), t.size(), nullptr);
		y+=16;
	}
	} catch (...) {
		cout << "Caught exception" << endl;
	}
}
