#include "PQLParser.h"

#include <PqlEvaluator.h>
#include<MergeTables.h>

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

string removeUnderscore(string s) {
  return s.substr(1, s.size() - 2);
}

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
  }
  catch (logic_error le) {
    return results;
  }

  return results;
}

list<string> PqlEvaluator::executeQuery(Query &q) {
  vector<Clause> clauses = q.clauses;
  vector<ClauseResult> clauseResults;
  list<string> results;
  if (q.clauses.empty()) {
    results = executeSimpleQuery(q.target.type);
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
    //return simple query
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
        if (finalTable.titles[i].type == q.target.type && finalTable.titles[i].
            name == q.target.name) {
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

list<string> PqlEvaluator::executeSimpleQuery(QueryEntityType q) {

  list<string> results;

  if (q == QueryEntityType::Variable) {
    set<string> vars = mypkb.getVarTable();
    list<string> temp(vars.begin(), vars.end());
    results = temp;
  } else if (q == QueryEntityType::Procedure) {
    set<string> vars = mypkb.getProcTable();
    list<string> temp(vars.begin(), vars.end());
    results = temp;
  } else if (q == QueryEntityType::Constant) {
    /*set<string> cons = mypkb.getConstTable();
    list<string> temp(cons.begin(), cons.end());*/
    return results;
  } else {
    set<string> vars = mypkb.getStatementsOfType(convertQType(q));
    list<string> temp(vars.begin(), vars.end());
    results = temp;
    return results;
  }
  return results;

}

//booleans: cons cons,cons _,_ cons, _ _
//data: s cons, cons s, s s, s _, _ s
ClauseResult PqlEvaluator::getUses(Clause c) {

  vector<QueryEntity>::iterator iter1 = c.parameters.begin();
  vector<QueryEntity>::iterator iter2 = c.parameters.begin();
  iter2++;
  QueryEntity qe1 = *iter1;
  QueryEntity qe2 = *iter2;
  vector<QueryEntity> titles;
  //titles.push_back(qe1);
  //titles.push_back(qe2);
  vector<vector<string>> result;

  if (isConstant(qe1.type) && isConstant(qe2.type)) {
    // both are constants
    bool bValue = mypkb.uses(qe1.name, qe2.name);
    ClauseResult clauseResult(true, bValue);
    return clauseResult;
  }
  if (isConstant(qe1.type) && isSynonym(qe2.type)) {
    //(constant,synonym)
    set<string> used = mypkb.getUses(qe1.name);
    set<string>::iterator iterr = used.begin();
    titles.push_back(qe2);
    while (iterr != used.end()) {

      if (validateType(*iterr, qe2.type)) {
        vector<string> tuple;
        /*tuple.push_back(qe1.name);*/
        tuple.push_back(*iterr);
        result.push_back(tuple);
      }
      iterr++;
    }
  }
  if (isSynonym(qe1.type) && isConstant(qe2.type)) {
    //(synonym,constant)
    set<string> uses = mypkb.getUsedBy(qe2.name);
    set<string>::iterator iterr = uses.begin();
    titles.push_back(qe1);
    while (iterr != uses.end()) {
      if (validateType(*iterr, qe1.type)) {
        vector<string> tuple;
        tuple.push_back(*iterr);
        /*tuple.push_back(qe2.name);*/
        result.push_back(tuple);
      }
      iterr++;
    }
  }
  if (isSynonym(qe1.type) && isSynonym(qe2.type)) {
    //(synonym,synonym)
    set<string> uses = getdataByTtype(qe1.type);
    titles.push_back(qe1);
    titles.push_back(qe2);
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

        }
        iterr2++;
      }
      iterr1++;
    }
  }
  if (isConstant(qe1.type) && isUnderscore(qe2.type)) {
    //(constant,"_") same as (constant,synonmy)
    set<string> used = mypkb.getUses(qe1.name);
    if (used.empty()) {
      return ClauseResult(true, false);
    } else {
      return ClauseResult(true, true);
    }
    /*set<string>::iterator iterr = used.begin();*/

    /*while (iterr != used.end()) {
      vector<string> tuple;
      tuple.push_back(qe1.name);
      tuple.push_back(*iterr);
      result.push_back(tuple);
      iterr++;
    }*/
  }
  if (isSynonym(qe1.type) && isUnderscore(qe2.type)) {
    //(synonym,"_") same as (synonym,synonym)
    set<string> uses = getdataByTtype(qe1.type);
    titles.push_back(qe1);
    set<string>::iterator iterr1 = uses.begin();
    while (iterr1 != uses.end()) {
      set<string> used = mypkb.getUses(*iterr1);
      set<string>::iterator iterr2 = used.begin();
      if (!used.empty()) {
        vector<string> tuple;
        tuple.push_back(*iterr1);
        result.push_back(tuple);
      }
      iterr1++;
    }
  }
  ClauseResult clauseResult(false, false, titles, result);
  return clauseResult;
}

ClauseResult PqlEvaluator::getModifies(Clause c) {
  vector<QueryEntity>::iterator iter1 = c.parameters.begin();
  vector<QueryEntity>::iterator iter2 = c.parameters.begin();
  iter2++;
  QueryEntity qe1 = *iter1;
  QueryEntity qe2 = *iter2;
  vector<QueryEntity> titles;
  //titles.push_back(qe1);
  //titles.push_back(qe2);
  vector<vector<string>> result;
  if (isConstant(qe1.type) && isConstant(qe2.type)) {
    // both are constants
    bool bValue = mypkb.modifies(qe1.name, qe2.name);
    ClauseResult clauseResult(true, bValue);
    return clauseResult;
  }
  if (isConstant(qe1.type) && isSynonym(qe2.type)) {
    //(constant,synonym)
    set<string> modified = mypkb.getModifies(qe1.name);
    titles.push_back(qe2);
    set<string>::iterator iterr = modified.begin();
    while (iterr != modified.end()) {
      if (validateType(*iterr, qe2.type)) {
        vector<string> tuple;
        tuple.push_back(*iterr);
        result.push_back(tuple);
      }
      iterr++;
    }
  }
  if (isSynonym(qe1.type) && isConstant(qe2.type)) {
    //(synonym,constant)
    set<string> modifies = mypkb.getModifiedBy(qe2.name);
    titles.push_back(qe1);
    set<string>::iterator iterr = modifies.begin();
    while (iterr != modifies.end()) {
      if (validateType(*iterr, qe1.type)) {
        vector<string> tuple;
        tuple.push_back(*iterr);
        result.push_back(tuple);
      }
      iterr++;
    }
  }
  if (isSynonym(qe1.type) && isSynonym(qe2.type)) {
    //(synonym,synonym)
    set<string> modifies = getdataByTtype(qe1.type);
    titles.push_back(qe1);
    titles.push_back(qe2);
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
        }
        iterr2++;
      }
      iterr1++;
    }
  }
  if (isConstant(qe1.type) && isUnderscore(qe2.type)) {
    //(constant,"_")
    set<string> modified = mypkb.getModifies(qe1.name);
    if (modified.empty()) {
      return ClauseResult(true, false);
    } else {
      return ClauseResult(true, true);
    }
    /* set<string>::iterator iterr = modified.begin();
     while (iterr != modified.end()) {
       vector<string> tuple;
       tuple.push_back(qe1.name);
       tuple.push_back(*iterr);
       result.push_back(tuple);
       iterr++;
     }*/
  }
  if (isSynonym(qe1.type) && isUnderscore(qe2.type)) {
    //(synonym,"_")
    set<string> modifies = getdataByTtype(qe1.type);
    titles.push_back(qe1);
    set<string>::iterator iterr1 = modifies.begin();
    while (iterr1 != modifies.end()) {
      set<string> modified = mypkb.getModifies(*iterr1);
      set<string>::iterator iterr2 = modified.begin();
      if (!modified.empty()) {
        vector<string> tuple;
        tuple.push_back(*iterr1);
        result.push_back(tuple);
      }
      iterr1++;
    }
  }
  ClauseResult clauseResult(false, false, titles, result);
  return clauseResult;
}

ClauseResult PqlEvaluator::getParent(Clause c) {
  vector<QueryEntity>::iterator iter1 = c.parameters.begin();
  vector<QueryEntity>::iterator iter2 = c.parameters.begin();
  iter2++;
  QueryEntity qe1 = *iter1;
  QueryEntity qe2 = *iter2;
  vector<QueryEntity> titles;
  //titles.push_back(qe1);
  //titles.push_back(qe2);
  vector<vector<string>> result;
  if (isConstant(qe1.type) && isConstant(qe2.type)) {
    // both are constants
    bool bValue = mypkb.parent(qe1.name, qe2.name);
    ClauseResult clauseResult(true, bValue);
    return clauseResult;
  }

  if (isConstant(qe1.type) && isSynonym(qe2.type)) {
    //(constant,synonym)
    set<string> children = mypkb.getParent(qe1.name);
    titles.push_back(qe2);
    set<string>::iterator iterr = children.begin();
    while (iterr != children.end()) {
      if (validateType(*iterr, qe2.type)) {
        vector<string> tuple;
        tuple.push_back(*iterr);
        result.push_back(tuple);
      }
      iterr++;
    }
  }
  if (isSynonym(qe1.type) && isConstant(qe2.type)) {
    //(synonym,constant)
    titles.push_back(qe1);
    set<string> parents = mypkb.getParentOf(qe2.name);
    set<string>::iterator iterr = parents.begin();
    while (iterr != parents.end()) {
      if (validateType(*iterr, qe1.type)) {
        vector<string> tuple;
        tuple.push_back(*iterr);
        result.push_back(tuple);
      }
      iterr++;
    }
  }
  if (isSynonym(qe1.type) && isSynonym(qe2.type)) {
    //(synonym,synonym)
    titles.push_back(qe1);
    titles.push_back(qe2);
    set<string> parents = getdataByTtype(qe1.type);
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
        }
        iterr2++;
      }
      iterr1++;
    }
  }
  if (isConstant(qe1.type) && isUnderscore(qe2.type)) {
    //(constant,"_")
    set<string> children = mypkb.getParent(qe1.name);
    if (children.empty()) {
      return ClauseResult(true, false);
    } else {
      return ClauseResult(true, true);
    }
    /*set<string>::iterator iterr = children.begin();
    while (iterr != children.end()) {
      vector<string> tuple;
      tuple.push_back(qe1.name);
      tuple.push_back(*iterr);
      result.push_back(tuple);
      iterr++;
    }*/
  }
  if (isUnderscore(qe1.type) && isConstant(qe2.type)) {
    //("_",constant)
    set<string> parents = mypkb.getParentOf(qe2.name);
    if (parents.empty()) {
      return ClauseResult(true, false);
    } else {
      return ClauseResult(true, true);
    }
    /* set<string>::iterator iterr = parents.begin();
     while (iterr != parents.end()) {
       vector<string> tuple;
       tuple.push_back(*iterr);
       tuple.push_back(qe2.name);
       result.push_back(tuple);
       iterr++;
     }*/
  }
  if (isUnderscore(qe1.type) && isSynonym(qe2.type)) {
    //("_",synonym)
    titles.push_back(qe2);
    set<string> children = getdataByTtype(qe2.type);
    set<string>::iterator iterr1 = children.begin();
    while (iterr1 != children.end()) {
      set<string> parents = mypkb.getParentOf(*iterr1);
      set<string>::iterator iterr2 = parents.begin();
      if (!parents.empty()) {
        vector<string> tuple;
        tuple.push_back(*iterr1);
        result.push_back(tuple);
      }
      iterr1++;
    }
  }
  if (isSynonym(qe1.type) && isUnderscore(qe2.type)) {
    //(synonym,"_")
    titles.push_back(qe1);
    set<string> parents = getdataByTtype(qe1.type);
    set<string>::iterator iterr1 = parents.begin();
    while (iterr1 != parents.end()) {
      set<string> children = mypkb.getParent(*iterr1);
      if (!children.empty()) {
        vector<string> tuple;
        tuple.push_back(*iterr1);
        result.push_back(tuple);
      }
      iterr1++;
    }
  }
  if (isUnderscore(qe1.type) && isUnderscore(qe2.type)) {
    //("_","_") wait for pkb to return whole table
    result = mypkb.getParentTable();
    if (!result.empty()) {
      return ClauseResult(true, true);
    } else {
      return ClauseResult(true, false);
    }
  }
  ClauseResult clauseResult(false, false, titles, result);
  return clauseResult;
}

ClauseResult PqlEvaluator::getParentS(Clause c) {
  vector<QueryEntity>::iterator iter1 = c.parameters.begin();
  vector<QueryEntity>::iterator iter2 = c.parameters.begin();
  iter2++;
  QueryEntity qe1 = *iter1;
  QueryEntity qe2 = *iter2;
  vector<QueryEntity> titles;
  //titles.push_back(qe1);
  //titles.push_back(qe2);
  vector<vector<string>> result;
  if (isConstant(qe1.type) && isConstant(qe2.type)) {
    // both are constants
    bool bValue = mypkb.parent(qe1.name, qe2.name);
    ClauseResult clauseResult(true, bValue);
    return clauseResult;
  }

  if (isConstant(qe1.type) && isSynonym(qe2.type)) {
    //(constant,synonym)
    titles.push_back(qe2);
    set<string> children = mypkb.getParentT(qe1.name);
    set<string>::iterator iterr = children.begin();
    while (iterr != children.end()) {
      if (validateType(*iterr, qe2.type)) {
        vector<string> tuple;
        tuple.push_back(*iterr);
        result.push_back(tuple);
      }
      iterr++;
    }
  }
  if (isSynonym(qe1.type) && isConstant(qe2.type)) {
    //(synonym,constant)
    titles.push_back(qe1);
    set<string> parents = mypkb.getParentOfT(qe2.name);
    set<string>::iterator iterr = parents.begin();
    while (iterr != parents.end()) {
      if (validateType(*iterr, qe1.type)) {
        vector<string> tuple;
        tuple.push_back(*iterr);
        result.push_back(tuple);
      }
      iterr++;
    }
  }
  if (isSynonym(qe1.type) && isSynonym(qe2.type)) {
    //(synonym,synonym)
    titles.push_back(qe1);
    titles.push_back(qe2);
    set<string> parents = getdataByTtype(qe1.type);
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
        }
        iterr2++;
      }
      iterr1++;
    }
  }
  if (isConstant(qe1.type) && isUnderscore(qe2.type)) {
    //(constant,"_")
    set<string> children = mypkb.getParentT(qe1.name);
    if (children.empty()) {
      return ClauseResult(true, false);
    } else {
      return ClauseResult(true, true);
    }
    /*set<string>::iterator iterr = children.begin();
    while (iterr != children.end()) {
      vector<string> tuple;
      tuple.push_back(qe1.name);
      tuple.push_back(*iterr);
      result.push_back(tuple);
      iterr++;
    }*/
  }
  if (isUnderscore(qe1.type) && isConstant(qe2.type)) {
    //("_",constant)
    set<string> parents = mypkb.getParentOfT(qe2.name);
    if (parents.empty()) {
      return ClauseResult(true, false);
    } else {
      return ClauseResult(true, true);
    }
    /*set<string>::iterator iterr = parents.begin();
    while (iterr != parents.end()) {
      vector<string> tuple;
      tuple.push_back(*iterr);
      tuple.push_back(qe2.name);
      result.push_back(tuple);
      iterr++;
    }*/
  }
  if (isUnderscore(qe1.type) && isSynonym(qe2.type)) {
    //("_",synonym)
    titles.push_back(qe2);
    set<string> children = getdataByTtype(qe2.type);
    set<string>::iterator iterr1 = children.begin();
    while (iterr1 != children.end()) {
      set<string> parents = mypkb.getParentOfT(*iterr1);
      set<string>::iterator iterr2 = parents.begin();
      if (!parents.empty()) {
        vector<string> tuple;
        tuple.push_back(*iterr1);
        result.push_back(tuple);
      }
      iterr1++;
    }
  }
  if (isSynonym(qe1.type) && isUnderscore(qe2.type)) {
    //(synonym,"_")
    titles.push_back(qe1);
    set<string> parents = getdataByTtype(qe1.type);
    set<string>::iterator iterr1 = parents.begin();
    while (iterr1 != parents.end()) {
      set<string> children = mypkb.getParentT(*iterr1);
      set<string>::iterator iterr2 = children.begin();
      if (!children.empty()) {
        vector<string> tuple;
        tuple.push_back(*iterr1);
        result.push_back(tuple);
        iterr2++;
      }
      iterr1++;
    }
  }
  if (isUnderscore(qe1.type) && isUnderscore(qe2.type)) {
    //("_","_") wait for pkb to return whole table
    result = mypkb.getParentTTable();
    if (!result.empty()) {
      return ClauseResult(true, true);
    } else {
      return ClauseResult(true, false);
    }
  }
  ClauseResult clauseResult(false, false, titles, result);
  return clauseResult;
}

ClauseResult PqlEvaluator::getFollows(Clause c) {
  vector<QueryEntity>::iterator iter1 = c.parameters.begin();
  vector<QueryEntity>::iterator iter2 = c.parameters.begin();
  iter2++;
  QueryEntity qe1 = *iter1;
  QueryEntity qe2 = *iter2;
  vector<QueryEntity> titles;
  //titles.push_back(qe1);
  //titles.push_back(qe2);
  vector<vector<string>> result;
  if (isConstant(qe1.type) && isConstant(qe2.type)) {
    // both are constants
    bool bValue = mypkb.follows(qe1.name, qe2.name);
    ClauseResult clauseResult(true, bValue);
    return clauseResult;
  }

  if (isConstant(qe1.type) && isSynonym(qe2.type)) {
    //(constant,synonym)
    titles.push_back(qe2);
    set<string> s2 = mypkb.getFollows(qe1.name);
    set<string>::iterator iterr = s2.begin();
    while (iterr != s2.end()) {
      if (validateType(*iterr, qe2.type)) {
        vector<string> tuple;
        tuple.push_back(*iterr);
        result.push_back(tuple);
      }
      iterr++;
    }
  }
  if (isSynonym(qe1.type) && isConstant(qe2.type)) {
    //(synonym,constant)
    titles.push_back(qe1);
    set<string> s1 = mypkb.getFollowedBy(qe2.name);
    set<string>::iterator iterr = s1.begin();
    while (iterr != s1.end()) {
      if (validateType(*iterr, qe1.type)) {
        vector<string> tuple;
        tuple.push_back(*iterr);
        result.push_back(tuple);
      }
      iterr++;
    }
  }
  if (isSynonym(qe1.type) && isSynonym(qe2.type)) {
    //(synonym,synonym)
    titles.push_back(qe1);
    titles.push_back(qe2);
    set<string> s1 = getdataByTtype(qe1.type);
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
        }
        iterr2++;
      }

      iterr1++;
    }
  }
  if (isConstant(qe1.type) && isUnderscore(qe2.type)) {
    //(constant,"_")
    set<string> s2 = mypkb.getFollows(qe1.name);
    if (s2.empty()) {
      return ClauseResult(true, false);
    } else {
      return ClauseResult(true, true);
    }
    /*set<string>::iterator iterr = s2.begin();
    while (iterr != s2.end()) {
      vector<string> tuple;
      tuple.push_back(qe1.name);
      tuple.push_back(*iterr);
      result.push_back(tuple);
      iterr++;
    }*/
  }
  if (isUnderscore(qe1.type) && isConstant(qe2.type)) {
    //("_",constant)
    set<string> s1 = mypkb.getFollowedBy(qe2.name);
    if (s1.empty()) {
      return ClauseResult(true, false);
    } else {
      return ClauseResult(true, true);
    }
    /*set<string>::iterator iterr = s1.begin();
    while (iterr != s1.end()) {
      vector<string> tuple;
      tuple.push_back(*iterr);
      tuple.push_back(qe2.name);
      result.push_back(tuple);
      iterr++;
    }*/
  }
  if (isUnderscore(qe1.type) && isSynonym(qe2.type)) {
    //("_",synonym)
    titles.push_back(qe2);
    set<string> s2 = getdataByTtype(qe2.type);
    set<string>::iterator iterr1 = s2.begin();
    while (iterr1 != s2.end()) {
      set<string> s1 = mypkb.getFollowedBy(*iterr1);
      set<string>::iterator iterr2 = s1.begin();
      if (!s1.empty()) {
        vector<string> tuple;
        tuple.push_back(*iterr1);
        result.push_back(tuple);
      }
      iterr1++;
    }
  }
  if (isSynonym(qe1.type) && isUnderscore(qe2.type)) {
    //(synonym,"_")
    titles.push_back(qe1);
    set<string> s1 = getdataByTtype(qe1.type);
    set<string>::iterator iterr1 = s1.begin();
    while (iterr1 != s1.end()) {
      set<string> s2 = mypkb.getFollows(*iterr1);
      set<string>::iterator iterr2 = s2.begin();
      if (!s2.empty()) {
        vector<string> tuple;
        tuple.push_back(*iterr1);
        result.push_back(tuple);
      }
      iterr1++;
    }
  }
  if (isUnderscore(qe1.type) && isUnderscore(qe2.type)) {
    //("_","_") wait for pkb to return whole table
    result = mypkb.getFollowsTable();
    if (result.empty()) {
      return ClauseResult(true, false);
    } else {
      return ClauseResult(true, true);
    }
  }

  ClauseResult clauseResult(false, false, titles, result);
  return clauseResult;
}

ClauseResult PqlEvaluator::getFollowsS(Clause c) {
  vector<QueryEntity>::iterator iter1 = c.parameters.begin();
  vector<QueryEntity>::iterator iter2 = c.parameters.begin();
  iter2++;
  QueryEntity qe1 = *iter1;
  QueryEntity qe2 = *iter2;
  vector<QueryEntity> titles;
  //titles.push_back(qe1);
  //titles.push_back(qe2);
  vector<vector<string>> result;
  if (isConstant(qe1.type) && isConstant(qe2.type)) {
    // both are constants
    bool bValue = mypkb.followsT(qe1.name, qe2.name);
    ClauseResult clauseResult(true, bValue);
    return clauseResult;
  }

  if (isConstant(qe1.type) && isSynonym(qe2.type)) {
    //(constant,synonym)
    titles.push_back(qe2);
    set<string> s2 = mypkb.getFollowsT(qe1.name);
    set<string>::iterator iterr = s2.begin();
    while (iterr != s2.end()) {
      if (validateType(*iterr, qe2.type)) {
        vector<string> tuple;
        tuple.push_back(*iterr);
        result.push_back(tuple);
      }
      iterr++;
    }
  }
  if (isSynonym(qe1.type) && isConstant(qe2.type)) {
    //(synonym,constant)
    titles.push_back(qe1);
    set<string> s1 = mypkb.getFollowedByT(qe2.name);
    set<string>::iterator iterr = s1.begin();
    while (iterr != s1.end()) {
      if (validateType(*iterr, qe1.type)) {
        vector<string> tuple;
        tuple.push_back(*iterr);
        result.push_back(tuple);
      }
      iterr++;
    }
  }
  if (isSynonym(qe1.type) && isSynonym(qe2.type)) {
    //(synonym,synonym)
    titles.push_back(qe1);
    titles.push_back(qe2);
    set<string> s1 = getdataByTtype(qe1.type);
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
        }
        iterr2++;
      }
      iterr1++;
    }
  }
  if (isConstant(qe1.type) && isUnderscore(qe2.type)) {
    //(constant,"_")
    set<string> s2 = mypkb.getFollowsT(qe1.name);
    if (s2.empty()) {
      return ClauseResult(true, false);
    } else {
      return ClauseResult(true, true);
    }
    /*set<string>::iterator iterr = s2.begin();
    while (iterr != s2.end()) {
      vector<string> tuple;
      tuple.push_back(qe1.name);
      tuple.push_back(*iterr);
      result.push_back(tuple);
      iterr++;
    }*/
  }
  if (isUnderscore(qe1.type) && isConstant(qe2.type)) {
    //("_",constant)
    set<string> s1 = mypkb.getFollowedByT(qe2.name);
    if (s1.empty()) {
      return ClauseResult(true, false);
    } else {
      return ClauseResult(true, true);
    }
    /* set<string>::iterator iterr = s1.begin();
     while (iterr != s1.end()) {
       vector<string> tuple;
       tuple.push_back(*iterr);
       tuple.push_back(qe2.name);
       result.push_back(tuple);
       iterr++;
     }*/
  }
  if (isUnderscore(qe1.type) && isSynonym(qe2.type)) {
    //("_",synonym)
    titles.push_back(qe2);
    set<string> s2 = getdataByTtype(qe2.type);
    set<string>::iterator iterr1 = s2.begin();
    while (iterr1 != s2.end()) {
      set<string> s1 = mypkb.getFollowedByT(*iterr1);
      set<string>::iterator iterr2 = s1.begin();
      if (!s1.empty()) {
        vector<string> tuple;
        tuple.push_back(*iterr1);
        result.push_back(tuple);
      }
      iterr1++;
    }
  }
  if (isSynonym(qe1.type) && isUnderscore(qe2.type)) {
    //(synonym,"_")
    titles.push_back(qe1);
    set<string> s1 = getdataByTtype(qe1.type);
    set<string>::iterator iterr1 = s1.begin();
    while (iterr1 != s1.end()) {
      set<string> s2 = mypkb.getFollowsT(*iterr1);
      set<string>::iterator iterr2 = s2.begin();
      if (!s2.empty()) {
        vector<string> tuple;
        tuple.push_back(*iterr1);
        result.push_back(tuple);
      }
      iterr1++;
    }
  }
  if (isUnderscore(qe1.type) && isUnderscore(qe2.type)) {
    //("_","_") wait for pkb to return whole table
    result = mypkb.getFollowsTTable();
    if (result.empty()) {
      return ClauseResult(true, false);
    } else {
      return ClauseResult(true, true);
    }
  }
  ClauseResult clauseResult(false, false, titles, result);
  return clauseResult;
}

ClauseResult PqlEvaluator::getAssPatern(Clause c) {
  QueryEntity qe1 = c.parameters[0];
  QueryEntity qe2 = c.parameters[1];
  QueryEntity qe3 = c.parameters[2];
  vector<QueryEntity> titles;
  titles.push_back(qe1);
  //titles.push_back(qe2);
  //titles.push_back(qe3);
  vector<vector<string>> result;

  if (isConstant(qe2.type) && isConstant(qe3.type)) {
    set<string> ass;
    if (isPartial(qe3.name)) {
      ass = mypkb.getAssignMatches(qe2.name, removeUnderscore(qe3.name), true);
    } else {
      ass = mypkb.getAssignMatches(qe2.name, qe3.name, false);
    }
    set<string>::iterator iterr = ass.begin();
    while (iterr != ass.end()) {
      vector<string> tuple;
      tuple.push_back(*iterr);
      result.push_back(tuple);
      iterr++;
    }
  }

  if (isSynonym(qe2.type) && isConstant(qe3.type)) {
    titles.push_back(qe2);
    set<string> var = getdataByTtype(qe2.type);
    set<string>::iterator iterr1 = var.begin();
    if (isPartial(qe3.name)) {
      set<string> ass;
      while (iterr1 != var.end()) {
        ass = mypkb.getAssignMatches(*iterr1, removeUnderscore(qe3.name), true);
        set<string>::iterator iterr2 = ass.begin();

        while (iterr2 != ass.end()) {
          vector<string> tuple;
          tuple.push_back(*iterr2);
          tuple.push_back(*iterr1);
          result.push_back(tuple);
          iterr2++;
        }
        iterr1++;
      }
    } else {
      set<string> ass;
      while (iterr1 != var.end()) {
        ass = mypkb.getAssignMatches(*iterr1, qe3.name, false);
        set<string>::iterator iterr2 = ass.begin();

        while (iterr2 != ass.end()) {
          vector<string> tuple;
          tuple.push_back(*iterr2);
          tuple.push_back(*iterr1);
          result.push_back(tuple);
          iterr2++;
        }
        iterr1++;
      }
    }

  }

  if (isSynonym(qe2.type) && isUnderscore(qe3.type)) {
    titles.push_back(qe2);
    set<string> var = getdataByTtype(qe2.type);
    set<string>::iterator iterr1 = var.begin();
    while (iterr1 != var.end()) {
      set<string> ass = mypkb.getAssignMatches(*iterr1, "", true);
      set<string>::iterator iterr2 = ass.begin();

      while (iterr2 != ass.end()) {
        vector<string> tuple;
        tuple.push_back(*iterr2);
        tuple.push_back(*iterr1);
        result.push_back(tuple);
        iterr2++;
      }
      iterr1++;

    }
  }
  if (isConstant(qe2.type) && isUnderscore(qe3.type)) {
    set<string> ass = mypkb.getAssignMatches(qe2.name, "", true);
    set<string>::iterator iterr = ass.begin();
    while (iterr != ass.end()) {
      vector<string> tuple;
      tuple.push_back(*iterr);
      result.push_back(tuple);
      iterr++;
    }
  }
  if (isUnderscore(qe2.type) && isUnderscore(qe3.type)) {
    set<string> ass = mypkb.getAssignMatches("", "", true);
    set<string>::iterator iterr = ass.begin();
    while (iterr != ass.end()) {
      vector<string> tuple;
      tuple.push_back(*iterr);
      result.push_back(tuple);
      iterr++;
    }
  }
  if (isUnderscore(qe2.type) && isConstant(qe3.type)) {
    set<string> ass;
    if (isPartial(qe3.name)) {
      ass = mypkb.getAssignMatches("", removeUnderscore(qe3.name), true);
    } else {
      ass = mypkb.getAssignMatches("", qe3.name, false);
    }
    set<string>::iterator iterr = ass.begin();
    while (iterr != ass.end()) {
      vector<string> tuple;
      tuple.push_back(*iterr);
      result.push_back(tuple);
      iterr++;
    }
  }
  ClauseResult clauseResult(false, false, titles, result);
  return clauseResult;

}

set<string> PqlEvaluator::getdataByTtype(QueryEntityType q) {
  if (q == QueryEntityType::Variable) {
    return mypkb.getVarTable();
  } else if (q == QueryEntityType::Procedure) {
    return mypkb.getProcTable();
  } else {
    StatementType st = convertQType(q);
    return mypkb.getStatementsOfType(st);
  }
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

bool PqlEvaluator::validateType(string result, QueryEntityType q) {
  return validateStmt(result, q) || isVar(result, q) || isPro(result, q);
}

bool PqlEvaluator::validateStmt(string result, QueryEntityType q) {
  StatementType st = mypkb.getStatementType(result);
  // Stmt, Assign, If, While, Read, Call, Print
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

bool PqlEvaluator::isVar(string result, QueryEntityType q) {
  return /*mypkb.isVar(result) &&*/ q == QueryEntityType::Variable;
}

bool PqlEvaluator::isPro(string result, QueryEntityType q) {
  return /*mypkb.isProc(result) &&*/ q == QueryEntityType::Procedure;
}
