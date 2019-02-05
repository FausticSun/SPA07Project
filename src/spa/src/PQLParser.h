#pragma once

#include<string>

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
enum TokenType
{
	identifier,
	keyword,
	separator,
	operator,
	literal
};



class PQLParser {
public:
	PQLParser(string input);
	queue<pair<DeclarationType, string>> getDeclarationQueue();
	queue<pair<RelationType, pair<string, string>>> getSelectQueue();

private:
	queue<pair<DeclarationType, string>> declarationQueue;
	queue<pair<RelationType, pari<string, string>>> selectQueue;
	void Tokenize(string input);
	vector<string> vectorize(string);
	void tokenizeVariable(vector<string>);
	void tokenizeProcedure(vector<string>);
	void tokenizePattern(vector<string>);
	void tokenizeParent(vector<string>);
	void tokenizeParentS(vector<string>);
	void tokenizeFollows(vector<string>);
	void tokenizeFollowsS(vector<string>);
	void tokenizeModifies(vector<string>);
	void tokenizeUses(vector<string>);



};
