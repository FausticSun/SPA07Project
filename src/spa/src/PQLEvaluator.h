#pragma once

#include "Query.h"
#include <PKB.h>
#include <list>
#include <string>
#include <vector>
#include <Table.h>

using namespace std;

//class ClauseResult {
//public:
//  ClauseResult(bool isbool, bool bvalue, vector<QueryEntity> titles = {},
//               vector<vector<string>> resultTable = {})
//      : isBool(isbool), bValue(bvalue), titles(titles),
//        resultTable(resultTable){};
//  bool isBool;
//  bool bValue;
//  vector<QueryEntity> titles;
//  vector<vector<string>> resultTable;
//  void normalize();
//  bool contains(QueryEntity &q);
//};

struct ClauseResult {
	bool isBool;
	bool boolValue;
	Table data = Table(0);

	ClauseResult(bool iB, bool bValue) {
		isBool = iB;
		boolValue = bValue;
	};
};

class PqlEvaluator {
public:
  PqlEvaluator(const PKB &pkb);
  list<string> evaluateQuery(string query);
  list<string> executeQuery(Query &q);

private:
  PKB mypkb;
  Table executeSimpleQuery(vector<QueryEntity> t);
	Table executeComplexQuery(Query q);
  Table getdataByTtype(QueryEntity q);
  ClauseResult dataFilter(Table data, Clause c);
  Table resultExtractor(Table result, Query q);
  list<string> resultFormater(Table t);
  bool isSynonym(QueryEntityType q);
  bool isUnderscore(QueryEntityType q);
  bool isConstant(QueryEntityType q);
  bool isVar(string result, QueryEntityType q);
  bool isPro(string result, QueryEntityType q);

  StatementType convertQType(QueryEntityType q);

  //ClauseResult getModifies(Clause c);
  //ClauseResult getUses(Clause c);
  //ClauseResult getParent(Clause c);
  //ClauseResult getParentS(Clause c);
  //ClauseResult getFollows(Clause c);
  //ClauseResult getFollowsS(Clause c);
  //ClauseResult getAssPatern(Clause c);

  //bool validateType(string result, QueryEntityType q);
  //bool validateStmt(string result, QueryEntityType q);
  //bool isCons(string result, QueryEntityType q);
};
