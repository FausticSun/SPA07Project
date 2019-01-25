
#include<stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <iterator>

using namespace std;

#include "Parser.h"
#include "PKB.h"
#include "TNode.h"

Parser::Parser(string input) {
	code = input;
	Parse(input);
}

int Parser::Parse(string input) {

	vector<string> tokens;
	istringstream iss(input);
	copy(istream_iterator<string>(iss),
		istream_iterator<string>(),
		back_inserter(tokens));

	//identifying different types of statement
	if (find(tokens.begin(), tokens.end(), "procedure") != tokens.end()) {
		parseProcedure(tokens);
	}

	if (find(tokens.begin(), tokens.end(), "=") != tokens.end()) {
		//parseAssignment(tokens);
	}

	return 0;

}

void Parser::parseProcedure(vector<string> tokens) {
	tokenQueue.push(make_pair(TokenType::Keyword, "procedure"));
	tokenQueue.push(make_pair(TokenType::Identifier, tokens[1]));
	tokenQueue.push(make_pair(TokenType::Operator, "{"));


	queue<pair<TokenType, string>> q = tokenQueue;
	while (!q.empty())
	{
		string type;
		TokenType t = q.front().first;
		switch (t) {
		case TokenType::Keyword: type = "keyword"; break;
		case TokenType::Identifier: type = "identifier"; break;
		case TokenType::Operator: type = "operator"; break;

		}
		cout << type << " " << q.front().second << endl;
		q.pop();
	}
	std::cout << std::endl;

}



