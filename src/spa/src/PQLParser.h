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
};*/

enum class RelationType
{
	Follows,
	FollowsT,
	Parent,
	ParentT,
	UsesS,
	UsesP,
	ModifiesS,
	ModifiesP
};

enum class DeclarationType
{
	Variable,
	Procedure,
	Read,
	Print,
	While,
	If,
	Stmt,
	Assign,
	Call,
	Constant
};

/*
enum class DeclarationType
{
	Identifier,
	Keyword,
	Separator,
	Operator,
	Literal,
};
*/

class PQLParser {
public:
	PQLParser(string input);
	queue<pair<DeclarationType, string>> getDeclarationQueue();
	queue<tuple<RelationType, string, string>> getSelectQueue();
	string getTarget();

private:
	string target;
	queue<pair<DeclarationType, string>> declarationQueue;
	queue<tuple<RelationType, string, string>> selectQueue;
	void Tokenize(string input);
	vector<string> vectorize(string);
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
