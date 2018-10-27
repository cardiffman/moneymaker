/*
 * json.cpp
 *
 *  Created on: Oct 26, 2018
 *      Author: menright
 */
#include "json.h"

#include <string>
#include <map>
#include <vector>
#include <memory>
#include <fstream>
#include <iostream>

using namespace std;
bool parse_number(shared_ptr<Value>& value, fstream& input) {
	int c = input.get();
	while ((c==' ' || c=='\t' || c=='\r' || c=='\n') && input.good())
		c = input.get();
	/*
	 * In this, recognizing a number is regarded
	 * as feasible while converting the recognized token to
	 * a number is delegated to the library.
	 */
	string token;
	if (c == '-') {
		token.append(1,c);
		c = input.get();
	} else if (!isdigit(c)) {
		input.unget();
		return false;
	}
	int dot=0;
	while (input.good() && (isdigit(c) || (dot==0&&c=='.'))) {
		token.append(1,c);
		if (c=='.') dot++;
		c = input.get();
	}
	if (c=='e' || c=='E') {
		token.append(1,c);
		c = input.get();
		if (c == '-'||c=='+') {
			token.append(1,c);
			c = input.get();
		}
		while (isdigit(c)) {
			token.append(1,c);
			c = input.get();
		}
	}
	input.unget();
	double num = std::stof(token);
	//cout << "Parsing token " << token << " as number giving " << num << endl;
	value = make_shared<Number>(num);
	return true;
}
bool parse_keyword(shared_ptr<Value>& value, fstream& input) {
	int c = input.get();

	while (c==' ' || c=='\t' || c=='\r' || c=='\n')
		c = input.get();
	string keyword;
	if (!isalpha(c)) {
		input.unget();
		return false;
	}
	while (isalpha(c)) {
		keyword.append(1, c);
		c = input.get();
	}
	input.unget();
	if (keyword == "null") {
		value = make_shared<Null>();
		return true;
	} else if (keyword == "true") {
		value = make_shared<Boolean>(true);
		return true;
	} else if (keyword == "false") {
		value = make_shared<Boolean>(false);
		return true;
	}
	return false;
}
bool parse_string(shared_ptr<Value>& value, fstream& input) {
	int c = input.get();

	while (c==' ' || c=='\t' || c=='\r' || c=='\n')
		c = input.get();

	if (c != '"') {
		input.unget();
		return false;
	}
	c = input.get();
	string inner;
	while (true) {
		if (c == '"') {
			value = make_shared<String>(inner);
			return true;
		}
		if (c == '\\') {
			c = input.get();
			inner.append(1, c);
			c = input.get();
		} else {
			inner.append(1, c);
			c = input.get();
		}
		if (!input.good())
			break;
	}
	return false;
}
bool parse_object(shared_ptr<Value>& value, fstream& input);
void print_value(shared_ptr<Value>& val);
bool parse_array(shared_ptr<Value>& value, fstream& input) {
	int c = input.get();

	//cout << "Parsing array" << endl;
	while ((c==' ' || c=='\t' || c=='\r' || c=='\n') && input.good())
		c = input.get();

	if (c != '[') {
		input.unget();
		return false;
	}
	c = input.get();
	vector<shared_ptr<Value>> inner;
	while (c != ']' && input.good()) {
		input.unget(); // put back chr checked for ']'
		shared_ptr<Value> t;
		if (parse_number(t, input)
			|| parse_string(t,input)
			|| parse_array(t,input)
			|| parse_keyword(t,input)
			|| parse_object(t,input)
			) {
			//cout << "Parsed value "; print_value(t); cout << endl;
			inner.push_back(t);
			c = input.get();
			while ((c==' ' || c=='\t' || c=='\r' || c=='\n') && input.good())
				c = input.get();
			if (c != ',')
				break;
			c = input.get(); // not right yet.
		} else {
			break;
		}
	}
	value = make_shared<Array>(inner);
	return true;
}
bool parse_object(shared_ptr<Value>& value, fstream& input) {
	int c = input.get();
	while ((c==' ' || c=='\t' || c=='\r' || c=='\n') && input.good())
		c = input.get();

	if (c != '{') {
		input.unget();
		return false;
	}
	//cout << "Parsing object" << endl;
	c = input.get();
	map<string,shared_ptr<Value>> inner;
	while (c != '}' && input.good()) {
		input.unget(); // put back chr checked for '}'
		shared_ptr<Value> n;
		if (!parse_string(n, input)) {
			return false;
		}
		string name = dynamic_cast<String&>(*n).value;
		//cout << "Parsed name " << name << endl;
		c = input.get();
		while ((c==' ' || c=='\t' || c=='\r' || c=='\n') && input.good())
			c = input.get();
		if (c != ':')
			break;

		shared_ptr<Value> t;
		if (parse_number(t, input)
			|| parse_string(t,input)
			|| parse_array(t,input)
			|| parse_keyword(t,input)
			|| parse_object(t,input)
			) {
			//cout << "Parsed value "; print_value(t); cout << endl;
			inner[name] = t;
			c = input.get();
			while ((c==' ' || c=='\t' || c=='\r' || c=='\n') && input.good())
				c = input.get();
			if (c != ',')
				break;
			c = input.get();
		} else {
			break;
		}
	}
	value = make_shared<Object>(inner);
	return true;
}
shared_ptr<Value> parse_value_from_file(const string& file) {
	shared_ptr<Value> value = make_shared<Null>();
	fstream input;
	input.open(file, std::ios::binary | std::fstream::in);
	if (input.good()) {
		if (!parse_number(value, input)
			&& !parse_string(value,input)
			&& !parse_array(value,input)
			&& !parse_object(value,input)
			&& !parse_keyword(value,input)) {
			throw "Input failure";
		}
		cout << "Must have parsed something?" <<endl;
	} else {
		cout << "IO fail" << endl;
		throw "IO fail";
	}
	return value;
}
void print_value(shared_ptr<Value>& val) {
	if (dynamic_cast<Null*>(val.get())) {
		cout << "null";
		return;
	}
	auto b = dynamic_cast<Boolean*>(val.get());
	if (b) {
		if (b->value)
			cout << "true";
		else cout << "false";
		return;
	}
	auto n = dynamic_cast<Number*>(val.get());
	if (n) {
		cout << n->value;
		return;
	}
	auto s = dynamic_cast<String*>(val.get());
	if (s) {
		cout << s->value;
		return;
	}
	auto a = dynamic_cast<Array*>(val.get());
	if (a) {
		cout << "[ ";
		for (unsigned i=0; i<a->values.size(); i++) {
			if (i!=0)
				cout << ',';
			print_value(a->values[i]);
		}
		cout << " ]";
		return;
	}
	auto o = dynamic_cast<Object*>(val.get());
	if (o) {
		cout << "{ ";
		for (auto i = o->values.begin(); i!=o->values.end(); i++) {
			if (i!=o->values.begin())
				cout << ',';
			cout << i->first << ": ";
			print_value(i->second);
		}
		cout << " }";
	}
}




