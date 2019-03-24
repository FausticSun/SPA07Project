#pragma once

#include "Query.h"
#include <PKB.h>
#include <list>
#include <string>
#include <vector>
#include <Table.h>

using namespace std;

struct ClauseResult {
  bool isBool;
  bool boolValue;
  Table data = Table(0);
  ClauseResult() {
		isBool = false;
		boolValue = false;
	};

  ClauseResult(bool iB, bool bValue) {
    isBool = iB;
    boolValue = bValue;
  };
};

class PqlEvaluator {
public:
  PqlEvaluator(const PKB &pkb);
  list<string> executeQuery(Query &q);

private:
  PKB mypkb;
	set<vector<string>> resultExtractor(Table result, Query q);
  list<string> resultFormater(set<vector<string>> t);
	set<vector<string>> executeSimpleQuery(vector<QueryEntity> t);
	set<vector<string>> executeComplexQuery(Query q);
  Table getdataByTtype(QueryEntity q);
  Table getdataWith(QueryEntity q);
  ClauseResult dataFilter(Table data, Clause c);
  ClauseResult withEvaluate(Clause c);
  StatementType convertQType(QueryEntityType q);
  bool isSynonym(QueryEntityType q);
  bool isUnderscore(QueryEntityType q);
  bool isConstant(QueryEntityType q);
  bool isAttr(QueryEntityType q);
};
