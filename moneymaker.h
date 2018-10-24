#pragma once
#include <windows.h>
#include <list>
#include <string>
#include <iostream>

enum XType {
	X_Spend, X_Deposit
};
inline std::string to_string(XType xt) {
	switch (xt) {
	case X_Spend: return "spend";
	case X_Deposit: return "deposit";
	}
	return "_";
}
inline std::ostream& operator<<(std::ostream& os, XType xt) {
	return os << to_string(xt);
}
enum Transfer {
	T_In, T_Out, T_CashOut, T_CashIn, T_AssetBuy
};
inline std::string to_string(Transfer xt) {
	switch (xt) {
	case T_In: return "xfer in";
	case T_Out: return "xfer out";
	case T_CashOut: return "cash out";
	case T_CashIn: return "cash in";
	case T_AssetBuy: return "asset buy";
	}
	return "_";
}
inline std::ostream& operator<<(std::ostream& os, Transfer xt) {
	return os << to_string(xt);
}
struct Transaction {
	double amount;
	int time;
	std::string counter;
	XType type;
	Transfer transfer;
};
enum Period {
	P_Daily = -1,
	P_Weekly = -7,
	P_BiWeekly = -14,
	P_Monthly = -30,
	P_BiMonthly = -60
};
struct BudgetTransaction {
	double amount;
	int period;
	int startTime;
	std::string counter;
	XType type;
	Transfer transfer;
};
struct Budget {
	std::list<BudgetTransaction> plan;
};
struct Journal {
	std::list<Transaction> transactions;
};
struct Account {
	std::string name;
	Budget budget;
	Journal journal;
};
struct Money {
	std::list<Account> accounts;
};
struct MoneyView {
	struct ViewItem {
		double amount;
		int time;
		std::string counter;
		XType type;
		Transfer transfer;
	};
};

void DrawMoney(HDC hdc);
