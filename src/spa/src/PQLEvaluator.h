#pragma once

#include "Query.h"
#include <PKB.h>
#include <Optimizer.h>
#include <list>
#include <string>
#include <vector>
#include <unordered_map>
#include <Table.h>
#include <Deque>

using namespace std;

typedef set<vector<string>> dataRows;

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
  PqlEvaluator(PKB &pkb);
  deque<string> executeQuery(Query &q);

private:
	Table NextTTable=Table(0);
	Table AffectTable= Table(0);
	Table AffectTTable= Table(0);
	vector<vector<Table>> relevantGroups;
	vector<vector<Table>> inrelevantGroups;
  PKB& mypkb;
	dataRows resultExtractor(Table result, Query q);
	deque<string> resultFormater(dataRows t);
	dataRows executeSimpleQuery(vector<QueryEntity> t);
	dataRows executeComplexQuery(Query q);
	ClauseResult executeOneClause(Clause c);
	void divideGroups(vector<Table>, vector<QueryEntity> targets);
  Table getdataByTtype(QueryEntity q);
  Table getdataWith(QueryEntity q);
	Table validateResult(Table t, vector<QueryEntity> target);
  ClauseResult dataFilter(Table data, Clause c);
  ClauseResult withEvaluate(Clause c);
	ClauseResult NextTEvaluate(Clause clause);
	ClauseResult AffectEvaluate(Clause clause);
	ClauseResult AffectsTEvaluate(Clause clause);
  StatementType convertQType(QueryEntityType q);
	string convertClauseTypeToString(ClauseType);
	Table targetsToTable(vector<QueryEntity> qe);
  bool isSynonym(QueryEntityType q);
  bool isUnderscore(QueryEntityType q);
  bool isConstant(QueryEntityType q);
  bool isAttr(QueryEntityType q);
};
