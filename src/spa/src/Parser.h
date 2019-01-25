#pragma once

#include<string>
using namespace std;

class Parser {
	string code;
public:
	Parser(string input);
	int Parse(string);
};
