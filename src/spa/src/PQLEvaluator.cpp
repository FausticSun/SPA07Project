#include "PQLParser.h"

#include <PQLEvaluator.h>
#include <iostream>
#include <sstream>
// split AttrRef  by '.'
vector<string> split(const string &s, const char delim = '.') {
  vector<string> sv;
  sv.clear();
  istringstream iss(s);
  string temp;

  while (getline(iss, temp, delim)) {
    sv.emplace_back(move(temp));
  }

  return sv;
}
// judge whether the a string is a partial expression
bool isPartial(string s) {
  char ending = '_';
  if (s.size() > 2 && s[0] == ending && s[s.size() - 1] == ending) {
    return true;
  }
  return false;
}
// remove underscore of partial expression
string removeUnderscore(string s) { return s.substr(1, s.size() - 2); }
// take in a one column table, double the column size, copy the 1st column to
// 2nd column
Table duplicateCols(string s, Table t) {
  if (t.getHeader().size() == 1) {
    dataRows rows = t.getData();
    Table result(2);
    result.setHeader({t.getHeader()[0], s});
    for (vector<string> row : rows) {
      row.push_back(row[0]);
      result.insertRow(row);
    }
    return result;
  } else {
    return Table(0);
  }
}
// convert dataRows to a complete TRable with Header
Table rowsToTable(dataRows rows, vector<string> header) {
  Table result(header.size());
  result.setHeader(header);
  for (vector<string> row : rows) {
    if (row.size() == header.size()) {
      result.insertRow(row);
    } else {
      return Table(0);
    }
  }
  return result;
}
// take in a two-column table, join the 1st column with the 2nd column
Table selfJoin(Table t) {
  if (t.getHeader().size() == 2) {
    Table result(2);
    result.setHeader(t.getHeader());
    for (vector<string> row : t.getData()) {
      if (row[0] == row[1]) {
        result.insertRow(row);
      }
    }
    return result;
  } else {
    return Table(0);
  }
}
// check whether two tables have joined attributes
bool isJoined(Table &s1, Table &s2) {
  vector<string> header1 = s1.getHeader();
  vector<string> header2 = s2.getHeader();
  for (int i = 0; i < header1.size(); ++i) {
    for (int j = 0; j < header2.size(); ++j) {
      if (header1[i] == header2[j]) {
        return true;
      }
    }
  }
  return false;
}
// check whether a table and a group of tables have joined columns
bool isJoined(vector<Table> &s1, Table &s2) {
  for (Table &t : s1) {
    if (isJoined(t, s2)) {
      return true;
    }
  }
  return false;
}

PqlEvaluator::PqlEvaluator(PKB &pkb) : mypkb(pkb){};
// start execute query, judge whether query is simple or complex, use different
// way to evaluate
void PqlEvaluator::executeQuery(Query &q, std::list<std::string> &results) {
  dataRows resultTable;
  if (q.clauses.empty()) {
    resultTable = executeSimpleQuery(q.target);
  } else {
    resultTable = executeComplexQuery(q);
  }
  // already get all results, format results into list of strings
  resultFormater(resultTable, results);
}
// take in tables merged from result table of each clause, according to selected
// tuple extract the data we want
dataRows PqlEvaluator::resultExtractor(Table result, Query q) {
  vector<string> s;
  vector<QueryEntity> attr;
  vector<Table> tables;
  Table tempTable(0);
  // when the selected is BOOLEAN
  if (q.target.front() == QueryEntityType::Boolean) {
    if (result.size() > 0) {
      Table t(1);
      t.insertRow({"TRUE"});
      return t.getData();
    }
    Table t(1);
    t.insertRow({"FALSE"});
    return t.getData();
  }
  // when selected is tuple
  if (result.empty()) {
    dataRows t;
    return t;
  } else {
    vector<string> header = result.getHeader();
    for (QueryEntity qe : q.target) {
      header = result.getHeader();
      if (isAttr(qe.type)) {
        vector<string> temp = split(qe.name, '.');
        if (find(header.begin(), header.end(), temp[0]) != header.end()) {
          Table t = getdataWith(qe);
          tables.push_back(t);
          result.mergeWith(t);
        } else {
          Table t = getdataWith(qe);
          t.dropColumn(t.getHeader()[0]);
          tables.push_back(t);
          result.mergeWith(t);
        }
      } else if (isSynonym(qe.type)) {
        if (find(header.begin(), header.end(), qe.name) != header.end()) {
        } else {
          Table t = getdataByTtype(qe.type);
          t.setHeader({qe.name});
          tables.push_back(t);
          result.mergeWith(t);
        }
      }
      s.push_back(qe.name);
    }
  }
  dataRows resultTable = result.getData(s);

  return resultTable;
}
// format results into list of string
void PqlEvaluator::resultFormater(dataRows &t,
                                  std::list<std::string> &results) {
  if (t.empty()) {
    return;
  }
  results.assign(t.size(), "");
  auto resultsIt = results.begin();
  auto dataIt = t.begin();
  while (dataIt != t.end()) {
    auto it = dataIt->begin();
    resultsIt->append(it->c_str());
    it++;
    for (; it != dataIt->end(); ++it) {
      resultsIt->append(" ");
      resultsIt->append(it->c_str());
    }
    dataIt++;
    resultsIt++;
  }
}
// execute queries without clauses
set<vector<string>> PqlEvaluator::executeSimpleQuery(vector<QueryEntity> t) {
  int count = 0;
  vector<string> s;
  vector<Table> tables;
  for (QueryEntity q : t) {
    if (q.type == QueryEntityType::Boolean) {
      Table result(1);
      result.insertRow({"TRUE"});
      return result.getData();
    }
    if (isSynonym(q.type)) {
      Table t = getdataByTtype(q.type);
      t.setHeader({q.name});
      tables.push_back(t);
    } else if (isConstant(q.type)) {
      Table t(1);
      t.insertRow({q.name});
      t.setHeader({to_string(count)});
      tables.push_back(t);
      count++;
    } else if (isAttr(q.type)) {
      Table t = getdataWith(q);
      if (t.getHeader().size() == 2) {
        t.dropColumn(t.getHeader()[0]);
      }

      tables.push_back(t);
    }
    s.push_back(q.name);
  }
  Table result = tables[0];
  for (int i = 1; i < tables.size(); i++) {
    result.mergeWith(tables[i]);
  }
  return result.getData(s);
}
// execute clauses with clause
set<vector<string>> PqlEvaluator::executeComplexQuery(Query q) {
  vector<Clause> clauses = q.clauses;
  vector<Table> tables;
  vector<Clause>::iterator iter = clauses.begin();
  Table data(0);
  ClauseResult result;
  for (iter; iter != clauses.end(); ++iter) {
    // get result by evaluating each clause
    result = executeOneClause(*iter);
    // when the result is from boolean type clause and is false
    if (result.isBool && !result.boolValue) {

      if (q.target.front().type == QueryEntityType::Boolean) {
        Table resultTable(1);
        resultTable.insertRow({"FALSE"});
        return resultTable.getData();
      }
      Table resultTable(0);
      return resultTable.getData();
    }
    // when the result is from boolean type clause and is true, ignore it
    if (result.isBool && result.boolValue) {

    } else {
      tables.push_back(result.data);
    }
  }
  // divided result tables into connected groups, which are divided to groups
  // having selected attributes or not
  divideGroups(tables, q.target);
  vector<Table> relevantResults;
  for (vector<Table> t : relevantGroups) {
    Optimizer optimizer(t);
    Table groupResult = optimizer.getResult();
    if (groupResult.empty()) {
      return validateResult(groupResult, q.target).getData();
    }
    groupResult = projectOut(groupResult, q.target);
    relevantResults.push_back(groupResult);
  }
  // join the groups that are relevant to the selected tuplle
  vector<Table> inrelevantResults;
  for (vector<Table> t : inrelevantGroups) {
    Optimizer optimizer(t);
    Table groupResult = optimizer.getResult();
    if (groupResult.empty()) {
      return validateResult(groupResult, q.target).getData();
    }
  }

  if (!relevantResults.empty()) {
    // optimization
    for (int i = 1; i < relevantResults.size(); i++) {
      relevantResults[0].mergeWith(relevantResults[i]);
    }
    dataRows complexresult = resultExtractor(relevantResults[0], q);
    return complexresult;
  }
  dataRows simpleResult = executeSimpleQuery(q.target);
  return simpleResult;
}
// evaluate each single clause
ClauseResult PqlEvaluator::executeOneClause(Clause c) {
  Table data(0);
  ClauseResult result;
  if (c.clauseType == ClauseType::ModifiesS) {
    // identity the first parameter is procedure or statement
    if ((c.parameters.front().type == QueryEntityType::Procedure ||
         c.parameters.front().type == QueryEntityType::Name)) {
      data = mypkb.getModifiesP();
    } else {
      data = mypkb.getModifiesS();
    }
    result = dataFilter(data, c);
  } else if (c.clauseType == ClauseType::UsesS) {
    // identity the first parameter is procedure or statement
    if ((c.parameters.front().type == QueryEntityType::Procedure ||
         c.parameters.front().type == QueryEntityType::Name)) {
      data = mypkb.getUsesP();
    } else {
      data = mypkb.getUsesS();
    }
    result = dataFilter(data, c);

  } else if (c.clauseType == ClauseType::Parent) {
    data = mypkb.getParent();
    result = dataFilter(data, c);
  } else if (c.clauseType == ClauseType::ParentT) {
    data = mypkb.getParentT();
    result = dataFilter(data, c);
  } else if (c.clauseType == ClauseType::Follows) {
    data = mypkb.getFollows();
    result = dataFilter(data, c);
  } else if (c.clauseType == ClauseType::FollowsT) {
    data = mypkb.getFollowsT();
    result = dataFilter(data, c);
  } else if (c.clauseType == ClauseType::AssignPatt) {
    // classify AssignPatt into Expression or Underscore to evaluate
    if (isConstant(c.parameters[2].type)) {
      if (isPartial(c.parameters[2].name)) {
        data = mypkb.getAssignMatches(removeUnderscore(c.parameters[2].name),
                                      true);
      } else {
        data = mypkb.getAssignMatches((c.parameters[2].name), false);
      }
    } else {
      data = mypkb.getAssignMatches("", true);
    }
    result = dataFilter(data, c);
  } else if (c.clauseType == ClauseType::WhilePatt) {
    data = mypkb.getWhileMatches();
    result = dataFilter(data, c);
  } else if (c.clauseType == ClauseType::IfPatt) {
    data = mypkb.getIfMatches();
    result = dataFilter(data, c);
  } else if (c.clauseType == ClauseType::With) {
    result = withEvaluate(c);
  } else if (c.clauseType == ClauseType::Next) {
    data = mypkb.getNext();
    result = dataFilter(data, c);
  } else if (c.clauseType == ClauseType::Calls) {
    data = mypkb.getCalls();
    result = dataFilter(data, c);
  } else if (c.clauseType == ClauseType::CallsT) {
    data = mypkb.getCallsT();
    result = dataFilter(data, c);
  } else if (c.clauseType == ClauseType::NextT) {
    result = NextTEvaluate(c);
  } else if (c.clauseType == ClauseType::Affects) {
    result = AffectEvaluate(c);
  } else if (c.clauseType == ClauseType::AffectsT) {
    result = AffectsTEvaluate(c);
  } else if (c.clauseType == ClauseType::AffectsBip) {
    result = AffectsBipEvaluate(c);
  } else if (c.clauseType == ClauseType::AffectsBipT) {
    result = AffectsBipTEvaluate(c);
  } else if (c.clauseType == ClauseType::NextBip) {
    result = NextBipEvaluate(c);
  } else if (c.clauseType == ClauseType::NextBipT) {
    result = NextBipTEvaluate(c);
  }

  return result;
}
// divide results into selected and not selected
void PqlEvaluator::divideGroups(vector<Table> tables,
                                vector<QueryEntity> targets) {
  bool hasJoin = false;
  bool end = true;
  Table target = targetsToTable(targets);
  vector<Table> copies(tables.begin(), tables.end());
  while (!copies.empty()) {
    vector<Table> present;
    while (end) {
      end = false;
      vector<Table>::iterator iter = copies.begin();
      while (iter != copies.end()) {
        if (present.empty()) {
          hasJoin = isJoined(*iter, target) ||
                    targets.front().type == QueryEntityType::Boolean;
          present.push_back(*iter);
          iter = copies.erase(iter);
          end = true;
        } else if (isJoined(present, *iter)) {
          hasJoin = isJoined(*iter, target) || isJoined(present, target) ||
                    targets.front().type == QueryEntityType::Boolean;
          present.push_back(*iter);
          iter = copies.erase(iter);
          end = true;
        } else {
          iter++;
        }
      }
    }
    if (hasJoin) {
      relevantGroups.push_back(present);
    } else {
      inrelevantGroups.push_back(present);
    }
    hasJoin = false;
    end = true;
  }
}
// filter result get from pkb according to type of parameters in clause
ClauseResult PqlEvaluator::dataFilter(Table data, Clause c) {
  QueryEntity qe1 = c.parameters[0];
  QueryEntity qe2 = c.parameters[1];
  vector<Table> columns;

  data.setHeader({"1", "2"});
  if (!isUnderscore(qe1.type)) {
    data.filterColumn("1", getdataByTtype(qe1).getColumn("0"));
  }
  if (isSynonym(qe1.type) && isSynonym(qe2.type) && qe1.name == qe2.name) {
    if ((c.clauseType == ClauseType::Calls ||
         c.clauseType == ClauseType::CallsT) &&
        qe1.type == qe2.type && qe2.type == QueryEntityType::Procedure) {
      data = Table(1);
    } else {
      data.selfJoin();
    }
  } else if (!isUnderscore(qe2.type)) {
    data.filterColumn("2", getdataByTtype(qe2).getColumn("0"));
  }

  if (data.empty()) {
    ClauseResult result(true, false);
    return result;
  }

  if (isSynonym(qe1.type) || isSynonym(qe2.type)) {
    if (!isSynonym(qe1.type)) {
      data.dropColumn("1");
    }

    if (!isSynonym(qe2.type)) {
      data.dropColumn("2");
    }
    for (string title : data.getHeader()) {
      if (title == "1") {
        data.modifyHeader("1", qe1.name);
      } else if (title == "2") {
        data.modifyHeader("2", qe2.name);
      }
    }
    ClauseResult result(false, false);
    result.data = data;
    return result;
  }
  return ClauseResult(true, true);
}

Table PqlEvaluator::projectOut(Table t, vector<QueryEntity> targets) {
  vector<string> headers;
  vector<string> tars = targetsToTable(targets).getHeader();
  set<string> tarset(tars.begin(), tars.end());
  for (string s : t.getHeader()) {
    if (!tarset.count(s)) {
      t.dropColumn(s);
    }
  }
  return t;
}

// evaluate with clause
ClauseResult PqlEvaluator::withEvaluate(Clause c) {
  vector<Table> tables;
  vector<pair<string, string>> expectation;
  for (QueryEntity q : c.parameters) {
    if (isConstant(q.type)) {
      Table t(1);
      t.insertRow({q.name});
      t.setHeader({c.parameters[0].name + "=" + c.parameters[0].name});
      tables.push_back(t);
    } else if (isSynonym(q.type)) {
      Table t = getdataByTtype(q.type);
      t.setHeader({c.parameters[0].name + "=" + c.parameters[0].name});
      tables.push_back(t);
      expectation.push_back(make_pair(t.getHeader()[0], q.name));
    } else if (isAttr(q.type)) {
      Table t = getdataWith(q);
      t.modifyHeader(q.name, c.parameters[0].name + "=" + c.parameters[0].name);
      tables.push_back(t);
      vector<string> temp = split(q.name, '.');
      expectation.push_back(make_pair(t.getHeader()[0], temp[0]));
    }
  }
  tables[0].mergeWith(tables[1]);
  if (tables[0].empty()) {
    return ClauseResult(true, false);
  }
  if (expectation.empty()) {
    return ClauseResult(true, true);
  }
  dataRows data;
  Table t(expectation.size());
  if (expectation.size() == 1) {
    data = tables[0].getData({expectation[0].first});
    t.setHeader({expectation[0].second});
    t = rowsToTable(data, {expectation[0].second});
  } else if (expectation.size() == 2 &&
             expectation[0].second != expectation[1].second) {
    data = tables[0].getData({expectation[0].first, expectation[1].first});
    t.setHeader({expectation[0].second, expectation[1].second});
    t = rowsToTable(data, {expectation[0].second, expectation[1].second});
  } else if (expectation.size() == 2 &&
             expectation[0].second == expectation[1].second) {
    data = tables[0].getData({expectation[0].first});
    t.setHeader({expectation[0].second});
    t = rowsToTable(data, {expectation[0].second});
  }
  ClauseResult result(false, true);
  result.data = t;
  return result;
}
// evaluate NextT,Affects,AffectsT,NextBip,NextBipT,AffectsBip,AffectsBipT
// clause
ClauseResult PqlEvaluator::NextTEvaluate(Clause c) {
  QueryEntity qe1 = c.parameters[0];
  QueryEntity qe2 = c.parameters[1];
  Table data(2);
  ClauseResult result;
  if (isConstant(qe1.type)) {
    Table col1(1);
    col1.setHeader({"1"});
    col1.insertRow({qe1.name});
    Table pkbData = mypkb.getNextT(stoi(qe1.name), true);
    pkbData.setHeader({"2"});
    col1.mergeWith(pkbData);
    result = dataFilter(col1, c);
    return result;
  } else if (isConstant(qe2.type)) {
    Table col2(1);
    col2.setHeader({"2"});
    col2.insertRow({qe2.name});
    Table pkbData = mypkb.getNextT(stoi(qe2.name), false);
    pkbData.setHeader({"1"});
    pkbData.mergeWith(col2);
    result = dataFilter(pkbData, c);
    return result;
  } else {
    Table pkbData(0);
    pkbData = mypkb.getNextT();
    result = dataFilter(pkbData, c);
  }
  return result;
}
ClauseResult PqlEvaluator::AffectEvaluate(Clause c) {
  QueryEntity qe1 = c.parameters[0];
  QueryEntity qe2 = c.parameters[1];
  ClauseResult result;
  if (isConstant(qe1.type)) {
    Table col1(1);
    col1.setHeader({"1"});
    col1.insertRow({qe1.name});
    Table pkbData = mypkb.getAffects(stoi(qe1.name), true);
    pkbData.setHeader({"2"});
    col1.mergeWith(pkbData);
    result = dataFilter(col1, c);
  } else if (isConstant(qe2.type)) {
    Table col2(1);
    col2.setHeader({"2"});
    col2.insertRow({qe2.name});
    Table pkbData = mypkb.getAffects(stoi(qe2.name), false);
    pkbData.setHeader({"1"});
    pkbData.mergeWith(col2);
    result = dataFilter(pkbData, c);
  } else {
    Table pkbData(0);
    pkbData = mypkb.getAffects();
    result = dataFilter(pkbData, c);
  }
  return result;
}
ClauseResult PqlEvaluator::AffectsTEvaluate(Clause c) {
  Table pkbData(0);
  ClauseResult result;
  pkbData = mypkb.getAffectsT();
  result = dataFilter(pkbData, c);
  return result;
}
ClauseResult PqlEvaluator::AffectsBipEvaluate(Clause c) {
  Table pkbData(0);
  ClauseResult result;
  /*pkbData = mypkb.getAffectsBip();*/
  result = dataFilter(pkbData, c);
  return result;
}

ClauseResult PqlEvaluator::AffectsBipTEvaluate(Clause c) {
  Table pkbData(0);
  ClauseResult result;
  /*pkbData = mypkb.getAffectsBipT();*/
  result = dataFilter(pkbData, c);
  return result;
}
ClauseResult PqlEvaluator::NextBipEvaluate(Clause c) {
  Table pkbData(0);
  ClauseResult result;
  pkbData = mypkb.getNextBip();
  result = dataFilter(pkbData, c);
  return result;
}
ClauseResult PqlEvaluator::NextBipTEvaluate(Clause c) {
  Table pkbData(0);
  ClauseResult result;
  pkbData = mypkb.getNextBipT();
  result = dataFilter(pkbData, c);
  return result;
}
// get data from pkb according to requested entity type:stmt, assign, while,
// if...
Table PqlEvaluator::getdataByTtype(QueryEntity q) {
  if (isSynonym(q.type)) {
    if (q.type == QueryEntityType::Variable) {
      return mypkb.getVarTable();
    }
    if (q.type == QueryEntityType::Procedure) {
      return mypkb.getProcTable();
    }
    if (q.type == QueryEntityType::Constant) {
      return mypkb.getConstTable();
    }
    StatementType st = convertQType(q.type);
    return mypkb.getStmtType(st);
  }
  if (isConstant(q.type)) {
    Table t(1);
    t.insertRow({q.name});
    return t;
  }
}
// get data from pkb that requested entity type is AttrRef: s.stmt#,prog_line
Table PqlEvaluator::getdataWith(QueryEntity q) {
  if (q.type != QueryEntityType::Attrref) {
    return Table(0);
  }
  vector<string> temp = split(q.name);

  Table result(0);
  if (temp[1] == "stmt#" || temp[1] == "value") {
    result = getdataByTtype(q.attrRefSynonymType);
    result.setHeader({temp[0]});
    result = duplicateCols(q.name, result);
  } else if (temp[1] == "procName") {
    if (q.attrRefSynonymType == QueryEntityType::Procedure) {
      result = getdataByTtype(q.attrRefSynonymType);
      result.setHeader({temp[0]});
      result = duplicateCols(q.name, result);
    } else if (q.attrRefSynonymType == QueryEntityType::Call) {
      result = mypkb.getCallProcName();
      result.setHeader({temp[0], q.name});
    }
  } else if (temp[1] == "varName") {
    if (q.attrRefSynonymType == QueryEntityType::Variable) {
      result = getdataByTtype(q.attrRefSynonymType);
      result.setHeader({temp[0]});
      result = duplicateCols(q.name, result);
    } else if (q.attrRefSynonymType == QueryEntityType::Read) {
      Table read = getdataByTtype(QueryEntityType::Read);
      Table modifies = mypkb.getModifiesS();
      read.setHeader({temp[0]});
      modifies.setHeader({temp[0], q.name});
      read.mergeWith(modifies);
      read.setHeader({temp[0], q.name});
      result = read;
    } else if (q.attrRefSynonymType == QueryEntityType::Print) {
      Table print = getdataByTtype(QueryEntityType::Print);
      Table uses = mypkb.getUsesS();
      print.setHeader({temp[0]});
      uses.setHeader({temp[0], q.name});
      print.mergeWith(uses);
      print.setHeader({temp[0], q.name});
      result = print;
    }
  }
  return result;
}
// accept empty result table, if select boolean, return flase;else,return none
Table PqlEvaluator::validateResult(Table t, vector<QueryEntity> target) {
  if (t.empty()) {
    if (target.front().type == QueryEntityType::Boolean) {
      Table resultTable(1);
      resultTable.insertRow({"FALSE"});
      return resultTable;
    }
    Table resultTable(0);
    return resultTable;
  }
}
// convert query entity type to corresbonding statement type,which are
// accepted by pkb to retrieve data
StatementType PqlEvaluator::convertQType(QueryEntityType q) {
  if (q == QueryEntityType::While)
    return StatementType::While;
  if (q == QueryEntityType::If)
    return StatementType::If;
  if (q == QueryEntityType::Read)
    return StatementType::Read;
  if (q == QueryEntityType::Print)
    return StatementType::Print;
  if (q == QueryEntityType::Call)
    return StatementType::Call;
  if (q == QueryEntityType::Assign)
    return StatementType::Assign;
  if (q == QueryEntityType::Stmt || q == QueryEntityType::Progline)
    return StatementType::Stmt;
}
// convert clausetype to readable string
string PqlEvaluator::convertClauseTypeToString(ClauseType ct) {
  switch (ct) {
  case ClauseType::Affects:
    return "affects";
    break;
  case ClauseType::AffectsT:
    return "affectsT";
    break;
  case ClauseType::NextT:
    return "nextT";
    break;
  default:
    return "";
    break;
  }
}
// convert selected tuple to table form
Table PqlEvaluator::targetsToTable(vector<QueryEntity> t) {
  set<string> result;
  for (QueryEntity qe : t) {
    if (isAttr(qe.type)) {
      vector<string> names = split(qe.name);
      result.insert(names[0]);
    } else if (t.front().type == QueryEntityType::Boolean) {

    } else {
      string name = qe.name;
      result.insert(name);
    }
  }
  vector<string> newTargets(result.begin(), result.end());
  Table table(newTargets.size());
  table.setHeader(newTargets);
  return table;
}
// classify the query entity
bool PqlEvaluator::isSynonym(QueryEntityType q) {
  return q == QueryEntityType::Stmt || q == QueryEntityType::Read ||
         q == QueryEntityType::Print || q == QueryEntityType::Call ||
         q == QueryEntityType::While || q == QueryEntityType::If ||
         q == QueryEntityType::Assign || q == QueryEntityType::Variable ||
         q == QueryEntityType::Constant || q == QueryEntityType::Procedure ||
         q == QueryEntityType::Progline;
}

bool PqlEvaluator::isUnderscore(QueryEntityType q) {
  return q == QueryEntityType::Underscore;
}

bool PqlEvaluator::isConstant(QueryEntityType q) {
  return q == QueryEntityType::Expression || q == QueryEntityType::Boolean ||
         q == QueryEntityType::Line || q == QueryEntityType::Name;
}

bool PqlEvaluator::isAttr(QueryEntityType q) {
  return q == QueryEntityType::Attrref;
}
