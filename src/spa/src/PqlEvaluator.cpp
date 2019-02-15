#include <PqlEvaluator.h>
#include "PQLParser.h"
#include "MergeTables.h"

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
bool ClauseResult::contains(QueryEntity &q)
{
  for(int i=0;i<titles.size();i++)
  {
    if(titles[i].name==q.name && titles[i].type==q.type)
    {
		return true;
    }
  }
  return false;
}

PqlEvaluator::PqlEvaluator(const PKB &pkb) { this->mypkb = pkb; }

PqlEvaluator::~PqlEvaluator() = default;

list<string> PqlEvaluator::evaluateQuery(string query) {
  list<string> results;
  try
  {
	  PQLParser pp;
	  queue<Token> tokens = pp.parse(query);
	  Query q = pp.buildQuery(tokens);
    if(q.clauses.empty())
    {
		results = executeSimpleQuery(q.target.type);
    }
	else
	{
		results = executeQuery(q);
	}

  }
  catch(invalid_argument ia)
  {
	  return results;
  }
  catch(logic_error le)
  {
	  return results;
  }

  return results;
}

list<string> PqlEvaluator::executeQuery(Query &q) {
	vector<Clause> clauses = q.clauses;
  vector<ClauseResult> clauseResults;
  list<string> results;
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
    //return simple query
	  return executeSimpleQuery(q.target.type);
  } else {
	  vector<ClauseResult> temp = removeBoolean(clauseResults);
	  MergeTables mt(temp);
	  ClauseResult finalTable = mt.getResultTables();
  }
  return results;
}

list<string> PqlEvaluator::executeSimpleQuery(QueryEntityType q)
{

	list<string> results;
  if(q==QueryEntityType::Variable)
  {
	  set<string> vars = mypkb.getVarTable();
	  list<string> temp(vars.begin(), vars.end());
	  results = temp;
  }
  else if(q==QueryEntityType::Procedure)
  {
	  set<string> vars = mypkb.getProcTable();
	  list<string> temp(vars.begin(), vars.end());
	  results = temp;
  }
  else if(q==QueryEntityType::Constant)
  {
	  return results;
  }else
  {
	  set<string> vars = mypkb.getStatementsOfType(convertQType(q));
	  list<string> temp(vars.begin(), vars.end());
	  results = temp;
  }
	return results;
}

ClauseResult PqlEvaluator::getUses(Clause c) {

  vector<QueryEntity>::iterator iter1 = c.parameters.begin();
  vector<QueryEntity>::iterator iter2 = c.parameters.begin();
  iter2++;
  QueryEntity qe1 = *iter1;
  QueryEntity qe2 = *iter2;
  vector<QueryEntity> titles;
  titles.push_back(qe1);
  titles.push_back(qe2);
  vector<vector<string>> result;

  if (isConstant(qe1.type) && isConstant(qe2.type)) {
    //both are constants
    bool bValue = mypkb.uses(qe1.name, qe2.name);
    ClauseResult clauseResult(true, bValue);
    return clauseResult;
  }
  if (isConstant(qe1.type) && isSynonym(qe2.type)) {
    //(constant,synonym)
    set<string> used = mypkb.getUses(qe1.name);
    set<string>::iterator iterr = used.begin();
    while (iterr != used.end()) {

      if (validateType(*iterr, qe2.type)) {
        vector<string> tuple;
        tuple.push_back(qe1.name);
        tuple.push_back(*iterr);
        result.push_back(tuple);
        iterr++;
      } else {
        iterr++;
        continue;
      }

    }
  }
  if (isSynonym(qe1.type) && isConstant(qe2.type)) {
    //(synonym,constant)
    set<string> uses = mypkb.getUsedBy(qe2.name);
    set<string>::iterator iterr = uses.begin();
    while (iterr != uses.end()) {
      if (validateType(*iterr, qe1.type)) {
        vector<string> tuple;
        tuple.push_back(*iterr);
        tuple.push_back(qe2.name);
        result.push_back(tuple);
        iterr++;
      } else {
        iterr++;
        continue;
      }

    }
  }
  if (isSynonym(qe1.type) && isSynonym(qe2.type)) {
    //(synonym,synonym)
    StatementType st = convertQType(qe1.type);
    set<string> uses = mypkb.getStatementsOfType(st);
    set<string>::iterator iterr1 = uses.begin();
    while (iterr1 != uses.end()) {
      set<string> used = mypkb.getUses(*iterr1);
      set<string>::iterator iterr2 = used.begin();
      while (iterr2 != used.end()) {
        if (validateType(*iterr2, qe2.type)) {
          vector<string> tuple;
          tuple.push_back(*iterr1);
          tuple.push_back(*iterr2);
          result.push_back(tuple);
          iterr2++;
        } else {
          iterr2++;
          continue;
        }
      }
      iterr1++;
    }
  }
  if (isConstant(qe1.type) && isUnderscore(qe2.type)) {
    //(constant,"_") same as (constant,synonmy)
    set<string> used = mypkb.getUses(qe1.name);
    set<string>::iterator iterr = used.begin();
    while (iterr != used.end()) {
      vector<string> tuple;
      tuple.push_back(qe1.name);
      tuple.push_back(*iterr);
      result.push_back(tuple);
      iterr++;
    }
  }
  if (isSynonym(qe1.type) && isUnderscore(qe2.type)) {
    //(synonym,"_") same as (synonym,synonym)
    StatementType st = convertQType(qe1.type);
    set<string> uses = mypkb.getStatementsOfType(st);
    set<string>::iterator iterr1 = uses.begin();
    while (iterr1 != uses.end()) {
      set<string> used = mypkb.getUses(*iterr1);
      set<string>::iterator iterr2 = used.begin();
      while (iterr2 != used.end()) {
        vector<string> tuple;
        tuple.push_back(*iterr1);
        tuple.push_back(*iterr2);
        result.push_back(tuple);
        iterr2++;
      }
      iterr1++;
    }
  }
  if (!result.empty()) {
    ClauseResult clauseResult(false, false, titles, result);
    return clauseResult;
  } else {
    return ClauseResult(false, false);
  }

}

ClauseResult PqlEvaluator::getModifies(Clause c) {
  vector<QueryEntity>::iterator iter1 = c.parameters.begin();
  vector<QueryEntity>::iterator iter2 = c.parameters.begin();
  iter2++;
  QueryEntity qe1 = *iter1;
  QueryEntity qe2 = *iter2;
  vector<QueryEntity> titles;
  titles.push_back(qe1);
  titles.push_back(qe2);
  vector<vector<string>> result;
  if (isConstant(qe1.type) && isConstant(qe2.type)) {
    //both are constants
    bool bValue = mypkb.modifies(qe1.name, qe2.name);
    ClauseResult clauseResult(true, bValue);
    return clauseResult;

  }
  if (isConstant(qe1.type) && isSynonym(qe2.type)) {
    //(constant,synonym)
    set<string> modified = mypkb.getModifies(qe1.name);
    set<string>::iterator iterr = modified.begin();
    while (iterr != modified.end()) {
      if (validateType(*iterr, qe2.type)) {
        vector<string> tuple;
        tuple.push_back(qe1.name);
        tuple.push_back(*iterr);
        result.push_back(tuple);
        iterr++;
      } else {
        iterr++;
        continue;
      }

    }
  }
  if (isSynonym(qe1.type) && isConstant(qe2.type)) {
    //(synonym,constant)
    set<string> modifies = mypkb.getModifiedBy(qe1.name);
    set<string>::iterator iterr = modifies.begin();
    while (iterr != modifies.end()) {
      if (validateType(*iterr, qe1.type)) {
        vector<string> tuple;
        tuple.push_back(qe1.name);
        tuple.push_back(*iterr);
        result.push_back(tuple);
        iterr++;
      } else {
        iterr++;
        continue;
      }

    }
  }
  if (isSynonym(qe1.type) && isSynonym(qe2.type)) {
    //(synonym,synonym)
    StatementType st = convertQType(qe1.type);
    set<string> modifies = mypkb.getStatementsOfType(st);
    set<string>::iterator iterr1 = modifies.begin();
    while (iterr1 != modifies.end()) {
      set<string> modified = mypkb.getModifies(*iterr1);
      set<string>::iterator iterr2 = modified.begin();
      while (iterr2 != modified.end()) {
        if (validateType(*iterr2, qe2.type)) {
          vector<string> tuple;
          tuple.push_back(*iterr1);
          tuple.push_back(*iterr2);
          result.push_back(tuple);
          iterr2++;
        } else {
          iterr2++;
          continue;
        }

      }
      iterr1++;
    }
  }
  if (isConstant(qe1.type) && isUnderscore(qe2.type)) {
    //(constant,"_")
    set<string> modified = mypkb.getModifies(qe1.name);
    set<string>::iterator iterr = modified.begin();
    while (iterr != modified.end()) {
      vector<string> tuple;
      tuple.push_back(qe1.name);
      tuple.push_back(*iterr);
      result.push_back(tuple);
      iterr++;
    }
  }
  if (isSynonym(qe1.type) && isUnderscore(qe2.type)) {
    //(synonym,"_")
    StatementType st = convertQType(qe1.type);
    set<string> modifies = mypkb.getStatementsOfType(st);
    set<string>::iterator iterr1 = modifies.begin();
    while (iterr1 != modifies.end()) {
      set<string> modified = mypkb.getModifies(*iterr1);
      set<string>::iterator iterr2 = modified.begin();
      while (iterr2 != modified.end()) {
        vector<string> tuple;
        tuple.push_back(*iterr1);
        tuple.push_back(*iterr2);
        result.push_back(tuple);
        iterr2++;
      }
      iterr1++;
    }
  }
  if (!result.empty()) {
    ClauseResult clauseResult(false, false, titles, result);
    return clauseResult;
  } else {
    return ClauseResult(false, false);
  }
}

ClauseResult PqlEvaluator::getParent(Clause c) {
  vector<QueryEntity>::iterator iter1 = c.parameters.begin();
  vector<QueryEntity>::iterator iter2 = c.parameters.begin();
  iter2++;
  QueryEntity qe1 = *iter1;
  QueryEntity qe2 = *iter2;
  vector<QueryEntity> titles;
  titles.push_back(qe1);
  titles.push_back(qe2);
  vector<vector<string>> result;
  if (isConstant(qe1.type) && isConstant(qe2.type)) {
    //both are constants
    bool bValue = mypkb.parent(qe1.name, qe2.name);
    ClauseResult clauseResult(true, bValue);
    return clauseResult;
  }

  if (isConstant(qe1.type) && isSynonym(qe2.type)) {
    //(constant,synonym)
    set<string> children = mypkb.getParent(qe1.name);
    set<string>::iterator iterr = children.begin();
    while (iterr != children.end()) {
      if (validateType(*iterr, qe2.type)) {
        vector<string> tuple;
        tuple.push_back(qe1.name);
        tuple.push_back(*iterr);
        result.push_back(tuple);
        iterr++;
      } else {
        iterr++;
        continue;
      }
    }
  }
  if (isSynonym(qe1.type) && isConstant(qe2.type)) {
    //(synonym,constant)
    set<string> parents = mypkb.getParentOf(qe2.name);
    set<string>::iterator iterr = parents.begin();
    while (iterr != parents.end()) {
      if (validateType(*iterr, qe1.type)) {
        vector<string> tuple;
        tuple.push_back(*iterr);
        tuple.push_back(qe2.name);
        result.push_back(tuple);
        iterr++;
      } else {
        iterr++;
        continue;
      }

    }
  }
  if (isSynonym(qe1.type) && isSynonym(qe2.type)) {
    //(synonym,synonym)
    StatementType st = convertQType(qe1.type);
    set<string> parents = mypkb.getStatementsOfType(st);
    set<string>::iterator iterr1 = parents.begin();
    while (iterr1 != parents.end()) {
      set<string> children = mypkb.getParent(*iterr1);
      set<string>::iterator iterr2 = children.begin();
      while (iterr2 != children.end()) {
        if (validateType(*iterr2, qe2.type)) {
          vector<string> tuple;
          tuple.push_back(*iterr1);
          tuple.push_back(*iterr2);
          result.push_back(tuple);
          iterr2++;
        } else {
          iterr2++;
          continue;
        }

      }
      iterr1++;
    }
  }
  if (isConstant(qe1.type) && isUnderscore(qe2.type)) {
    //(constant,"_")
    set<string> children = mypkb.getParent(qe1.name);
    set<string>::iterator iterr = children.begin();
    while (iterr != children.end()) {
      vector<string> tuple;
      tuple.push_back(qe1.name);
      tuple.push_back(*iterr);
      result.push_back(tuple);
      iterr++;
    }
  }
  if (isUnderscore(qe1.type) && isConstant(qe2.type)) {
    //("_",constant)
    set<string> parents = mypkb.getParentOf(qe2.name);
    set<string>::iterator iterr = parents.begin();
    while (iterr != parents.end()) {
      vector<string> tuple;
      tuple.push_back(*iterr);
      tuple.push_back(qe2.name);
      result.push_back(tuple);
      iterr++;
    }
  }
  if (isUnderscore(qe1.type) && isSynonym(qe2.type)) {
    //("_",synonym)
    StatementType st = convertQType(qe2.type);
    set<string> children = mypkb.getStatementsOfType(st);
    set<string>::iterator iterr1 = children.begin();
    while (iterr1 != children.end()) {
      set<string> parents = mypkb.getParentOf(*iterr1);
      set<string>::iterator iterr2 = parents.begin();
      while (iterr2 != parents.end()) {
        vector<string> tuple;
        tuple.push_back(*iterr2);
        tuple.push_back(*iterr1);
        result.push_back(tuple);
        iterr2++;
      }
      iterr1++;
    }
  }
  if (isSynonym(qe1.type) && isUnderscore(qe2.type)) {
    //(synonym,"_")
    StatementType st = convertQType(qe1.type);
    set<string> parents = mypkb.getStatementsOfType(st);
    set<string>::iterator iterr1 = parents.begin();
    while (iterr1 != parents.end()) {
      set<string> children = mypkb.getParent(*iterr1);
      set<string>::iterator iterr2 = children.begin();
      while (iterr2 != children.end()) {
        vector<string> tuple;
        tuple.push_back(*iterr1);
        tuple.push_back(*iterr2);
        result.push_back(tuple);
        iterr2++;
      }
      iterr1++;
    }
  }
  if (isUnderscore(qe1.type) && isUnderscore(qe2.type)) {
    //("_","_") wait for pkb to return whole table

  }
  if (!result.empty()) {
    ClauseResult clauseResult(false, false, titles, result);
    return clauseResult;
  } else {
    return ClauseResult(false, false);
  }
}

ClauseResult PqlEvaluator::getParentS(Clause c) {
  vector<QueryEntity>::iterator iter1 = c.parameters.begin();
  vector<QueryEntity>::iterator iter2 = c.parameters.begin();
  iter2++;
  QueryEntity qe1 = *iter1;
  QueryEntity qe2 = *iter2;
  vector<QueryEntity> titles;
  titles.push_back(qe1);
  titles.push_back(qe2);
  vector<vector<string>> result;
  if (isConstant(qe1.type) && isConstant(qe2.type)) {
    //both are constants
    bool bValue = mypkb.parent(qe1.name, qe2.name);
    ClauseResult clauseResult(true, bValue);
    return clauseResult;
  }

  if (isConstant(qe1.type) && isSynonym(qe2.type)) {
    //(constant,synonym)
    set<string> children = mypkb.getParentT(qe1.name);
    set<string>::iterator iterr = children.begin();
    while (iterr != children.end()) {
      if (validateType(*iterr, qe2.type)) {
        vector<string> tuple;
        tuple.push_back(qe1.name);
        tuple.push_back(*iterr);
        result.push_back(tuple);
        iterr++;
      } else {
        iterr++;
        continue;
      }

    }
  }
  if (isSynonym(qe1.type) && isConstant(qe2.type)) {
    //(synonym,constant)
    set<string> parents = mypkb.getParentOfT(qe2.name);
    set<string>::iterator iterr = parents.begin();
    while (iterr != parents.end()) {
      if (validateType(*iterr, qe1.type)) {
        vector<string> tuple;
        tuple.push_back(*iterr);
        tuple.push_back(qe2.name);
        result.push_back(tuple);
        iterr++;
      } else {
        iterr++;
        continue;
      }

    }
  }
  if (isSynonym(qe1.type) && isSynonym(qe2.type)) {
    //(synonym,synonym)
    StatementType st = convertQType(qe1.type);
    set<string> parents = mypkb.getStatementsOfType(st);
    set<string>::iterator iterr1 = parents.begin();
    while (iterr1 != parents.end()) {
      set<string> children = mypkb.getParentT(*iterr1);
      set<string>::iterator iterr2 = children.begin();
      while (iterr2 != children.end()) {
        if (validateType(*iterr2, qe2.type)) {
          vector<string> tuple;
          tuple.push_back(*iterr1);
          tuple.push_back(*iterr2);
          result.push_back(tuple);
          iterr2++;
        } else {
          iterr2++;
          continue;
        }

      }
      iterr1++;
    }
  }
  if (isConstant(qe1.type) && isUnderscore(qe2.type)) {
    //(constant,"_")
    set<string> children = mypkb.getParentT(qe1.name);
    set<string>::iterator iterr = children.begin();
    while (iterr != children.end()) {
      vector<string> tuple;
      tuple.push_back(qe1.name);
      tuple.push_back(*iterr);
      result.push_back(tuple);
      iterr++;
    }
  }
  if (isUnderscore(qe1.type) && isConstant(qe2.type)) {
    //("_",constant)
    set<string> parents = mypkb.getParentOfT(qe2.name);
    set<string>::iterator iterr = parents.begin();
    while (iterr != parents.end()) {
      vector<string> tuple;
      tuple.push_back(*iterr);
      tuple.push_back(qe2.name);
      result.push_back(tuple);
      iterr++;
    }
  }
  if (isUnderscore(qe1.type) && isSynonym(qe2.type)) {
    //("_",synonym)
    StatementType st = convertQType(qe2.type);
    set<string> children = mypkb.getStatementsOfType(st);
    set<string>::iterator iterr1 = children.begin();
    while (iterr1 != children.end()) {
      set<string> parents = mypkb.getParentOfT(*iterr1);
      set<string>::iterator iterr2 = parents.begin();
      while (iterr2 != parents.end()) {
        vector<string> tuple;
        tuple.push_back(*iterr2);
        tuple.push_back(*iterr1);
        result.push_back(tuple);
        iterr2++;
      }
      iterr1++;
    }
  }
  if (isSynonym(qe1.type) && isUnderscore(qe2.type)) {
    //(synonym,"_")
    StatementType st = convertQType(qe1.type);
    set<string> parents = mypkb.getStatementsOfType(st);
    set<string>::iterator iterr1 = parents.begin();
    while (iterr1 != parents.end()) {
      set<string> children = mypkb.getParentT(*iterr1);
      set<string>::iterator iterr2 = children.begin();
      while (iterr2 != children.end()) {
        vector<string> tuple;
        tuple.push_back(*iterr1);
        tuple.push_back(*iterr2);
        result.push_back(tuple);
        iterr2++;
      }
      iterr1++;
    }
  }
  if (isUnderscore(qe1.type) && isUnderscore(qe2.type)) {
    //("_","_") wait for pkb to return whole table

  }
  if (!result.empty()) {
    ClauseResult clauseResult(false, false, titles, result);
    return clauseResult;
  } else {
    return ClauseResult(false, false);
  }
}

ClauseResult PqlEvaluator::getFollows(Clause c) {
  vector<QueryEntity>::iterator iter1 = c.parameters.begin();
  vector<QueryEntity>::iterator iter2 = c.parameters.begin();
  iter2++;
  QueryEntity qe1 = *iter1;
  QueryEntity qe2 = *iter2;
  vector<QueryEntity> titles;
  titles.push_back(qe1);
  titles.push_back(qe2);
  vector<vector<string>> result;
  if (isConstant(qe1.type) && isConstant(qe2.type)) {
    //both are constants
    bool bValue = mypkb.follows(qe1.name, qe2.name);
    ClauseResult clauseResult(true, bValue);
    return clauseResult;
  }

  if (isConstant(qe1.type) && isSynonym(qe2.type)) {
    //(constant,synonym)
    set<string> s2 = mypkb.getFollows(qe1.name);
    set<string>::iterator iterr = s2.begin();
    while (iterr != s2.end()) {
      if (validateType(*iterr, qe2.type)) {
        vector<string> tuple;
        tuple.push_back(qe1.name);
        tuple.push_back(*iterr);
        result.push_back(tuple);
        iterr++;
      } else {
        iterr++;
        continue;
      }

    }
  }
  if (isSynonym(qe1.type) && isConstant(qe2.type)) {
    //(synonym,constant)
    set<string> s1 = mypkb.getFollowedBy(qe2.name);
    set<string>::iterator iterr = s1.begin();
    while (iterr != s1.end()) {
      if (validateType(*iterr, qe1.type)) {
        vector<string> tuple;
        tuple.push_back(*iterr);
        tuple.push_back(qe2.name);
        result.push_back(tuple);
        iterr++;
      } else {
        iterr++;
        continue;
      }

    }
  }
  if (isSynonym(qe1.type) && isSynonym(qe2.type)) {
    //(synonym,synonym)
    StatementType st = convertQType(qe1.type);
    set<string> s1 = mypkb.getStatementsOfType(st);
    set<string>::iterator iterr1 = s1.begin();
    while (iterr1 != s1.end()) {
      set<string> s2 = mypkb.getFollows(*iterr1);
      set<string>::iterator iterr2 = s2.begin();
      while (iterr2 != s2.end()) {
        if (validateType(*iterr2, qe2.type)) {
          vector<string> tuple;
          tuple.push_back(*iterr1);
          tuple.push_back(*iterr2);
          result.push_back(tuple);
          iterr2++;
        } else {
          iterr2++;
          continue;
        }

      }
      iterr1++;
    }
  }
  if (isConstant(qe1.type) && isUnderscore(qe2.type)) {
    //(constant,"_")
    set<string> s2 = mypkb.getFollows(qe1.name);
    set<string>::iterator iterr = s2.begin();
    while (iterr != s2.end()) {
      vector<string> tuple;
      tuple.push_back(qe1.name);
      tuple.push_back(*iterr);
      result.push_back(tuple);
      iterr++;
    }
  }
  if (isUnderscore(qe1.type) && isConstant(qe2.type)) {
    //("_",constant)
    set<string> s1 = mypkb.getFollowedBy(qe2.name);
    set<string>::iterator iterr = s1.begin();
    while (iterr != s1.end()) {
      vector<string> tuple;
      tuple.push_back(*iterr);
      tuple.push_back(qe2.name);
      result.push_back(tuple);
      iterr++;
    }
  }
  if (isUnderscore(qe1.type) && isSynonym(qe2.type)) {
    //("_",synonym)
    StatementType st = convertQType(qe2.type);
    set<string> s2 = mypkb.getStatementsOfType(st);
    set<string>::iterator iterr1 = s2.begin();
    while (iterr1 != s2.end()) {
      set<string> s1 = mypkb.getFollowedBy(*iterr1);
      set<string>::iterator iterr2 = s1.begin();
      while (iterr2 != s1.end()) {
        vector<string> tuple;
        tuple.push_back(*iterr2);
        tuple.push_back(*iterr1);
        result.push_back(tuple);
        iterr2++;
      }
      iterr1++;
    }
  }
  if (isSynonym(qe1.type) && isUnderscore(qe2.type)) {
    //(synonym,"_")
    StatementType st = convertQType(qe1.type);
    set<string> s1 = mypkb.getStatementsOfType(st);
    set<string>::iterator iterr1 = s1.begin();
    while (iterr1 != s1.end()) {
      set<string> s2 = mypkb.getFollows(*iterr1);
      set<string>::iterator iterr2 = s2.begin();
      while (iterr2 != s2.end()) {
        vector<string> tuple;
        tuple.push_back(*iterr1);
        tuple.push_back(*iterr2);
        result.push_back(tuple);
        iterr2++;
      }
      iterr1++;
    }
  }
  if (isUnderscore(qe1.type) && isUnderscore(qe2.type)) {
    //("_","_") wait for pkb to return whole table

  }
  if (!result.empty()) {
    ClauseResult clauseResult(false, false, titles, result);
    return clauseResult;
  } else {
    return ClauseResult(false, false);
  }
}

ClauseResult PqlEvaluator::getFollowsS(Clause c) {
  vector<QueryEntity>::iterator iter1 = c.parameters.begin();
  vector<QueryEntity>::iterator iter2 = c.parameters.begin();
  iter2++;
  QueryEntity qe1 = *iter1;
  QueryEntity qe2 = *iter2;
  vector<QueryEntity> titles;
  titles.push_back(qe1);
  titles.push_back(qe2);
  vector<vector<string>> result;
  if (isConstant(qe1.type) && isConstant(qe2.type)) {
    //both are constants
    bool bValue = mypkb.follows(qe1.name, qe2.name);
    ClauseResult clauseResult(true, bValue);
    return clauseResult;
  }

  if (isConstant(qe1.type) && isSynonym(qe2.type)) {
    //(constant,synonym)
    set<string> s2 = mypkb.getFollowsT(qe1.name);
    set<string>::iterator iterr = s2.begin();
    while (iterr != s2.end()) {
      if (validateType(*iterr, qe2.type)) {
        vector<string> tuple;
        tuple.push_back(qe1.name);
        tuple.push_back(*iterr);
        result.push_back(tuple);
        iterr++;
      } else {
        iterr++;
        continue;
      }

    }
  }
  if (isSynonym(qe1.type) && isConstant(qe2.type)) {
    //(synonym,constant)
    set<string> s1 = mypkb.getFollowedByT(qe2.name);
    set<string>::iterator iterr = s1.begin();
    while (iterr != s1.end()) {
      if (validateType(*iterr, qe1.type)) {
        vector<string> tuple;
        tuple.push_back(*iterr);
        tuple.push_back(qe2.name);
        result.push_back(tuple);
        iterr++;
      } else {
        iterr++;
        continue;
      }

    }
  }
  if (isSynonym(qe1.type) && isSynonym(qe2.type)) {
    //(synonym,synonym)
    StatementType st = convertQType(qe1.type);
    set<string> s1 = mypkb.getStatementsOfType(st);
    set<string>::iterator iterr1 = s1.begin();
    while (iterr1 != s1.end()) {
      set<string> s2 = mypkb.getFollowsT(*iterr1);
      set<string>::iterator iterr2 = s2.begin();
      while (iterr2 != s2.end()) {
        if (validateType(*iterr2, qe2.type)) {
          vector<string> tuple;
          tuple.push_back(*iterr1);
          tuple.push_back(*iterr2);
          result.push_back(tuple);
          iterr2++;
        } else {
          iterr2++;
          continue;
        }

      }
      iterr1++;
    }
  }
  if (isConstant(qe1.type) && isUnderscore(qe2.type)) {
    //(constant,"_")
    set<string> s2 = mypkb.getFollowsT(qe1.name);
    set<string>::iterator iterr = s2.begin();
    while (iterr != s2.end()) {
      vector<string> tuple;
      tuple.push_back(qe1.name);
      tuple.push_back(*iterr);
      result.push_back(tuple);
      iterr++;
    }
  }
  if (isUnderscore(qe1.type) && isConstant(qe2.type)) {
    //("_",constant)
    set<string> s1 = mypkb.getFollowedByT(qe2.name);
    set<string>::iterator iterr = s1.begin();
    while (iterr != s1.end()) {
      vector<string> tuple;
      tuple.push_back(*iterr);
      tuple.push_back(qe2.name);
      result.push_back(tuple);
      iterr++;
    }
  }
  if (isUnderscore(qe1.type) && isSynonym(qe2.type)) {
    //("_",synonym)
    StatementType st = convertQType(qe2.type);
    set<string> s2 = mypkb.getStatementsOfType(st);
    set<string>::iterator iterr1 = s2.begin();
    while (iterr1 != s2.end()) {
      set<string> s1 = mypkb.getFollowedByT(*iterr1);
      set<string>::iterator iterr2 = s1.begin();
      while (iterr2 != s1.end()) {
        vector<string> tuple;
        tuple.push_back(*iterr2);
        tuple.push_back(*iterr1);
        result.push_back(tuple);
        iterr2++;
      }
      iterr1++;
    }
  }
  if (isSynonym(qe1.type) && isUnderscore(qe2.type)) {
    //(synonym,"_")
    StatementType st = convertQType(qe1.type);
    set<string> s1 = mypkb.getStatementsOfType(st);
    set<string>::iterator iterr1 = s1.begin();
    while (iterr1 != s1.end()) {
      set<string> s2 = mypkb.getFollowsT(*iterr1);
      set<string>::iterator iterr2 = s2.begin();
      while (iterr2 != s2.end()) {
        vector<string> tuple;
        tuple.push_back(*iterr1);
        tuple.push_back(*iterr2);
        result.push_back(tuple);
        iterr2++;
      }
      iterr1++;
    }
  }
  if (isUnderscore(qe1.type) && isUnderscore(qe2.type)) {
    //("_","_") wait for pkb to return whole table

  }
  if (!result.empty()) {
    ClauseResult clauseResult(false, false, titles, result);
    return clauseResult;
  } else {
    return ClauseResult(false, false);
  }
}

ClauseResult PqlEvaluator::getAssPatern(Clause c) {
  QueryEntity qe1 = c.parameters[0];
  QueryEntity qe2 = c.parameters[1];
  QueryEntity qe3 = c.parameters[2];
  vector<QueryEntity> titles;
  titles.push_back(qe1);
  titles.push_back(qe2);
  vector<vector<string>> result; 

}

bool PqlEvaluator::isSynonym(QueryEntityType q) {
	//¡®stmt¡¯ | ¡®read¡¯ | ¡®print¡¯ | ¡®call¡¯ |
   //¡®while¡¯ | ¡®if¡¯ | ¡®assign¡¯ | ¡®variable¡¯ | ¡®constant¡¯ |¡®procedure
	return q == QueryEntityType::Stmt || q == QueryEntityType::Read ||
		q == QueryEntityType::Print || q == QueryEntityType::Call ||
		q == QueryEntityType::While || q == QueryEntityType::If ||
		q == QueryEntityType::Assign || q == QueryEntityType::Variable ||
		q == QueryEntityType::Constant || q == QueryEntityType::Procedure;
}

bool PqlEvaluator::isUnderscore(QueryEntityType q) {
  return q == QueryEntityType::Underscore;

}

bool PqlEvaluator::isConstant(QueryEntityType q) {
	return q == QueryEntityType::Expression || q == QueryEntityType::Boolean ||
		q == QueryEntityType::Line || q == QueryEntityType::Name;

}

StatementType PqlEvaluator::convertQType(QueryEntityType q) {
  if (q == QueryEntityType::While) return StatementType::While;
  if (q == QueryEntityType::If) return StatementType::If;
  if (q == QueryEntityType::Read) return StatementType::Read;
  if (q == QueryEntityType::Print) return StatementType::Print;
  if (q == QueryEntityType::Call) return StatementType::Call;
  if (q == QueryEntityType::Assign) return StatementType::Assign;
  if (q == QueryEntityType::Stmt) return StatementType::Stmt;
}

bool PqlEvaluator::validateType(string result, QueryEntityType q) {
  return validateStmt(result, q) || isVar(result, q) || isPro(result, q);
}

bool PqlEvaluator::validateStmt(string result, QueryEntityType q) {
  StatementType st = mypkb.getStatementType(result);
  //Stmt, Assign, If, While, Read, Call, Print
  if (q == QueryEntityType::Stmt) {
    return st == StatementType::While || st == StatementType::If ||
           st == StatementType::Read || st == StatementType::Print ||
           st == StatementType::Call || st == StatementType::Assign ||
           st == StatementType::Stmt;
  } else if (q == QueryEntityType::Assign) return st == StatementType::Assign;
  else if (q == QueryEntityType::If) return st == StatementType::If;
  else if (q == QueryEntityType::While) return st == StatementType::While;
  else if (q == QueryEntityType::Read) return st == StatementType::Read;
  else if (q == QueryEntityType::Call) return st == StatementType::Call;
  else if (q == QueryEntityType::Print) return st == StatementType::Print;
}


bool PqlEvaluator::isCons(string result, QueryEntityType q) { return false; }
bool PqlEvaluator::isVar(string result, QueryEntityType q) { return mypkb.isVar(result) && q==QueryEntityType::Variable; }
bool PqlEvaluator::isPro(string result, QueryEntityType q) { return mypkb.isVar(result) && q == QueryEntityType::Procedure; }
