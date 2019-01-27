#pragma once

#include<string>
#include<queue>
#include<utility>

using namespace std;

enum class TokenType {
	Keyword,
	Identifier,
	Separator,
	Operator,
	Literal,
	Fail
};

class Parser {
	string code;
public:
	Parser(string input);
	int Parse(string);

private:
	queue<pair<TokenType, string>> tokenQueue;
	void parseProcedure(vector<string>);
	void parseAssignment(vector<string>);
	void pushToTokenQueue(vector<string>);
	vector<string> arrangeSemiColon(vector<string>);
	bool isIdentifier(string);
	bool isSeparator(string);
	bool isOperator(string);
	bool isLiteral(string);
	bool onlyContainDigits(string);

};
