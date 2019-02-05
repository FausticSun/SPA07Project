#pragma once

#pragma once

#include<Token.h>
#include<string>
#include<queue>
#include<utility>

using namespace std;

class Lexer {
	string code;
public:
	Lexer(string input);
	queue<pair<TokenType, string>> getTokenQueue();
	

private:
	int tokenize(string);
	queue<pair<TokenType, string>> tokenQueue;
	vector<string> vectorize(string);
	void tokenizeProcedure(vector<string>);
	void tokenizeAssignment(vector<string>);
	void tokenizeRead(vector<string>);
	void tokenizePrint(vector<string>);
	Token* pushIdentifier(string);
	Token* pushOperator(string);
	Token* pushToken(string);
	bool isIdentifier(string);
	bool isSeparator(string);
	bool isOperator(string);
	bool isConstant(string);
	bool onlyContainDigits(string);
	void print(vector<Token*>);

};