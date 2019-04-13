#pragma once

#include "Query.h"
#include <Deque>
#include <Optimizer.h>
#include <PKB.h>
#include <Table.h>
#include <list>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

typedef set<vector<string>> dataRows;

struct ClauseResult {
  // whether the clause is boolean type
  bool isBool;
  // if the clause is boolean type, is true or false
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
  void executeQuery(Query &q, std::list<std::string> &results);

private:
  // store results that are divided into selected and not selected
  vector<vector<Table>> relevantGroups;
  vector<vector<Table>> inrelevantGroups;
  // pkb instance
  PKB &mypkb;
  // extracte results from merged results
  dataRows resultExtractor(Table result, Query q);
  // format results into list of string
  void resultFormater(dataRows &t, std::list<std::string> &results);
  // evaluate query withoud clause
  dataRows executeSimpleQuery(vector<QueryEntity> t);
  // evaluate query with clauses
  dataRows executeComplexQuery(Query q);
  // evaluate a single clause, return result as ClauseResult
  ClauseResult executeOneClause(Clause c);
  // divide results into selected and not selected
  void divideGroups(vector<Table>, vector<QueryEntity> targets);
  // get data from pkb according to requested entity type:stmt, assign, while,
  // if...
  Table getdataByTtype(QueryEntity q);
  // get data from pkb that requested entity type is AttrRef: s.stmt#,prog_line
  Table getdataWith(QueryEntity q);
  // accept empty result table, if select boolean, return flase;else,return none
  Table validateResult(Table t, vector<QueryEntity> target);
  // filter result get from pkb according to type of parameters in clause
  ClauseResult dataFilter(Table data, Clause c);
	//projectOut unnecessary columns
	Table projectOut(Table t, vector<QueryEntity> targets);
  // evaluate with clauses
  ClauseResult withEvaluate(Clause c);
  // evaluate NextT,Affects,AffectsT,NextBip,NextBipT,AffectsBip,AffectsBipT
  // clause
  ClauseResult NextTEvaluate(Clause clause);
  ClauseResult AffectEvaluate(Clause clause);
  ClauseResult AffectsTEvaluate(Clause clause);
  ClauseResult NextBipEvaluate(Clause clause);
  ClauseResult NextBipTEvaluate(Clause clause);
  ClauseResult AffectsBipEvaluate(Clause clause);
  ClauseResult AffectsBipTEvaluate(Clause clause);
  // convert query entity type to corresbonding statement type,which are
  // accepted by pkb to retrieve data
  StatementType convertQType(QueryEntityType q);
  // convert clausetype to readable string
  string convertClauseTypeToString(ClauseType);
  // convert selected tuple to table form
  Table targetsToTable(vector<QueryEntity> qe);
  // classify the query entity
  bool isSynonym(QueryEntityType q);
  bool isUnderscore(QueryEntityType q);
  bool isConstant(QueryEntityType q);
  bool isAttr(QueryEntityType q);
};
