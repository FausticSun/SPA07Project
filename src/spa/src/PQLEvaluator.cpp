#include "PQLParser.h"

#include <PQLEvaluator.h>

//bool checkForFalse(vector<ClauseResult> clauseResults) {
//  vector<ClauseResult>::iterator iter = clauseResults.begin();
//  for (iter; iter != clauseResults.end(); iter++) {
//    if ((iter->isBool == true) && (iter->bValue == false)) {
//      return true;
//    }
//  }
//  return false;
//}
//
//bool isAllTrue(vector<ClauseResult> clauseResults) {
//  vector<ClauseResult>::iterator iter = clauseResults.begin();
//  for (iter; iter != clauseResults.end(); iter++) {
//    if ((iter->isBool == false)) {
//      return false;
//    }
//  }
//  return true;
//}
//
//vector<ClauseResult> removeBoolean(vector<ClauseResult> clauseResults) {
//  vector<ClauseResult>::iterator iter = clauseResults.begin();
//  for (iter; iter != clauseResults.end();) {
//    if (iter->isBool == true) {
//      iter = clauseResults.erase(iter);
//    } else {
//      iter++;
//    }
//  }
//  return clauseResults;
//}

bool isPartial(string s) {
  char ending = '_';
  if (s.size() > 2 && s[0] == ending && s[s.size() - 1] == ending) {
    return true;
  }
  return false;
}

string removeUnderscore(string s) { return s.substr(1, s.size() - 2); }

//bool ClauseResult::contains(QueryEntity &q) {
//  for (int i = 0; i < titles.size(); i++) {
//    if (titles[i].name == q.name && titles[i].type == q.type) {
//      return true;
//    }
//  }
//  return false;
//}

PqlEvaluator::PqlEvaluator(const PKB &pkb) { this->mypkb = pkb; }

list<string> PqlEvaluator::evaluateQuery(string query) {
  list<string> results;
  try {
    PQLParser pp;
    queue<QueryToken> tokens = pp.parse(query);
    Query q = pp.buildQuery(tokens);
    results = executeQuery(q);
  } catch (invalid_argument ia) {
    return results;
  } catch (logic_error le) {
    return results;
  }

  return results;
}

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

list<string> PqlEvaluator::resultFormater(Table t) {
  set<vector<string>>::iterator iterRow = t.getData().begin();
  list<string> result;
  string tuple = "";
  for (iterRow; iterRow != t.getData().end(); ++iterRow) {
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

Table PqlEvaluator::resultExtractor(Table result, Query q) {
  if (q.target.front() == QueryEntityType::Boolean) {
    if (result.size() > 0) {
      Table t(1);
      t.insertRow({"TRUE"});
      return t;
    }else {
			Table t(1);
			t.insertRow({ "FALSE" });
			return t;
    }
    
  }else {
    if(result.empty()) {
			return Table(0);
    }
		else {
			int count = 0;
			vector<Table> tables;
			for (int i = 0; i < q.target.size(); i++) {
				if (isConstant(q.target[i].type)) {
					Table t(1);
					t.insertRow({ q.target[i].name });
					t.setHeader({ to_string(count) });
					tables.push_back(t);
					count++;
				}
				else if (isSynonym(q.target[i].type)) {
					if (find(result.getHeader().begin(), result.getHeader().end(),
						q.target[i].name) != result.getHeader().end()) {
						set<vector<string>> datarows = result.getData({ q.target[i].name });
						Table t(1);
						t.setHeader({ q.target[i].name });
						for (vector<string> row : datarows) {
							t.insertRow(row);
						}
						tables.push_back(t);
					}
					else {
						Table t = getdataByTtype(q.target[i].type);
						tables.push_back(t);

					}
				}
			}
			Table result = tables[0];
			for (int i = 1; i < tables.size(); i++) {
				result.mergeWith(tables[i]);
			}
			return result;
		}
		
  }
  
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
      tables.push_back(getdataByTtype(q.type));
    } else if (isConstant(q.type)) {
      Table t(1);
      t.insertRow({q.name});
      t.setHeader({to_string(count)});
      tables.push_back(t);
      count++;
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
  for (iter; iter != clauses.end(); ++iter) {
    if (iter->clauseType == ClauseType::ModifiesS) {
      if ((iter->parameters.front().type == QueryEntityType::Procedure ||
           iter->parameters.front().type == QueryEntityType::Name)) {
        data = mypkb.getModifiesP();
      } else {
        data = mypkb.getModifiesS();
      }
    } else if (iter->clauseType == ClauseType::UsesS) {
      if ((iter->parameters.front().type == QueryEntityType::Procedure ||
           iter->parameters.front().type == QueryEntityType::Name)) {
        data = mypkb.getUsesP();
      } else {
        data = mypkb.getUsesS();
      }

    } else if (iter->clauseType == ClauseType::Parent) {
      data = mypkb.getParent();
    } else if (iter->clauseType == ClauseType::ParentT) {
      data = mypkb.getParentT();
    } else if (iter->clauseType == ClauseType::Follows) {
      data = mypkb.getFollows();
    } else if (iter->clauseType == ClauseType::FollowsT) {
      data = mypkb.getFollowsT();
    } else if (iter->clauseType == ClauseType::AssignPatt) {
      if (isConstant(iter->parameters[2].type)) {
        if (isPartial(iter->parameters[2].name)) {
          data = mypkb.
            getAssignMatches(removeUnderscore(iter->parameters[2].name), true);
        } else {
          data = mypkb.getAssignMatches((iter->parameters[2].name), false);
        }
      } else {
        data = mypkb.getAssignMatches("", true);
      }
    } else if (iter->clauseType == ClauseType::WhilePatt) {
      data = mypkb.getWhileMatches();
    } else if (iter->clauseType == ClauseType::IfPatt) {
      data = mypkb.getIfMatches();
    } else if (iter->clauseType == ClauseType::With) {

    }
    ClauseResult result = dataFilter(data, *iter);
    if (result.isBool && !result.boolValue) {
			
      if(q.target.front().type == QueryEntityType::Boolean) {
				Table result(1);
				result.insertRow({"False"});
				return result;
      }
			else {
				Table result(0);
				return result;
			}
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
		Table result = resultExtractor(tables[0], q);
		return result;
	}else {
		Table result = executeSimpleQuery(q.target);
		return result;
	}
	
 
}

// booleans: cons cons,cons _,_ cons, _ _
// data: s cons, cons s, s s, s _, _ s

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

//
//bool PqlEvaluator::isVar(string result, QueryEntityType q) {
//  return mypkb.isVar(result) && q == QueryEntityType::Variable;
//}
//
//bool PqlEvaluator::isPro(string result, QueryEntityType q) {
//  return mypkb.isProc(result) && q == QueryEntityType::Procedure;
//}
