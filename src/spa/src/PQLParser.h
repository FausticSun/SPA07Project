#pragma once

#include "PQLLexer.h"
#include "Query.h"
#include "QueryToken.h"
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
  std::queue<QueryToken> parse(std::string);
  Query buildQuery(std::queue<QueryToken> &tokenQueue);
  Query getQuery();

private:
  std::vector<string> expectedEntityTokens = {
      "variable", "procedure", "if",     "while",    "read",     "print",
      "call",     "stmt",      "assign", "constant", "prog_line"};
  std::vector<string> expectedIndicatorTokens = {"such that", "pattern", "with"};
  std::vector<string> expectedClauseTokens = {
      "Follows", "Follows*", "Parent", "Parent*", "Uses", "Modifies", "Call", "Call*", "Next", "Next*"};
  std::map<std::string, std::vector<QueryEntityType>> validationTable = {
	  {"FPN12", {
	  QueryEntityType::Assign,    QueryEntityType::If,
	  QueryEntityType::While,     QueryEntityType::Call,
	  QueryEntityType::Print,     QueryEntityType::Read,
	  QueryEntityType::Stmt,      QueryEntityType::Line,
	  QueryEntityType::Underscore}},
	  {"U1", {
	  QueryEntityType::Assign, QueryEntityType::If,
	  QueryEntityType::While,  QueryEntityType::Call,
	  QueryEntityType::Print,  QueryEntityType::Stmt,
	  QueryEntityType::Line,   QueryEntityType::Procedure,
	  QueryEntityType::Name}},
	  {"M1", {
	  QueryEntityType::Assign, QueryEntityType::If,
	  QueryEntityType::While,  QueryEntityType::Call,
	  QueryEntityType::Read,  QueryEntityType::Stmt,
	  QueryEntityType::Line,   QueryEntityType::Procedure,
	  QueryEntityType::Name}},
	  {"UMPAT2", {
	  QueryEntityType::Variable, QueryEntityType::Name,
	  QueryEntityType::Underscore}},
	  {"PAT1", {
	  QueryEntityType::Assign, QueryEntityType::If,
	  QueryEntityType::While}},
	  {"PAT3", {
	  QueryEntityType::Expression, QueryEntityType::Underscore}},
	  {"C12", {
	  QueryEntityType::Procedure, QueryEntityType::Name,
	  QueryEntityType::Underscore}},
	  {"W12", {
	  QueryEntityType::Name, QueryEntityType::Line,
	  QueryEntityType::Attrref, QueryEntityType::Progline}}
  };
  Query query;
  std::queue<QueryToken> tokenQueue;
  QueryToken token;
  std::map<std::string, QueryEntityType> entityMaps;
  std::vector<QueryEntity> target;
  std::vector<QueryEntity> selectors;
  std::vector<Clause> clauses;

  void getNextToken();
  void expectToken(std::string);
  void expectTokenIn(std::vector<string>);
  void setQueryTarget();
  void insertTarget();
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
  void checkFPNValidity(QueryEntity, QueryEntity);
  void checkModifiesValidity(QueryEntity, QueryEntity);
  void checkUsesValidity(QueryEntity, QueryEntity);
  void checkWithValidity(QueryEntity, QueryEntity);
  void checkCallsValidity(QueryEntity, QueryEntity);
  void checkPatValidity(QueryEntityType);
  void insertClauseFollows();
  void insertClauseFollowsT();
  void insertClauseParent();
  void insertClauseParentT();
  void insertClauseModifiesS();
  void insertClauseUseS();
  void insertClauseNext();
  void insertClauseNextT();
  void insertClauseCalls();
  void insertClauseCallsT();

  string convertToPostfix(string s);
  void insertClausePattern();
  void insertAssignPattern(string);
  void insertIfPattern(string);
  void insertWhilePattern(string);
  QueryEntity parseExpression();
  Query constructQuery();
  bool isInt(string s);
  bool isDefined(string s);
  string checkNameValidity(string s);

  QueryEntity determineWithClauseEntity();
  void insertClauseWith();
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
