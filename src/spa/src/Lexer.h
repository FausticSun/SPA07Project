#pragma once

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
};

class Lexer {
	string code;
public:
	Lexer(string input);
	queue<pair<TokenType, string>> getTokenQueue();
	

private:
	int Tokenize(string);
	queue<pair<TokenType, string>> tokenQueue;
	vector<string> vectorize(string);
	void tokenizeProcedure(vector<string>);
	void tokenizeAssignment(vector<string>);
	void pushToTokenQueue(vector<string>);
	vector<string> arrangeSemiColon(vector<string>);
	bool isIdentifier(string);
	bool isSeparator(string);
	bool isOperator(string);
	bool isLiteral(string);
	bool onlyContainDigits(string);

};