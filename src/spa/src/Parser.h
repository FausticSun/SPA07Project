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
	Number
};

class Parser {
	string code;
public:
	Parser(string input);
	int Parse(string);

private:
	queue<pair<TokenType, string>> tokenQueue;
	void parseProcedure(vector<string>);
};
