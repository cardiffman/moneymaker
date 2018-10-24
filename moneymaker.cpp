#include "moneymaker.h"
#include <memory>
#include <sstream>
#include <windows.h>

using namespace std;
shared_ptr<Money> moneyptr;
void foo() {
	moneyptr->accounts;
}
Account acct = {
		"One",
		{ // list of budget transactions
				{{ .30, 30, "cash" }}
		},
		{ // journal of actual transactions
				{{ 0, 0, "init" }}
		}
};
Money money = {
		{ // list of accounts
				{
						"Checking",
						{ // list of budget transactions
								{{ 3375.0, P_BiWeekly, "Jethead", X_Deposit, T_In },
								{972.0, P_Monthly, "Mortgage", X_Spend, T_Out}}

						},
						{ // journal of actual transactions
								{{ 0, 0, "init" }}
						}
				},{
						"MC-MBNA",
						{ // list of budget transactions
								{{ 150.0, P_Monthly, "Verizon", X_Spend, T_Out }}
						},
						{ // journal of actual transactions
								{{ 0, 0, "init" }}
						}
				}

		}
};
void DrawMoney(HDC hdc) {
	int y=0;
	for (auto account : money.accounts) {
		ExtTextOutA(hdc, 0, y, ETO_OPAQUE, nullptr, account.name.data(), account.name.size(), nullptr);
		y+=16;
		for (auto x : account.journal.transactions) {
			x.time;
			x.amount;
			x.counter;
			ostringstream os;
			os << "Date: " << x.time << " $" << x.amount << " " << x.counter << ' ' <<x.transfer << ' ' << x.type;
			auto t = os.str();
			ExtTextOutA(hdc, 0, y, ETO_OPAQUE, nullptr, t.data(), t.size(), nullptr);
			y+=16;
		}
	//}
	//for (auto account : money.accounts) {
	//	ExtTextOutA(hdc, 0, y, ETO_OPAQUE, nullptr, account.name.data(), account.name.size(), nullptr);
	//	y+=16;
		for (auto x : account.budget.plan) {
			x.period;
			x.amount;
			x.counter;
			ostringstream os;
			os << "Per: " << x.period << " $" << x.amount << " " << x.counter << ' ' <<x.transfer << ' ' << x.type;
			auto t = os.str();
			ExtTextOutA(hdc, 0, y, ETO_OPAQUE, nullptr, t.data(), t.size(), nullptr);
			y+=16;
		}
	}
}
