
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
		parseAssignment(tokens);
	}

	/**
	queue<pair<TokenType, string>> q = tokenQueue;
	while (!q.empty())
	{
		string type;
		TokenType t = q.front().first;
		switch (t) {
		case TokenType::Keyword: type = "keyword"; break;
		case TokenType::Identifier: type = "identifier"; break;
		case TokenType::Operator: type = "operator"; break;
		case TokenType::Literal: type = "literal"; break;
		case TokenType::Separator: type = "separator"; break;
		case TokenType::Fail: type = "fail"; break;

		}
		cout << type << " " << q.front().second << endl;
		q.pop();
	}
	std::cout << std::endl;
	**/

	return 0;

}

void Parser::parseProcedure(vector<string> tokens) {
	tokenQueue.push(make_pair(TokenType::Keyword, "procedure"));
	tokenQueue.push(make_pair(TokenType::Identifier, tokens[1]));
	tokenQueue.push(make_pair(TokenType::Separator, "{"));
}

void Parser::parseAssignment(vector<string> tokens) {
	tokenQueue.push(make_pair(TokenType::Keyword, "assign"));
	pushToTokenQueue(tokens);
}

void Parser::pushToTokenQueue(vector<string> tokens) {
	tokens = arrangeSemiColon(tokens);
	for (auto i : tokens) {
		if (isLiteral(i)) {
			tokenQueue.push(make_pair(TokenType::Literal, i));
		}
		else if (isSeparator(i)) {
			tokenQueue.push(make_pair(TokenType::Separator, i));
		}
		else if (isOperator(i)) {
			tokenQueue.push(make_pair(TokenType::Operator, i));
		}
		else if (isIdentifier(i)) {
			tokenQueue.push(make_pair(TokenType::Identifier, i));
		}
		else {
			tokenQueue.push(make_pair(TokenType::Fail, i));
		}
	}
}

vector<string> Parser::arrangeSemiColon(vector<string> tokens) {
	int target;
	for (int i = 0; i < tokens.size(); i++) {
		if (tokens[i].find(';') != string::npos) {
			target = i;
			tokens[target] = tokens[target].substr(0, tokens[target].size() - 1);
			tokens.insert(tokens.begin() + target + 1, ";");
			break;
		}
	}

	return tokens;
}

bool Parser::isIdentifier(string s) {
	return (!onlyContainDigits(s));
}

bool Parser::isSeparator(string s) {
	return (s == ";" || s == "{" || s == "}" || s == "(" || s == ")");

}

bool Parser::isOperator(string s) {
	return (s == "+" || s == "-" || s == "*" || s == "/" || s == "=");
}

bool Parser::isLiteral(string s) {
	if (s == "true" || s == "false") {
		return true;
	}
	else if (s[0] == '"') {
		return true;
	}
	else {
		return onlyContainDigits(s);
	}
}

bool Parser::onlyContainDigits(string s) {
	if (s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) {
		return false;
	}

	char * p;
	strtol(s.c_str(), &p, 10);

	return (*p == 0);
}



