#pragma once

#include<string>
#include<queue>
#include<tuple>

using namespace std;

/*DeclarationType {
        Variable,
        Procedure

};*/

/*{
        Follows,
        Follows*,
        Parent,
        Parent*,
        UsesS,
        UsesP,
        ModifiesS,
        ModifiesP
};

enum class TokenType
{
	Follows,
	FollowsT,
	Parent,
	ParentT,
	UsesS,
	UsesP,
	ModifiesS,
	ModifiesP,
	Variable,
	Procedure,
	Read,
	Print,
	While,
	If,
	Stmt,
	Assign,
	Call,
	Constant,
        Separator
};*/


enum class TokenType
{
	Identifier,
	Keyword,
	Separator,
	Operator,
	Literal,
};


class PQLLexer {
public:
	PQLLexer(string input);
	queue<pair<TokenType, string>> getTokenQueue();
	string getTarget();

private:
	string target;
	queue<pair<TokenType, string>> tokenQueue;
	void Tokenize(string input);
	vector<string> vectorize(string);
	string SplitSemi(string);
	void tokenizeVariable(vector<string>);
	void tokenizeCall(vector<string>);
	void tokenizeAssign(vector<string>);
	void tokenizeConstant(vector<string>);
	void tokenizeWhile(vector<string>);
	void tokenizePrint(vector<string>);
	void tokenizeRead(vector<string>);
	void tokenizeStmt(vector<string>);
	void tokenizeIf(vector<string>);
        
	void tokenizeProcedure(vector<string>);
	void tokenizeSelect(vector<string>);
	void tokenizePattern(vector<string>);
	void tokenizeParent(vector<string>);
	void tokenizeParentT(vector<string>);
	void tokenizeFollows(vector<string>);
	void tokenizeFollowsT(vector<string>);
	void tokenizeModifies(vector<string>);
	void tokenizeModifiesP(vector<string>);
	void tokenizeUses(vector<string>);
	void tokenizeUsesP(vector<string>);
};
