#include "PQLParser.h"
#include <string.h>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

PQLParser::PQLParser(string input)
{
	Tokenize(input);
}

void PQLParser::Tokenize(string input)
{
	vector<string> token = vectorize(input);

	if (find(token.begin(), token.end(), "procedure") != token.end()) {
		tokenizeProcedure(token);
	}

	if (find(token.begin(), token.end(), "variable") != token.end()) {
		tokenizeVariable(token);
	}

	if (find(token.begin(), token.end(), "Modifies") != token.end()) {
		tokenizeModifies(token);
	}

	if (find(token.begin(), token.end(), "Uses") != token.end()) {
		tokenizeUses(token);
	}

	if (find(token.begin(), token.end(), "Parent*") != token.end()) {
		tokenizeParentS(token);
	} else if (find(token.begin(), token.end(), "Parent") != token.end()) {
		tokenizeParent(token);
	}

	if (find(token.begin(), token.end(), "Follows*") != token.end()) {
		tokenizeFollowsS(token);
	} else if (find(token.begin(), token.end(), "Follows") != token.end()) {
		tokenizeFollows(token);
	}

}

vector<string> PQLParser::vectorize(string input)
{
	vector<string> tokens;
	char *d = " ";
	char *p;
	char *temp = (char*)input.c_str();
	p = strtok(temp, d);
	while (p) {
		tokens.push_back(p);
		p = strtok(NULL, d);
	}
	return tokens;
}

queue<pair<TokenType, string>> PQLParser::getDeclarationQueue()
{
	return declarationQueue;
}

queue<pair<TokenType, pair<string, string>>> PQLParser::getSelectQueue()
{
	return selectQueue;
}


void PQLParser::tokenizeVariable(vector<string> token)
{
	declarationQueue.push(make_pair(TokenType::Keyword, "variable"));
	declarationQueue.push(make_pair(TokenType::Identifier, token[1]));
	declarationQueue.push(make_pair(TokenType::Separator, ";"));
}

void PQLParser::tokenizeProcedure(vector<string> token)
{
	declarationQueue.push(make_pair(TokenType::Keyword, "procedure"));
	declarationQueue.push(make_pair(TokenType::Identifier, token[1]));
	declarationQueue.push(make_pair(TokenType::Separator, ";"));
}

void PQLParser::tokenizeSelect(vector<string> token) {

}

void PQLParser::tokenizePattern(vector<string>)
{
}

void PQLParser::tokenizeFollows(vector<string>)
{
}

void PQLParser::tokenizeFollowsS(vector<string>)
{
}

void PQLParser::tokenizeParent(vector<string>)
{
}

void PQLParser::tokenizeParentS(vector<string>)
{
}
void PQLParser::tokenizeUses(vector<string>)
{
}

void PQLParser::tokenizeModifies(vector<string>)
{
}


