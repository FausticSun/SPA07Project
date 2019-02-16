#pragma once

#include<string>
#include<queue>
#include<tuple>
#include<iostream>

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


enum class TokenType {
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
  queue<string> getTarget();

private:
  queue<string> target;
  queue<pair<TokenType, string>> tokenQueue;
  void Tokenize(string input);
  vector<string> vectorize(string);
  string SplitSemi(string);
  string SplitCommas(string);
  bool selectEnd;
  bool tokenContainCommas(string);
  bool tokenContainSemi(string);
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
  vector<string> tokenizeSelect(vector<string>);
  void tokenizePattern(vector<string>);
  vector<string> tokenizeParent(vector<string>);
  vector<string> tokenizeParentT(vector<string>);
  vector<string> tokenizeFollows(vector<string>);
  vector<string> tokenizeFollowsT(vector<string>);
  vector<string> tokenizeModifies(vector<string>);
  vector<string> tokenizeModifiesP(vector<string>);
  vector<string> tokenizeUses(vector<string>);
  vector<string> tokenizeUsesP(vector<string>);
};
