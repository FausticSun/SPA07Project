#pragma once

#include "Query.h"
#include "Token.h"
#include <map>
#include <queue>
#include <string>

using namespace std;

/*
enum class DeclarationType
{
                Identifier,
                Keyword,
                Separator,
                Operator,
                Literal,
};

enum class DeclarationType {
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
*/

class PQLParser {
public:
  PQLParser();
  std::queue<Token> parse(std::string);
  Query buildQuery(std::queue<Token> &tokenQueue);
  Query getQuery();

private:
  std::vector<string> expectedEntityTokens = {
      "variable", "procedure", "if",     "while",    "read",     "print",
      "call",     "stmt",      "assign", "constant", "prog_line"};
  std::vector<string> expectedIndicatorTokens = {"such that", "pattern"};
  std::vector<string> expectedClauseTokens = {
      "Follows", "Follows*", "Parent", "Parent*", "Uses", "Modifies"};
  Query query;
  std::queue<Token> tokenQueue;
  Token token;
  std::map<std::string, QueryEntityType> entityMaps;
  QueryEntity target;
  std::vector<QueryEntity> selectors;
  std::vector<Clause> clauses;

  void getNextToken();
  void expectToken(std::string);
  void expectTokenIn(std::vector<string>);
  void setQueryTarget();
  void tokenizeSelect();

  void insertQueryEntityVariable();
  void insertQueryEntityProcedure();
  void insertQueryEntityStmt();
  void insertQueryEntityRead();
  void insertQueryEntityPrint();
  void insertQueryEntityCall();
  void insertQueryEntityAssign();
  void insertQueryEntityIf();
  void insertQueryEntityWhile();
  void insertQueryEntityConstant();
  void insertQueryEntityProgline();

  QueryEntity determineQueryEntity();
  void insertClauseFollows();
  void insertClauseFollowsT();
  void insertClauseParent();
  void insertClauseParentT();
  void insertClauseModifiesS();
  void insertClauseUseS();

  Query constructQuery();

  /*void Tokenize(string input);
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
  void tokenizeUsesP(vector<string>);*/
};
