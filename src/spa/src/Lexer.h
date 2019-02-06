#pragma once

#include<Token.h>
#include<string>
#include<queue>
#include<utility>

using namespace std;

class Lexer {

public:
	Lexer();
	~Lexer();
	vector<Token*> tokenize(string);

private:
	int statementLine;
	vector<string> vectorize(string);
	vector<Token*> tokenizeProcedure(vector<string>);
	vector<Token*> tokenizeAssignment(vector<string>);
	vector<Token*> tokenizeRead(vector<string>);
	vector<Token*> tokenizePrint(vector<string>);
	Token* pushIdentifier(string);
	Token* pushOperator(string);
	Token* pushToken(string);
	bool isIdentifier(string);
	bool isSeparator(string);
	bool isOperator(string);
	bool isConstant(string);
	bool onlyContainDigits(string);

};
