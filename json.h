/*
 * json.h
 *
 *  Created on: Oct 26, 2018
 *      Author: menright
 */

#ifndef JSON_H_
#define JSON_H_

#include <memory>
#include <string>
#include <map>
#include <vector>

struct Value {
	virtual ~Value() {}
};
struct Object : public Value {
	std::map<std::string, std::shared_ptr<Value>> values;
	Object(std::map<std::string,std::shared_ptr<Value>> values) : values(values) {}
};

struct Array : public Value {
	std::vector<std::shared_ptr<Value>> values;
	Array(std::vector<std::shared_ptr<Value>> values) : values(values) {}
};

struct String : public Value {
	std::string value;
	String(std::string value) : value(value) {}
};
struct Number : public Value {
	double value;
	Number(double value) : value(value) {}
};
struct Boolean : public Value {
	bool value;
	Boolean(bool value) : value(value) {}
};
struct Null : public Value {
	nullptr_t value;
};

void print_value(std::shared_ptr<Value>& val);
std::shared_ptr<Value> parse_value_from_file(const std::string& file);

#endif /* JSON_H_ */
