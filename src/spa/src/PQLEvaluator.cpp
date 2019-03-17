#include "PQLParser.h"

#include <PQLEvaluator.h>
#include <iostream>
#include <sstream>

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

bool isPartial(string s) {
  char ending = '_';
  if (s.size() > 2 && s[0] == ending && s[s.size() - 1] == ending) {
    return true;
  }
  return false;
}

string removeUnderscore(string s) { return s.substr(1, s.size() - 2); }

PqlEvaluator::PqlEvaluator(const PKB &pkb) { this->mypkb = pkb; }

list<string> PqlEvaluator::executeQuery(Query &q) {
  Table resultTable(0);
  list<string> results;
  if (q.clauses.empty()) {
    resultTable = executeSimpleQuery(q.target);
    results = resultFormater(resultTable);
    return results;
  }
  resultTable = executeComplexQuery(q);
  results = resultFormater(resultTable);
  return results;

  return results;
}

Table PqlEvaluator::resultExtractor(Table result, Query q) {
  if (q.target.front() == QueryEntityType::Boolean) {
    if (result.size() > 0) {
      Table t(1);
      t.insertRow({"TRUE"});
      return t;
    }
    Table t(1);
    t.insertRow({"FALSE"});
    return t;
  }
  if (result.empty()) {
    return Table(0);
  }
  int count = 0;
  vector<Table> tables;
  for (int i = 0; i < q.target.size(); i++) {
    if (isConstant(q.target[i].type)) {
      Table t(1);
      t.insertRow({q.target[i].name});
      t.setHeader({to_string(count)});
      tables.push_back(t);
      count++;
    } else if (isSynonym(q.target[i].type)) {
      if (find(result.getHeader().begin(), result.getHeader().end(),
               q.target[i].name) != result.getHeader().end()) {
        set<vector<string>> datarows = result.getData({q.target[i].name});
        Table t(1);
        t.setHeader({q.target[i].name});
        for (vector<string> row : datarows) {
          t.insertRow(row);
        }
        tables.push_back(t);
      } else {
        Table t = getdataByTtype(q.target[i].type);
        tables.push_back(t);

      }
    } else if (isAttr(q.target[i].type)) {
      //lishan
      Table t = getdataWith(q.target[i]);
      vector<string> temp = split(q.target[i].name, '.');
      if (find(result.getHeader().begin(), result.getHeader().end(),
               temp[0]) != result.getHeader().end()) {
        set<vector<string>> datarows = result.getData({temp[0]});
        Table tempTable(1);
				tempTable.setHeader({"temp"});
        for (vector<string> row : datarows) {
					tempTable.insertRow(row);
        }
        t.modifyHeader(t.getHeader()[0], "temp");
        t.mergeWith(tempTable);
        if (t.getHeader().size() == 2) {
          t.dropColumn("temp");
        }
        tables.push_back(t);

      } else {
        if (t.getHeader().size() == 2) {
          t.dropColumn(t.getHeader()[0]);
        }
        tables.push_back(t);
      }
    }
  }
  Table resultTable = tables[0];
  for (int i = 1; i < tables.size(); i++) {
		resultTable.mergeWith(tables[i]);
  }
  return resultTable;

}

list<string> PqlEvaluator::resultFormater(Table t) {
	set<vector<string>> tempData = t.getData();
  set<vector<string>>::iterator iterRow ;
  list<string> result;
  string tuple = "";
  
  for (iterRow = tempData.begin(); iterRow != tempData.end(); iterRow++) {
    vector<string> temp = *iterRow;
    for (int i = 0; i < temp.size(); i++) {
      tuple = tuple + temp[i] + " ";
    }
    tuple = tuple.substr(0, tuple.size() - 1);
    result.push_back(tuple);
    tuple = "";
  }
  return result;
}

Table PqlEvaluator::executeSimpleQuery(vector<QueryEntity> t) {
  int count = 0;
  vector<Table> tables;
  for (QueryEntity q : t) {
    if (q.type == QueryEntityType::Boolean) {
      Table result(1);
      result.insertRow({"TRUE"});
      return result;
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
    }else if(isAttr(q.type)) {
			Table t = getdataWith(q);
      if(t.getHeader().size()==2) {
				t.dropColumn(t.getHeader()[0]);
      }
      
			tables.push_back(t);
    }
  }
  Table result = tables[0];
  for (int i = 1; i < tables.size(); i++) {
    result.mergeWith(tables[i]);
  }
  return result;
}

Table PqlEvaluator::executeComplexQuery(Query q) {
  vector<Clause> clauses = q.clauses;
  vector<Table> tables;
  vector<Clause>::iterator iter = clauses.begin();
  Table data(0);
  ClauseResult result;
	for (iter; iter != clauses.end(); ++iter) {

		if (iter->clauseType == ClauseType::ModifiesS) {
			if ((iter->parameters.front().type == QueryEntityType::Procedure ||
				iter->parameters.front().type == QueryEntityType::Name)) {
				data = mypkb.getModifiesP();
			}
			else {
				data = mypkb.getModifiesS();
			}
			result = dataFilter(data, *iter);
		}
		else if (iter->clauseType == ClauseType::UsesS) {
			if ((iter->parameters.front().type == QueryEntityType::Procedure ||
				iter->parameters.front().type == QueryEntityType::Name)) {
				data = mypkb.getUsesP();
			}
			else {
				data = mypkb.getUsesS();
			}
			result = dataFilter(data, *iter);

		}
		else if (iter->clauseType == ClauseType::Parent) {
			data = mypkb.getParent();
			result = dataFilter(data, *iter);
		}
		else if (iter->clauseType == ClauseType::ParentT) {
			data = mypkb.getParentT();
			result = dataFilter(data, *iter);
		}
		else if (iter->clauseType == ClauseType::Follows) {
			data = mypkb.getFollows();
			result = dataFilter(data, *iter);
		}
		else if (iter->clauseType == ClauseType::FollowsT) {
			data = mypkb.getFollowsT();
			result = dataFilter(data, *iter);
		}
		else if (iter->clauseType == ClauseType::AssignPatt) {
			if (isConstant(iter->parameters[2].type)) {
				if (isPartial(iter->parameters[2].name)) {
					data = mypkb.
						getAssignMatches(removeUnderscore(iter->parameters[2].name),
							true);
				}
				else {
					data = mypkb.getAssignMatches((iter->parameters[2].name), false);
				}
			}
			else {
				data = mypkb.getAssignMatches("", true);
			}
			result = dataFilter(data, *iter);
		}
		else if (iter->clauseType == ClauseType::WhilePatt) {
			data = mypkb.getWhileMatches();
			result = dataFilter(data, *iter);
		}
		else if (iter->clauseType == ClauseType::IfPatt) {
			data = mypkb.getIfMatches();
			result = dataFilter(data, *iter);
		}

		else if (iter->clauseType == ClauseType::With) {
			result = withEvaluate(*iter);
		}
		else if (iter->clauseType == ClauseType::Next) {
			data = mypkb.getNext();
			result = dataFilter(data, *iter);
		}else if(iter->clauseType == ClauseType::Calls) {
			data = mypkb.getCalls();
			result = dataFilter(data, *iter);
		}else if(iter->clauseType == ClauseType::CallsT) {
			data = mypkb.getCalls();
			result = dataFilter(data, *iter);
		}
		/*else if (iter->clauseType == ClauseType::NextT) {

		}*/
    if (result.isBool && !result.boolValue) {

      if (q.target.front().type == QueryEntityType::Boolean) {
        Table resultTable(1);
				resultTable.insertRow({"False"});
        return resultTable;
      }
      Table resultTable(0);
      return resultTable;
    }
    if (result.isBool && result.boolValue) {

    } else {
      tables.push_back(data);
    }
  }
  if (!tables.empty()) {
    for (int i = 1; i < tables.size(); i++) {
      tables[0].mergeWith(tables[i]);
    }
    Table complexResult = resultExtractor(tables[0], q);
    return complexResult;
  }
  Table simpleResult = executeSimpleQuery(q.target);
  return simpleResult;

}

ClauseResult PqlEvaluator::dataFilter(Table data, Clause c) {
  QueryEntity qe1 = c.parameters[0];
  QueryEntity qe2 = c.parameters[1];
  vector<Table> columns;
  data.setHeader({"qe1", "qe2"});
  if (!isUnderscore(qe1.type)) {
    Table col1 = getdataByTtype(qe1);
    col1.setHeader({"qe1"});
    columns.push_back(col1);
  }
  if (!isUnderscore(qe2.type)) {
    Table col2 = getdataByTtype(qe2);
    col2.setHeader({"qe2"});
    columns.push_back(col2);
  }
  for (int i = 0; i < columns.size(); i++) {
    data.mergeWith(columns[i]);
  }
  data.setHeader({qe1.name, qe2.name});

  if (data.empty()) {
    ClauseResult result(true, false);
    return result;
  }
  if (isSynonym(qe1.type) || isSynonym(qe2.type)) {
    if (!isSynonym(qe1.type)) {
      data.dropColumn(qe1.name);
    }
    if (!isSynonym(qe2.type)) {
      data.dropColumn(qe2.name);
    }
    ClauseResult result(false, false);
    result.data = data;
    return result;
  }
  return ClauseResult(true, true);
}

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
  set<vector<string>> data;
  Table t(expectation.size());
  if (expectation.size() == 1) {
    data = tables[0].getData({expectation[0].first});
    t.setHeader({expectation[0].second});
  } else if (expectation.size() == 2) {
    data = tables[0].getData({expectation[0].first, expectation[1].first});
    t.setHeader({expectation[0].second, expectation[1].second});
  }

  set<vector<string>>::iterator iterRow = data.begin();
  for (iterRow; iterRow != data.end(); ++iterRow) {
    vector<string> temp = *iterRow;
    t.insertRow(temp);
  }
  ClauseResult result(false, true);
  result.data = t;
  return result;
}

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

Table PqlEvaluator::getdataWith(QueryEntity q) {
  if (q.type != QueryEntityType::Attrref) {
    return Table(0);
  }
  vector<string> temp = split(q.name);

  Table result(0);
  if (temp[1] == "stmt#" || temp[1] == "value") {
    result = getdataByTtype(q.attrRefSynonymType);
    result.setHeader({q.name});
  } else if (temp[1] == "procName") {
    if (q.attrRefSynonymType == QueryEntityType::Procedure) {
      result = getdataByTtype(q.attrRefSynonymType);
      result.setHeader({q.name});
    } else if (q.attrRefSynonymType == QueryEntityType::Call) {
      result = mypkb.getCallProcName();
      result.setHeader({temp[0], q.name});
    }
  } else if (temp[1] == "varName") {
    if (q.attrRefSynonymType == QueryEntityType::Variable) {
      result = getdataByTtype(q.attrRefSynonymType);
      result.setHeader({q.name});
    } else if (q.attrRefSynonymType == QueryEntityType::Read) {
      Table read = getdataByTtype(QueryEntityType::Read);
      Table modifies = mypkb.getModifiesS();
      read.mergeWith(modifies);
      read.setHeader({temp[0], q.name});
    } else if (q.attrRefSynonymType == QueryEntityType::Print) {
      Table print = getdataByTtype(QueryEntityType::Print);
      Table uses = mypkb.getUsesS();
      print.mergeWith(uses);
      uses.setHeader({temp[0], q.name});
    }
  }
  return result;
}

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
