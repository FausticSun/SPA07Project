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
Table getCols(vector<string> s,Table t) {
  if(s.empty()) {
		return Table(0);
  }else {
		set<vector<string>> rows = t.getData(s);
		Table result(s.size());
		result.setHeader(s);
    for (vector<string> row : rows) {
			result.insertRow(row);

    }
		return result;
  }

}
Table duplicateCols(string s,Table t) {
	if (t.getHeader().size() == 1) {
		set<vector<string>> rows = t.getData();
		Table result(2);
		result.setHeader({ t.getHeader()[0], s });
		for (vector<string> row : rows) {
			row.push_back(row[0]);
			result.insertRow(row);
		}
		return result;
	}
	else {
		return Table(0);
	}
}
Table rowsToTable(set<vector<string>> rows, vector<string> header) {
	Table result(header.size());
	result.setHeader(header);
	for (vector<string> row : rows) {
		if (row.size() == header.size()) {
			result.insertRow(row);
		}
		else {
			return Table(0);
		}
	}
	return result;
}
Table selfJoin(Table t) {
  if(t.getHeader().size()==2) {
		Table result(2);
		result.setHeader(t.getHeader());
    for(vector<string> row:t.getData()) {
      if(row[0]==row[1]) {
				result.insertRow(row);
      }
    }
		return result;
  }
	else {
		return Table(0);
	}
}

PqlEvaluator::PqlEvaluator(const PKB &pkb) { this->mypkb = pkb; }

list<string> PqlEvaluator::executeQuery(Query &q) {
  list<string> results;
  if (q.clauses.empty()) {
		set<vector<string>> resultTable = executeSimpleQuery(q.target);
    results = resultFormater(resultTable);
    return results;
  }
	set<vector<string>> resultTable = executeComplexQuery(q);
  results = resultFormater(resultTable);
  return results;

  return results;
}

set<vector<string>> PqlEvaluator::resultExtractor(Table result, Query q) {
	vector<string> s;
	vector<QueryEntity> attr;
	vector<Table> tables;
	Table tempTable(0);
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
  if (result.empty()) {
		set<vector<string>> t;
    return t;
  }else {
		vector<string> header = result.getHeader();
		for (QueryEntity qe : q.target) {
			header = result.getHeader();
			if (isAttr(qe.type)) {
				vector<string> temp = split(qe.name, '.');
				if (find(header.begin(), header.end(), temp[0])!= header.end()) {
					Table t = getdataWith(qe);
					tables.push_back(t);
					result.mergeWith(t);
				}
				else {
					Table t = getdataWith(qe);
					t.dropColumn(t.getHeader()[0]);
					tables.push_back(t);
					result.mergeWith(t);
				}
			}
			else if (isSynonym(qe.type)) {
				if (find(header.begin(), header.end(), qe.name) != header.end()) {
				}
				else {
					Table t = getdataByTtype(qe.type);
					t.setHeader({ qe.name });
					tables.push_back(t);
					result.mergeWith(t);
				}
			}
			s.push_back(qe.name);
		}
		/*for (Table t : tables) {
			result.mergeWith(t);
		}*/
  }
	set<vector<string>> resultTable = result.getData(s);
  
  return resultTable;

}

list<string> PqlEvaluator::resultFormater(set<vector<string>> t) {
	set<vector<string>> tempData = t;
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
    }else if(isAttr(q.type)) {
			Table t = getdataWith(q);
      if(t.getHeader().size()==2) {
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

set<vector<string>> PqlEvaluator::executeComplexQuery(Query q) {
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
			data = mypkb.getCallsT();
			result = dataFilter(data, *iter);
		}
		else if (iter->clauseType == ClauseType::NextT) {
			result = NextTEvaluate(*iter);
		}
		else if (iter->clauseType == ClauseType::Affects) {
			result = AffectEvaluate(*iter);
		}/*else if (iter->clauseType == ClauseType::AffectsT) {
		}*/
    if (result.isBool && !result.boolValue) {

      if (q.target.front().type == QueryEntityType::Boolean) {
        Table resultTable(1);
				resultTable.insertRow({"FALSE"});
        return resultTable.getData();
      }
      Table resultTable(0);
      return resultTable.getData();
    }
    if (result.isBool && result.boolValue) {

    } else {
      tables.push_back(result.data);
    }
  }
  if (!tables.empty()) {
    for (int i = 1; i < tables.size(); i++) {
      tables[0].mergeWith(tables[i]);
    }
    set<vector<string>> complexResult = resultExtractor(tables[0], q);
    return complexResult;
  }
	set<vector<string>> simpleResult = executeSimpleQuery(q.target);
  return simpleResult;

}

ClauseResult PqlEvaluator::dataFilter(Table data, Clause c) {
  QueryEntity qe1 = c.parameters[0];
  QueryEntity qe2 = c.parameters[1];
  vector<Table> columns;
  data.setHeader({"1", "2"});
  if (!isUnderscore(qe1.type)) {
    Table col1 = getdataByTtype(qe1);
    col1.setHeader({"1"});
    columns.push_back(col1);
  }
  if (!isUnderscore(qe2.type)) {
    Table col2 = getdataByTtype(qe2);
    col2.setHeader({"2"});
    columns.push_back(col2);
  }
  for (int i = 0; i < columns.size(); i++) {
    data.mergeWith(columns[i]);
  }

	if (isSynonym(qe1.type) && isSynonym(qe2.type) && qe1.name == qe2.name) {
		data = selfJoin(data);
		data.dropColumn("2");
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
		  if(title=="1") {
				data.modifyHeader("1", qe1.name);
		  }
			else if(title=="2") {
				data.modifyHeader("2", qe2.name);
			}
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
		t = rowsToTable(data, { expectation[0].second });
  } else if (expectation.size() == 2 && expectation[0].second != expectation[1].second) {
    data = tables[0].getData({expectation[0].first, expectation[1].first});
    t.setHeader({expectation[0].second, expectation[1].second});
		t = rowsToTable(data, { expectation[0].second, expectation[1].second });
  }
	else if (expectation.size() == 2 && expectation[0].second == expectation[1].second) {
		data = tables[0].getData({ expectation[0].first});
		t.setHeader({ expectation[0].second});
		t = rowsToTable(data,{ expectation[0].second });
	}
  ClauseResult result(false, true);
  result.data = t;
  return result;
}

ClauseResult PqlEvaluator::NextTEvaluate(Clause c) {
	QueryEntity qe1 = c.parameters[0];
	QueryEntity qe2 = c.parameters[1];
	Table result(0);
	if (isConstant(qe1.type)) {
		Table col1(1);
		col1.setHeader({ "1" });
		col1.insertRow({ qe1.name });
		Table pkbData = mypkb.getNextT(stoi(qe1.name), true);
		pkbData.setHeader({"2"});
		pkbData.mergeWith(col1);
		
		if (isConstant(qe2.type)) {
			Table col2(1);
			col2.setHeader({"2"});
			col2.insertRow({ qe2.name });
			pkbData.mergeWith(col2);
    } else if(isSynonym(qe2.type)) {
			Table col2 = getdataByTtype(qe2.type);
			col2.setHeader({ "2" });
			pkbData.mergeWith(col2);
		}
		result = pkbData;
	}
	else if (isConstant(qe2.type)) {
		Table col2(1);
		col2.setHeader({ "2" });
		col2.insertRow({ qe2.name });
		Table pkbData = mypkb.getNextT(stoi(qe2.name), false);
		pkbData.setHeader({"1"});
		pkbData.mergeWith(col2);
		if (isConstant(qe1.type)) {
			Table col1(1);
			col1.setHeader({ "1" });
			col1.insertRow({ qe1.name });
			pkbData.mergeWith(col1);
		}
		else if (isSynonym(qe1.type)) {
			Table col1 = getdataByTtype(qe1);
			col1.setHeader({ "1" });
			pkbData.mergeWith(col1);
		}
		result = pkbData;
	}
	else if (isSynonym(qe1.type)) {
		set<vector<string>> col1 = getdataByTtype(qe1).getData();
		Table pkbData(2);
		pkbData.setHeader({ "1","2" });
		for (vector<string> row : col1) {
			Table col1(1);
			col1.setHeader({ "1" });
			col1.insertRow({row[0]});
				Table t2 = mypkb.getNextT(stoi(row[0]),true);
				t2.setHeader({ "2" });
				col1.mergeWith(t2);
			pkbData.concatenate(col1);
		}
		if (isSynonym(qe2.type)) {
			Table col2 = getdataByTtype(qe2);
			col2.setHeader({ "2" });
			pkbData.mergeWith(col2);
		}
		result = pkbData;
	}
	else if (isSynonym(qe2.type)) {
		set<vector<string>> col2 = getdataByTtype(qe2).getData();
		Table pkbData(2);
		pkbData.setHeader({ "1","2" });
		for (vector<string> row : col2) {
			Table col2(1);
			col2.setHeader({ "2" });
			col2.insertRow({ row[0] });
			Table t1 = mypkb.getNextT(stoi(row[0]),false);
			t1.setHeader({ "1" });
			t1.mergeWith(col2);
			pkbData.concatenate(t1);
		}
		if (isSynonym(qe1.type)) {
			Table col1 = getdataByTtype(qe1);
			col1.setHeader({ "1" });
			pkbData.mergeWith(col1);
		}
		result = pkbData;
	}
	else {
		result = mypkb.getNextT();
	}

	if (isSynonym(qe1.type) && isSynonym(qe2.type) && qe1.name == qe2.name) {
		result = selfJoin(result);
		result.dropColumn("2");
	}

	if (result.empty()) {
		ClauseResult result(true, false);
		return result;
	}

	if (isSynonym(qe1.type) || isSynonym(qe2.type)) {
		if (!isSynonym(qe1.type)) {
			result.dropColumn("1");
		}

		if (!isSynonym(qe2.type)) {
			result.dropColumn("2");
		}
		for (string title : result.getHeader()) {
			if (title == "1") {
				result.modifyHeader("1", qe1.name);
			}
			else if (title == "2") {
				result.modifyHeader("2", qe2.name);
			}
		}
		ClauseResult clauseResult(false, false);
		clauseResult.data = result;
		return clauseResult;
	}
	return ClauseResult(true, true);

}
ClauseResult PqlEvaluator::AffectEvaluate(Clause c) {
	QueryEntity qe1 = c.parameters[0];
	QueryEntity qe2 = c.parameters[1];
	Table result(0);
	if (isConstant(qe1.type)) {
		Table col1(1);
		col1.setHeader({ "1" });
		col1.insertRow({ qe1.name });
		Table pkbData = mypkb.getAffects(stoi(qe1.name), true);
		pkbData.setHeader({ "2" });
		pkbData.mergeWith(col1);

		if (isConstant(qe2.type)) {
			Table col2(1);
			col2.setHeader({ "2" });
			col2.insertRow({ qe2.name });
			pkbData.mergeWith(col2);
		}
		else if (isSynonym(qe2.type)) {
			Table col2 = getdataByTtype(qe2.type);
			col2.setHeader({ "2" });
			pkbData.mergeWith(col2);
		}
		result = pkbData;
	}
	else if (isConstant(qe2.type)) {
		Table col2(1);
		col2.setHeader({ "2" });
		col2.insertRow({ qe2.name });
		Table pkbData = mypkb.getAffects(stoi(qe2.name), false);
		pkbData.setHeader({ "1" });
		pkbData.mergeWith(col2);
		if (isConstant(qe1.type)) {
			Table col1(1);
			col1.setHeader({ "1" });
			col1.insertRow({ qe1.name });
			pkbData.mergeWith(col1);
		}
		else if (isSynonym(qe1.type)) {
			Table col1 = getdataByTtype(qe1);
			col1.setHeader({ "1" });
			pkbData.mergeWith(col1);
		}
		result = pkbData;
	}
	else if (isSynonym(qe1.type)) {
		set<vector<string>> col1 = getdataByTtype(qe1).getData();
		Table pkbData(2);
		pkbData.setHeader({ "1","2" });
		for (vector<string> row : col1) {
			Table col1(1);
			col1.setHeader({ "1" });
			col1.insertRow({ row[0] });
			Table t2 = mypkb.getAffects(stoi(row[0]), true);
			t2.setHeader({ "2" });
			col1.mergeWith(t2);
			pkbData.concatenate(col1);
		}
		if (isSynonym(qe2.type)) {
			Table col2 = getdataByTtype(qe2);
			col2.setHeader({ "2" });
			pkbData.mergeWith(col2);
		}
		result = pkbData;
	}
	else if (isSynonym(qe2.type)) {
		set<vector<string>> col2 = getdataByTtype(qe2).getData();
		Table pkbData(2);
		pkbData.setHeader({ "1","2" });
		for (vector<string> row : col2) {
			Table col2(1);
			col2.setHeader({ "2" });
			col2.insertRow({ row[0] });
			Table t1 = mypkb.getAffects(stoi(row[0]), false);
			t1.setHeader({ "1" });
			t1.mergeWith(col2);
			pkbData.concatenate(t1);
		}
		if (isSynonym(qe1.type)) {
			Table col1 = getdataByTtype(qe1);
			col1.setHeader({ "1" });
			pkbData.mergeWith(col1);
		}
		result = pkbData;
	}
	else {
		result = mypkb.getAffects();
	}

	if (isSynonym(qe1.type) && isSynonym(qe2.type) && qe1.name == qe2.name) {
		result = selfJoin(result);
		result.dropColumn("2");
	}

	if (result.empty()) {
		ClauseResult result(true, false);
		return result;
	}

	if (isSynonym(qe1.type) || isSynonym(qe2.type)) {
		if (!isSynonym(qe1.type)) {
			result.dropColumn("1");
		}

		if (!isSynonym(qe2.type)) {
			result.dropColumn("2");
		}
		for (string title : result.getHeader()) {
			if (title == "1") {
				result.modifyHeader("1", qe1.name);
			}
			else if (title == "2") {
				result.modifyHeader("2", qe2.name);
			}
		}
		ClauseResult clauseResult(false, false);
		clauseResult.data = result;
		return clauseResult;
	}
	return ClauseResult(true, true);

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
			result.setHeader({ temp[0] });
			result = duplicateCols(q.name, result);
    } else if (q.attrRefSynonymType == QueryEntityType::Read) {
      Table read = getdataByTtype(QueryEntityType::Read);
      Table modifies = mypkb.getModifiesS();
			read.setHeader({ temp[0] });
			modifies.setHeader({temp[0],q.name});
      read.mergeWith(modifies);
      read.setHeader({temp[0], q.name});
			result = read;
    } else if (q.attrRefSynonymType == QueryEntityType::Print) {
      Table print = getdataByTtype(QueryEntityType::Print);
      Table uses = mypkb.getUsesS();
			print.setHeader({temp[0]});
			uses.setHeader({temp[0],q.name});
      print.mergeWith(uses);
      print.setHeader({temp[0], q.name});
			result = print;
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
