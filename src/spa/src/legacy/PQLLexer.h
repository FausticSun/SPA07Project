#pragma once

#include "Token.h"
#include <queue>
#include <string>
#include <tuple>

using namespace std;
using namespace PQLLexerToken;

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
  string SplitCommas(string);
  bool selectEnd;
  void expectionOfSelect(vector<string>);
  void expectionOfDeclaration(vector<string>);
  bool tokenContainCommas(string);
  bool tokenContainSemi(string);
  bool existSemi(string);
  vector<string> tokenizeVariable(vector<string>);
  vector<string> tokenizeAssign(vector<string>);
  vector<string> tokenizeConstant(vector<string>);
  vector<string> tokenizeWhile(vector<string>);
  vector<string> tokenizePrint(vector<string>);
  vector<string> tokenizeRead(vector<string>);
  vector<string> tokenizeProgLine(vector<string>);
  vector<string> tokenizeStmt(vector<string>);
  vector<string> tokenizeIf(vector<string>);
  vector<string> tokenizeCall(vector<string>);
  vector<string> tokenizeProcedure(vector<string>);
  vector<string> tokenizeSelect(vector<string>);
  vector<string> tokenizePattern(vector<string>);
  vector<string> tokenizeWith(vector<string>);
  vector<string> tokenizeParent(vector<string>);
  vector<string> tokenizeParentT(vector<string>);
  vector<string> tokenizeCalls(vector<string>);
  vector<string> tokenizeCallsT(vector<string>);
  vector<string> tokenizeNext(vector<string>);
  vector<string> tokenizeNextT(vector<string>);
  vector<string> tokenizeFollows(vector<string>);
  vector<string> tokenizeFollowsT(vector<string>);
  vector<string> tokenizeModifies(vector<string>);
  vector<string> tokenizeModifiesP(vector<string>);
  vector<string> tokenizeUses(vector<string>);
  vector<string> tokenizeUsesP(vector<string>);
};
