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
	while (0 < token.size()) {
		if (token[0] == "procedure") {
			tokenizeProcedure(token);
			token.erase(token.begin());
			token.erase(token.begin());
		}


		if (find(token.begin(), token.end(), "variable") != token.end()) {
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
	string variableName;
	variableName = token[1];
	variableName = SplitSemi(variableName);
	tokenQueue.push(make_pair(TokenType::Variable, variableName));
	tokenQueue.push(make_pair(TokenType::Separator, ";"));
}


void PQLLexer::tokenizeProcedure(vector<string> token)
{
	string procedureName;
	procedureName = token[1];
	procedureName = SplitSemi(procedureName);
	tokenQueue.push(make_pair(TokenType::Procedure, procedureName));
	tokenQueue.push(make_pair(TokenType::Separator, ";"));
}


void PQLLexer::tokenizeRead(vector<string> token)
{
	declarationQueue.push(make_pair(DeclarationType::Read, token[1]));
}


void PQLLexer::tokenizePrint(vector<string> token)
{
	declarationQueue.push(make_pair(DeclarationType::Print, token[1]));
}


void PQLLexer::tokenizeWhile(vector<string> token)
{
	declarationQueue.push(make_pair(DeclarationType::While, token[1]));
}


void PQLLexer::tokenizeIf(vector<string> token)
{
	declarationQueue.push(make_pair(DeclarationType::If, token[1]));
}


void PQLLexer::tokenizeAssign(vector<string> token)
{
	declarationQueue.push(make_pair(DeclarationType::Assign, token[1]));
}


void PQLLexer::tokenizeStmt(vector<string> token)
{
	declarationQueue.push(make_pair(DeclarationType::Stmt, token[1]));
}


void PQLLexer::tokenizeCall(vector<string> token)
{
	declarationQueue.push(make_pair(DeclarationType::Call, token[1]));
}


void PQLLexer::tokenizeConstant(vector<string> token)
{
	declarationQueue.push(make_pair(DeclarationType::Constant, token[1]));
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

