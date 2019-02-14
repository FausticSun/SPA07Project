#pragma once

#include <PKB.h>
#include <Query.h>
#include <iostream>
#include <list>
#include <string>
#include <vector>
#include "Query.h"

using namespace std;

class ClauseResult
{
public:
	ClauseResult(bool isbool, bool bvalue, vector<QueryEntity> titles = {}, vector<vector<string>> resultTable={})
          :isBool(isbool),
          bValue(bvalue),
          titles(titles),
          resultTable(resultTable){
	  };
	bool isBool;
	bool bValue;
	vector<QueryEntity> titles;
	vector<vector<string>> resultTable;
};


class PqlEvaluator {
public:
  PqlEvaluator(const PKB& pkb);
  ~PqlEvaluator();
  list<string> evaluateQuery(string query);
  
private:
  PKB mypkb;
  list<string> executeQuery(vector<Clause> &clauses);
  ClauseResult getModifies(Clause c);
  ClauseResult getUses(Clause c);
  ClauseResult getParent(Clause c);
  ClauseResult getParentS(Clause c);
  ClauseResult getFollows(Clause c);
  ClauseResult getFollowsS(Clause c);
  ClauseResult getAssPatern(Clause c);
  bool isSynonym(QueryEntityType q);
  bool isUnderscore(QueryEntityType q);
  bool isConstant(QueryEntityType q);
  bool validateType(string result, QueryEntityType q);
  bool validateStmt(string result, QueryEntityType q);
  bool isVar(string result, QueryEntityType q);
  bool isPro(string result, QueryEntityType q);
  bool isCons(string result, QueryEntityType q);
  StatementType convertQType(QueryEntityType q);

  /*map<string, QueryEntityType> executeDeclaration(const vector<QueryEntity> &selectors);
  vector<ClauseResult> excuteClauses(const vector<Clause> &clauses);
  int analyseClauseConstants(const Clause &clause);*/

  
};