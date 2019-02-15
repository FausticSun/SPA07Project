#include "PQLLexer.h"
#include <algorithm>
#include <queue>
#include <string.h>
#include <string>
#include <vector>

//modifies and uses(left emplty), pattern a is not a actual assignemnt, type, how to check errors
using namespace std;

PQLLexer::PQLLexer(string input) { Tokenize(input); }

vector<string> PQLLexer::vectorize(string input)
{
	vector<string> tokens;     

	char *p;
	char *temp = (char*)input.c_str();
	p = strtok(temp, " ");
	while (p) {
		tokens.push_back(p);
		p = strtok(NULL, " ");
	}
	return tokens;
}

void PQLLexer::Tokenize(string input)
{
	vector<string> token = vectorize(input);
	while (!token.empty()) {
		if (token[0] == "procedure") {
			tokenizeProcedure(token);
			token.erase(token.begin());
			token.erase(token.begin());
		}


		if (token[0] == "variable") {
			tokenizeVariable(token);
			token.erase(token.begin());
			token.erase(token.begin());
		}


		if (find(token.begin(), token.end(), "stmt") != token.end()) {
			tokenizeStmt(token);
			token.erase(token.begin());
			token.erase(token.begin());
		}


		if (find(token.begin(), token.end(), "read") != token.end()) {
			tokenizeRead(token);
			token.erase(token.begin());
			token.erase(token.begin());
		}


		if (find(token.begin(), token.end(), "print") != token.end()) {
			tokenizePrint(token);
			token.erase(token.begin());
			token.erase(token.begin());
		}


		if (find(token.begin(), token.end(), "while") != token.end()) {
			tokenizeWhile(token);
			token.erase(token.begin());
			token.erase(token.begin());
		}


		if (find(token.begin(), token.end(), "if") != token.end()) {
			tokenizeIf(token);
			token.erase(token.begin());
			token.erase(token.begin());
		}


		if (find(token.begin(), token.end(), "call") != token.end()) {
			tokenizeCall(token);
			token.erase(token.begin());
			token.erase(token.begin());
		}


		if (find(token.begin(), token.end(), "assign") != token.end()) {
			tokenizeAssign(token);
			token.erase(token.begin());
			token.erase(token.begin());
		}


		if (find(token.begin(), token.end(), "constant") != token.end()) {
			tokenizeConstant(token);
			token.erase(token.begin());
			token.erase(token.begin());
		}

		//if (find(token.begin(), token.end(), "select") != token.end()) {
		if (token[0] == "select") {
			tokenizeSelect(token);
			token.erase(token.begin());
			token.erase(token.begin());
			if (token.size() > 0) {
				if (token[0] == "such" && token[1] == "that") {
					token.erase(token.begin());
					token.erase(token.begin());
					while (0 < token.size())
					{
						if (find(token.begin(), token.end(), "Modifies") != token.end()) {
							tokenizeModifies(token);
							token.erase(token.begin());
							token.erase(token.begin());
							token.erase(token.begin());
						}



						if (find(token.begin(), token.end(), "Uses") != token.end()) {
							tokenizeUses(token);
							token.erase(token.begin());
							token.erase(token.begin());
							token.erase(token.begin());
						}


						if (find(token.begin(), token.end(), "Parent*") != token.end()) {
							tokenizeParentT(token);
							token.erase(token.begin());
							token.erase(token.begin());
							token.erase(token.begin());
						}
						else if (find(token.begin(), token.end(), "Parent") != token.end()) {
							tokenizeParent(token);
							token.erase(token.begin());
							token.erase(token.begin());
							token.erase(token.begin());
						}

						if (find(token.begin(), token.end(), "Follows*") != token.end()) {
							tokenizeFollowsT(token);
							token.erase(token.begin());
							token.erase(token.begin());
							token.erase(token.begin());
						}
						else if (find(token.begin(), token.end(), "Follows") != token.end()) {
							tokenizeFollows(token);
							token.erase(token.begin());
							token.erase(token.begin());
							token.erase(token.begin());
						}
					}
				}
			}
		}


	}

}
bool isInt(string s)
{
	if (s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false;


	char * p;
	strtol(s.c_str(), &p, 10);


	return (*p == 0);
}

queue<pair<TokenType, string>> PQLLexer::getTokenQueue()
{
	return tokenQueue;
}


string PQLLexer::getTarget()
{
	return target;
}

string PQLLexer::SplitSemi(string s)
{
	s = s.substr(0, s.length - 1);
	return s;
}


void PQLLexer::tokenizeVariable(vector<string> token)
{
	string variableName = token[1];
	variableName = SplitSemi(variableName);
	tokenQueue.push(make_pair(TokenType::Keyword, "variable"));
	tokenQueue.push(make_pair(TokenType::Identifier, variableName));
	tokenQueue.push(make_pair(TokenType::Separator, ";"));
}


void PQLLexer::tokenizeProcedure(vector<string> token)
{
	string procedureName;
	procedureName = token[1];
	procedureName = SplitSemi(procedureName);
	tokenQueue.push(make_pair(TokenType::Keyword, "procedure"));
	tokenQueue.push(make_pair(TokenType::Identifier, procedureName));
	tokenQueue.push(make_pair(TokenType::Separator, ";"));
}


void PQLLexer::tokenizeRead(vector<string> token)
{
	string readName;
	readName = token[1];
	readName = SplitSemi(readName);
	tokenQueue.push(make_pair(TokenType::Keyword, "read"));
	tokenQueue.push(make_pair(TokenType::Identifier, readName));
	tokenQueue.push(make_pair(TokenType::Separator, ";"));
}


void PQLLexer::tokenizePrint(vector<string> token)
{
	string printName;
	printName = token[1];
	printName = SplitSemi(printName);
	tokenQueue.push(make_pair(TokenType::Keyword, "print"));
	tokenQueue.push(make_pair(TokenType::Identifier, printName));
	tokenQueue.push(make_pair(TokenType::Separator, ";")); 
}


void PQLLexer::tokenizeWhile(vector<string> token)
{
	string whileName;
	whileName = token[1];
	whileName = SplitSemi(whileName);
	tokenQueue.push(make_pair(TokenType::Keyword, "while"));
	tokenQueue.push(make_pair(TokenType::Identifier, whileName));
	tokenQueue.push(make_pair(TokenType::Separator, ";"));
}


void PQLLexer::tokenizeIf(vector<string> token)
{
	string ifName;
	ifName = token[1];
	ifName = SplitSemi(ifName);
	tokenQueue.push(make_pair(TokenType::Keyword, "if"));
	tokenQueue.push(make_pair(TokenType::Identifier, ifName));
	tokenQueue.push(make_pair(TokenType::Separator, ";"));
}


void PQLLexer::tokenizeAssign(vector<string> token)
{
	string assignName;
	assignName = token[1];
	assignName = SplitSemi(assignName);
	tokenQueue.push(make_pair(TokenType::Keyword, "assign"));
	tokenQueue.push(make_pair(TokenType::Identifier, assignName));
	tokenQueue.push(make_pair(TokenType::Separator, ";"));
}


void PQLLexer::tokenizeStmt(vector<string> token)
{
	string stmtName;
	stmtName = token[1];
	stmtName = SplitSemi(stmtName);
	tokenQueue.push(make_pair(TokenType::Keyword, "stmt"));
	tokenQueue.push(make_pair(TokenType::Identifier, stmtName));
	tokenQueue.push(make_pair(TokenType::Separator, ";"));
}


void PQLLexer::tokenizeCall(vector<string> token)
{
	string callName;
	callName = token[1];
	callName = SplitSemi(callName);
	tokenQueue.push(make_pair(TokenType::Keyword, "call"));
	tokenQueue.push(make_pair(TokenType::Identifier, callName));
	tokenQueue.push(make_pair(TokenType::Separator, ";"));
}


void PQLLexer::tokenizeConstant(vector<string> token)
{
	string constantName;
	constantName = token[1];
	constantName = SplitSemi(constantName);
	tokenQueue.push(make_pair(TokenType::Keyword, "constant"));
	tokenQueue.push(make_pair(TokenType::Identifier, constantName));
	tokenQueue.push(make_pair(TokenType::Separator, ";"));
}

void PQLLexer::tokenizeSelect(vector<string> token) {
	target = token[1];
}



void PQLLexer::tokenizePattern(vector<string>)
{
}

void PQLLexer::tokenizeFollows(vector<string> token)
{
	selectQueue.push(make_tuple(RelationType::Follows, token[1], token[2]));
}


void PQLLexer::tokenizeFollowsT(vector<string> token)
{
	selectQueue.push(make_tuple(RelationType::FollowsT, token[1], token[2]));
}


void PQLLexer::tokenizeParent(vector<string> token)
{
	selectQueue.push(make_tuple(RelationType::Parent, token[1], token[2]));
}


void PQLLexer::tokenizeParentT(vector<string> token)
{
	selectQueue.push(make_tuple(RelationType::ParentT, token[1], token[2]));
}

void PQLLexer::tokenizeUses(vector<string> token)
{
	if (isInt(token[0]))
	{
		selectQueue.push(make_tuple(RelationType::UsesS, token[1], token[2]));
	}
	else
	{
		selectQueue.push(make_tuple(RelationType::UsesP, token[1], token[2]));
	}
}


void PQLLexer::tokenizeModifies(vector<string> token)
{
	if (isInt(token[0]))
	{
		selectQueue.push(make_tuple(RelationType::ModifiesS, token[1], token[2]));
	}
	else
	{
		selectQueue.push(make_tuple(RelationType::ModifiesP, token[1], token[2]));
	}
}

