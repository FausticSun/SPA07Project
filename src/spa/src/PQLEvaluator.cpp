#include "PQLParser.h"

#include <PQLEvaluator.h>

bool checkForFalse(vector<ClauseResult> clauseResults) {
  vector<ClauseResult>::iterator iter = clauseResults.begin();
  for (iter; iter != clauseResults.end(); iter++) {
    if ((iter->isBool == true) && (iter->bValue == false)) {
      return true;
    }
  }
  return false;
}

bool isAllTrue(vector<ClauseResult> clauseResults) {
  vector<ClauseResult>::iterator iter = clauseResults.begin();
  for (iter; iter != clauseResults.end(); iter++) {
    if ((iter->isBool == false)) {
      return false;
    }
  }
  return true;
}

vector<ClauseResult> removeBoolean(vector<ClauseResult> clauseResults) {
  vector<ClauseResult>::iterator iter = clauseResults.begin();
  for (iter; iter != clauseResults.end();) {
    if (iter->isBool == true) {
      iter = clauseResults.erase(iter);
    } else {
      iter++;
    }
  }
  return clauseResults;
}

bool isPartial(string s) {
  char ending = '_';
  if (s.size() > 2 && s[0] == ending && s[s.size() - 1] == ending) {
    return true;
  }
  return false;
}

string removeUnderscore(string s) { return s.substr(1, s.size() - 2); }

bool ClauseResult::contains(QueryEntity &q) {
  for (int i = 0; i < titles.size(); i++) {
    if (titles[i].name == q.name && titles[i].type == q.type) {
      return true;
    }
  }
  return false;
}

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
  vector<Clause> clauses = q.clauses;
  vector<ClauseResult> clauseResults;
  list<string> results;
  if (q.clauses.empty()) {
    results = executeSimpleQuery(q.target);
    results.sort();
    results.unique();
    return results;
  }

  vector<Clause>::iterator iter = clauses.begin();
  for (iter; iter != clauses.end(); iter++) {
    if (iter->clauseType == ClauseType::ModifiesS) {
      clauseResults.push_back(getModifies(*iter));
    } else if (iter->clauseType == ClauseType::UsesS) {
      clauseResults.push_back(getUses(*iter));
    } else if (iter->clauseType == ClauseType::Parent) {
      clauseResults.push_back(getParent(*iter));
    } else if (iter->clauseType == ClauseType::ParentT) {
      clauseResults.push_back(getParentS(*iter));
    } else if (iter->clauseType == ClauseType::Follows) {
      clauseResults.push_back(getFollows(*iter));
    } else if (iter->clauseType == ClauseType::FollowsT) {
      clauseResults.push_back(getFollowsS(*iter));
    } else if (iter->clauseType == ClauseType::AssignPatt) {
      clauseResults.push_back(getAssPatern(*iter));
    }
  }
  if (checkForFalse(clauseResults)) {
    return results;
  } else if (isAllTrue(clauseResults)) {
    // return simple query
    results = executeSimpleQuery(q.target.type);
    results.sort();
    results.unique();
    return results;
  } else {
    vector<ClauseResult> temp = removeBoolean(clauseResults);
    MergeTables mt(temp);
    ClauseResult finalTable = mt.getResultTables();
    if (finalTable.contains(q.target)) {
      int index;
      for (int i = 0; i < finalTable.titles.size(); i++) {
        if (finalTable.titles[i].type == q.target.type &&
            finalTable.titles[i].name == q.target.name) {
          index = i;
          break;
        }
      }
      for (unsigned j = 0; j < finalTable.resultTable.size(); j++) {
        results.push_back(finalTable.resultTable[j][index]);
      }
    } else if (finalTable.resultTable.empty()) {
      return results;
    } else {
      results = executeSimpleQuery(q.target.type);
      results.sort();
      results.unique();
      return results;
    }
  }
  results.sort();
  results.unique();
  return results;
}

list<string> PqlEvaluator::executeSimpleQuery(vector<QueryEntity> t) {
	int count = 0;
	vector<Table> tables;
	for(QueryEntity q: t) {
	  if(isSynonym(q.type)) {
		  tables.push_back(getdataByTtype(q.type));
	  }
	}
  
}

// booleans: cons cons,cons _,_ cons, _ _
// data: s cons, cons s, s s, s _, _ s

Table PqlEvaluator::getdataByTtype(QueryEntityType q) {
  if (q == QueryEntityType::Variable) {
    return mypkb.getVarTable();
  } else if (q == QueryEntityType::Procedure) {
    return mypkb.getProcTable();
  }
  else if(q == QueryEntityType::Constant) {
	  return mypkb.getConstTable;
  }
  else {
    StatementType st = convertQType(q);
    return mypkb.getStmtType(st);
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
  if (q == QueryEntityType::Stmt)
    return StatementType::Stmt;
}

bool PqlEvaluator::isVar(string result, QueryEntityType q) {
  return mypkb.isVar(result) && q == QueryEntityType::Variable;
}

bool PqlEvaluator::isPro(string result, QueryEntityType q) {
  return mypkb.isProc(result) && q == QueryEntityType::Procedure;
}
