/*
 * myjson.cpp
 *
 *  Created on: Oct 25, 2018
 *      Author: menright
 */
#include "json.h"
using namespace std;
int main(int argc, char** argv) {
	shared_ptr<Value> val =
	parse_value_from_file(argv[1]);
	print_value(val);
	return 0;
}
