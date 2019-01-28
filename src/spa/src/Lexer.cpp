#include<stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <iterator>

using namespace std;

#include "Lexer.h"
#include "PKB.h"
#include "TNode.h"

Lexer::Lexer(string input) {
	code = input;
	Tokenize(input);
}

int Lexer::Tokenize(string input) {

	vector<string> tokens = vectorize(input);

	//identifying different types of statement
	if (find(tokens.begin(), tokens.end(), "procedure") != tokens.end()) {
		tokenizeProcedure(tokens);
	}

	if (find(tokens.begin(), tokens.end(), "=") != tokens.end()) {
		tokenizeAssignment(tokens);
	}

	return 0;

}
vector<string> Lexer::vectorize(string input) {
	vector<string> tokens;
	istringstream iss(input);
	copy(istream_iterator<string>(iss),
		istream_iterator<string>(),
		back_inserter(tokens));

	return tokens;
}

void Lexer::tokenizeProcedure(vector<string> tokens) {
	tokenQueue.push(make_pair(TokenType::Keyword, "procedure"));
	tokenQueue.push(make_pair(TokenType::Identifier, tokens[1]));
	tokenQueue.push(make_pair(TokenType::Separator, "{"));
}

void Lexer::tokenizeAssignment(vector<string> tokens) {
	tokenQueue.push(make_pair(TokenType::Keyword, "assign"));
	pushToTokenQueue(tokens);
}

void Lexer::pushToTokenQueue(vector<string> tokens) {
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
	}
}

queue<pair<TokenType, string>> Lexer::getTokenQueue() {
	return tokenQueue;
}

vector<string> Lexer::arrangeSemiColon(vector<string> tokens) {
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

bool Lexer::isIdentifier(string s) {
	return (!onlyContainDigits(s));
}

bool Lexer::isSeparator(string s) {
	return (s == ";" || s == "{" || s == "}" || s == "(" || s == ")");

}

bool Lexer::isOperator(string s) {
	return (s == "+" || s == "-" || s == "*" || s == "/" || s == "=");
}

bool Lexer::isLiteral(string s) {
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

bool Lexer::onlyContainDigits(string s) {
	if (s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) {
		return false;
	}

	char * p;
	strtol(s.c_str(), &p, 10);

	return (*p == 0);
}


