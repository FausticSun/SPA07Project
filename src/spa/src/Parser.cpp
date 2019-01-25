
#include<stdio.h>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

#include "Parser.h"
#include "PKB.h"
#include "TNode.h"

Parser::Parser(string input) {
	code = input;
	Parse(input);
}

int Parser::Parse(string input) {
	cout << input;

	return 0;
}



