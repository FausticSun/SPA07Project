#include <PqlEvaluator.h>
#include "PQLParser.h"

PqlEvaluator::PqlEvaluator(const PKB &pkb) { this->mypkb = pkb; }

PqlEvaluator::~PqlEvaluator() = default;

list<string> PqlEvaluator::evaluateQuery(string query) {
  string target;
  list<string> results;

  return results;
}

list<string> PqlEvaluator::executeQuery(vector<Clause> &clauses) {
  vector<Clause>::iterator iter = clauses.begin();
  for (iter; iter != clauses.end(); iter++) {
    if (iter->clauseType == ClauseType::ModifiesS) {
      getModifies(*iter);
    } else if (iter->clauseType == ClauseType::UsesS) {
      getUses(*iter);
    } else if (iter->clauseType == ClauseType::Parent) {
      getParent(*iter);
    } else if (iter->clauseType == ClauseType::ParentT) {
      getParentS(*iter);
    } else if (iter->clauseType == ClauseType::Follows) {
      getFollows(*iter);
    } else if (iter->clauseType == ClauseType::FollowsT) {
      getFollowsS(*iter);
    } else if (iter->clauseType == ClauseType::AssignPatt) {
      getAssPatern(*iter);
    }

  }
  return list<string> {};
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

  if(isConstant(qe1.type) && isConstant(qe2.type) )
  {
    //both are constants
	  bool bValue = mypkb.uses(qe1.name, qe2.name);
	  ClauseResult clauseResult(true, bValue);
	  return clauseResult;
  }
  if(isConstant(qe1.type) && isSynonym(qe2.type))
  {
    //(constant,synonym)
	  set<string> used = mypkb.getUses(qe1.name);
	  set<string>::iterator iterr = used.begin();
	  while (iterr != used.end())
	  {
		  vector<string> tuple;
		  tuple.push_back(qe1.name);
		  tuple.push_back(*iterr);
		  result.push_back(tuple);
		  iterr++;
	  }
  }
  if (isSynonym(qe1.type) && isConstant(qe2.type))
  {
	  //(synonym,constant)
	  set<string> uses = mypkb.getUsedBy(qe2.name);
	  set<string>::iterator iterr = uses.begin();
	  while (iterr != uses.end())
	  {
		  vector<string> tuple;
		  tuple.push_back(*iterr);
		  tuple.push_back(qe2.name);
		  result.push_back(tuple);
		  iterr++;
	  }
  }
  if (isSynonym(qe1.type) && isSynonym(qe2.type))
  {
	  //(synonym,synonym)
	  StatementType st = convertQType(qe1.type);
	  set<string>uses = mypkb.getStatementsOfType(st);
	  set<string>::iterator iterr1 = uses.begin();
	  while (iterr1 != uses.end())
	  {
		  set<string> used = mypkb.getUses(*iterr1);
		  set<string>::iterator iterr2 = used.begin();
		  while (iterr2 != used.end())
		  {
			  vector<string> tuple;
			  tuple.push_back(*iterr1);
			  tuple.push_back(*iterr2);
			  result.push_back(tuple);
			  iterr2++;
		  }
		  iterr1++;
	  }
  }
  if (isConstant(qe1.type) && isUnderscore(qe2.type))
  {
	  //(constant,"_") same as (constant,synonmy)
	  set<string> used = mypkb.getUses(qe1.name);
	  set<string>::iterator iterr = used.begin();
	  while (iterr != used.end())
	  {
		  vector<string> tuple;
		  tuple.push_back(qe1.name);
		  tuple.push_back(*iterr);
		  result.push_back(tuple);
		  iterr++;
	  }
  }
  if (isSynonym(qe1.type) && isUnderscore(qe2.type))
  {
	  //(synonym,"_") same as (synonym,synonym)
	  StatementType st = convertQType(qe1.type);
	  set<string>uses = mypkb.getStatementsOfType(st);
	  set<string>::iterator iterr1 = uses.begin();
	  while (iterr1 != uses.end())
	  {
		  set<string> used = mypkb.getUses(*iterr1);
		  set<string>::iterator iterr2 = used.begin();
		  while (iterr2 != used.end())
		  {
			  vector<string> tuple;
			  tuple.push_back(*iterr1);
			  tuple.push_back(*iterr2);
			  result.push_back(tuple);
			  iterr2++;
		  }
		  iterr1++;
	  }
  }
  if (!result.empty())
  {
	  ClauseResult clauseResult(false, false, titles, result);
	  return clauseResult;
  }
  else
  {
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
	if (isConstant(qe1.type) && isConstant(qe2.type))
	{
		//both are constants
		bool bValue = mypkb.modifies(qe1.name, qe2.name);
		ClauseResult clauseResult(true, bValue);
		return clauseResult;

	}
	if (isConstant(qe1.type) && isSynonym(qe2.type))
	{
		//(constant,synonym)
		set<string> modified = mypkb.getModifies(qe1.name);
		set<string>::iterator iterr = modified.begin();
          while(iterr!=modified.end())
          {
			  vector<string> tuple;
			  tuple.push_back(qe1.name);
			  tuple.push_back(*iterr);
			  result.push_back(tuple);
			  iterr++;
          }
	}
	if (isSynonym(qe1.type) && isConstant(qe2.type))
	{
		//(synonym,constant)
		set<string> modifies = mypkb.getModifiedBy(qe1.name);
		set<string>::iterator iterr = modifies.begin();
		while (iterr != modifies.end())
		{
			vector<string> tuple;
			tuple.push_back(qe1.name);
			tuple.push_back(*iterr);
			result.push_back(tuple);
			iterr++;
		}
	}
	if (isSynonym(qe1.type) && isSynonym(qe2.type))
	{
		//(synonym,synonym)
		StatementType st = convertQType(qe1.type);
		set<string> modifies = mypkb.getStatementsOfType(st);
		set<string>::iterator iterr1 = modifies.begin();
		while (iterr1 != modifies.end())
		{
			set<string> modified = mypkb.getModifies(*iterr1);
			set<string>::iterator iterr2 = modified.begin();
			while (iterr2 != modified.end())
			{
				vector<string> tuple;
				tuple.push_back(*iterr1);
				tuple.push_back(*iterr2);
				result.push_back(tuple);
				iterr2++;
			}
			iterr1++;
		}
	}
	if (isConstant(qe1.type) && isUnderscore(qe2.type))
	{
		//(constant,"_")
		set<string> modified = mypkb.getModifies(qe1.name);
		set<string>::iterator iterr = modified.begin();
		while (iterr != modified.end())
		{
			vector<string> tuple;
			tuple.push_back(qe1.name);
			tuple.push_back(*iterr);
			result.push_back(tuple);
			iterr++;
		}
	}
	if (isSynonym(qe1.type) && isUnderscore(qe2.type))
	{
		//(synonym,"_")
		StatementType st = convertQType(qe1.type);
		set<string> modifies = mypkb.getStatementsOfType(st);
		set<string>::iterator iterr1 = modifies.begin();
		while (iterr1 != modifies.end())
		{
			set<string> modified = mypkb.getModifies(*iterr1);
			set<string>::iterator iterr2 = modified.begin();
			while (iterr2 != modified.end())
			{
				vector<string> tuple;
				tuple.push_back(*iterr1);
				tuple.push_back(*iterr2);
				result.push_back(tuple);
				iterr2++;
			}
			iterr1++;
		}
	}
	if (!result.empty())
	{
		ClauseResult clauseResult(false, false, titles, result);
		return clauseResult;
	}
	else
	{
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
	if (isConstant(qe1.type) && isConstant(qe2.type))
	{
		//both are constants
		bool bValue = mypkb.parent(qe1.name, qe2.name);
		ClauseResult clauseResult(true, bValue);
		return clauseResult;
	}

	if (isConstant(qe1.type) && isSynonym(qe2.type))
	{
		//(constant,synonym)
		set<string> children = mypkb.getParent(qe1.name);
		set<string>::iterator iterr = children.begin();
		while (iterr != children.end())
		{
			vector<string> tuple;
			tuple.push_back(qe1.name);
			tuple.push_back(*iterr);
			result.push_back(tuple);
			iterr++;
		}
	}
	if (isSynonym(qe1.type) && isConstant(qe2.type))
	{
		//(synonym,constant)
		set<string> parents = mypkb.getParentOf(qe2.name);
		set<string>::iterator iterr = parents.begin();
		while (iterr != parents.end())
		{
			vector<string> tuple;
			tuple.push_back(*iterr);
			tuple.push_back(qe2.name);
			result.push_back(tuple);
			iterr++;
		}
	}
	if (isSynonym(qe1.type) && isSynonym(qe2.type))
	{
		//(synonym,synonym)
		StatementType st = convertQType(qe1.type);
		set<string>parents = mypkb.getStatementsOfType(st);
		set<string>::iterator iterr1 = parents.begin();
		while (iterr1 != parents.end())
		{
			set<string> children = mypkb.getParent(*iterr1);
			set<string>::iterator iterr2 = children.begin();
			while (iterr2 != children.end())
			{
				vector<string> tuple;
				tuple.push_back(*iterr1);
				tuple.push_back(*iterr2);
				result.push_back(tuple);
				iterr2++;
			}
			iterr1++;
		}
	}
	if (isConstant(qe1.type) && isUnderscore(qe2.type))
	{
		//(constant,"_")
		set<string> children = mypkb.getParent(qe1.name);
		set<string>::iterator iterr = children.begin();
		while (iterr != children.end())
		{
			vector<string> tuple;
			tuple.push_back(qe1.name);
			tuple.push_back(*iterr);
			result.push_back(tuple);
			iterr++;
		}
	}
	if (isUnderscore(qe1.type) && isConstant(qe2.type))
	{
		//("_",constant)
		set<string> parents = mypkb.getParentOf(qe2.name);
		set<string>::iterator iterr = parents.begin();
		while (iterr != parents.end())
		{
			vector<string> tuple;
			tuple.push_back(*iterr);
			tuple.push_back(qe2.name);
			result.push_back(tuple);
			iterr++;
		}
	}
	if (isUnderscore(qe1.type) && isSynonym(qe2.type))
	{
		//("_",synonym)
		StatementType st = convertQType(qe2.type);
		set<string>children = mypkb.getStatementsOfType(st);
		set<string>::iterator iterr1 = children.begin();
		while (iterr1 != children.end())
		{
			set<string> parents = mypkb.getParentOf(*iterr1);
			set<string>::iterator iterr2 = parents.begin();
			while (iterr2 != parents.end())
			{
				vector<string> tuple;
			        tuple.push_back(*iterr2);
				tuple.push_back(*iterr1);
				result.push_back(tuple);
				iterr2++;
			}
			iterr1++;
		}
	}
	if (isSynonym(qe1.type) && isUnderscore(qe2.type))
	{
		//(synonym,"_")
		StatementType st = convertQType(qe1.type);
		set<string>parents = mypkb.getStatementsOfType(st);
		set<string>::iterator iterr1 = parents.begin();
		while (iterr1 != parents.end())
		{
			set<string> children = mypkb.getParent(*iterr1);
			set<string>::iterator iterr2 = children.begin();
			while (iterr2 != children.end())
			{
				vector<string> tuple;
				tuple.push_back(*iterr1);
				tuple.push_back(*iterr2);
				result.push_back(tuple);
				iterr2++;
			}
			iterr1++;
		}
	}
	if (isUnderscore(qe1.type) && isUnderscore(qe2.type))
	{
		//("_","_") wait for pkb to return whole table

	}
	if (!result.empty())
	{
		ClauseResult clauseResult(false, false, titles, result);
		return clauseResult;
	}
	else
	{
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
	if (isConstant(qe1.type) && isConstant(qe2.type))
	{
		//both are constants
		bool bValue = mypkb.parent(qe1.name, qe2.name);
		ClauseResult clauseResult(true, bValue);
		return clauseResult;
	}

	if (isConstant(qe1.type) && isSynonym(qe2.type))
	{
		//(constant,synonym)
		set<string> children = mypkb.getParentT(qe1.name);
		set<string>::iterator iterr = children.begin();
		while (iterr != children.end())
		{
			vector<string> tuple;
			tuple.push_back(qe1.name);
			tuple.push_back(*iterr);
			result.push_back(tuple);
			iterr++;
		}
	}
	if (isSynonym(qe1.type) && isConstant(qe2.type))
	{
		//(synonym,constant)
		set<string> parents = mypkb.getParentOfT(qe2.name);
		set<string>::iterator iterr = parents.begin();
		while (iterr != parents.end())
		{
			vector<string> tuple;
			tuple.push_back(*iterr);
			tuple.push_back(qe2.name);
			result.push_back(tuple);
			iterr++;
		}
	}
	if (isSynonym(qe1.type) && isSynonym(qe2.type))
	{
		//(synonym,synonym)
		StatementType st = convertQType(qe1.type);
		set<string>parents = mypkb.getStatementsOfType(st);
		set<string>::iterator iterr1 = parents.begin();
		while (iterr1 != parents.end())
		{
			set<string> children = mypkb.getParentT(*iterr1);
			set<string>::iterator iterr2 = children.begin();
			while (iterr2 != children.end())
			{
				vector<string> tuple;
				tuple.push_back(*iterr1);
				tuple.push_back(*iterr2);
				result.push_back(tuple);
				iterr2++;
			}
			iterr1++;
		}
	}
	if (isConstant(qe1.type) && isUnderscore(qe2.type))
	{
		//(constant,"_")
		set<string> children = mypkb.getParentT(qe1.name);
		set<string>::iterator iterr = children.begin();
		while (iterr != children.end())
		{
			vector<string> tuple;
			tuple.push_back(qe1.name);
			tuple.push_back(*iterr);
			result.push_back(tuple);
			iterr++;
		}
	}
	if (isUnderscore(qe1.type) && isConstant(qe2.type))
	{
		//("_",constant)
		set<string> parents = mypkb.getParentOfT(qe2.name);
		set<string>::iterator iterr = parents.begin();
		while (iterr != parents.end())
		{
			vector<string> tuple;
			tuple.push_back(*iterr);
			tuple.push_back(qe2.name);
			result.push_back(tuple);
			iterr++;
		}
	}
	if (isUnderscore(qe1.type) && isSynonym(qe2.type))
	{
		//("_",synonym)
		StatementType st = convertQType(qe2.type);
		set<string>children = mypkb.getStatementsOfType(st);
		set<string>::iterator iterr1 = children.begin();
		while (iterr1 != children.end())
		{
			set<string> parents = mypkb.getParentOfT(*iterr1);
			set<string>::iterator iterr2 = parents.begin();
			while (iterr2 != parents.end())
			{
				vector<string> tuple;
				tuple.push_back(*iterr2);
				tuple.push_back(*iterr1);
				result.push_back(tuple);
				iterr2++;
			}
			iterr1++;
		}
	}
	if (isSynonym(qe1.type) && isUnderscore(qe2.type))
	{
		//(synonym,"_")
		StatementType st = convertQType(qe1.type);
		set<string>parents = mypkb.getStatementsOfType(st);
		set<string>::iterator iterr1 = parents.begin();
		while (iterr1 != parents.end())
		{
			set<string> children = mypkb.getParentT(*iterr1);
			set<string>::iterator iterr2 = children.begin();
			while (iterr2 != children.end())
			{
				vector<string> tuple;
				tuple.push_back(*iterr1);
				tuple.push_back(*iterr2);
				result.push_back(tuple);
				iterr2++;
			}
			iterr1++;
		}
	}
	if (isUnderscore(qe1.type) && isUnderscore(qe2.type))
	{
		//("_","_") wait for pkb to return whole table

	}
	if (!result.empty())
	{
		ClauseResult clauseResult(false, false, titles, result);
		return clauseResult;
	}
	else
	{
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
	if (isConstant(qe1.type) && isConstant(qe2.type))
	{
		//both are constants
		bool bValue = mypkb.follows(qe1.name, qe2.name);
		ClauseResult clauseResult(true, bValue);
		return clauseResult;
	}

	if (isConstant(qe1.type) && isSynonym(qe2.type))
	{
		//(constant,synonym)
		set<string> s2 = mypkb.getFollows(qe1.name);
		set<string>::iterator iterr = s2.begin();
		while (iterr != s2.end())
		{
			vector<string> tuple;
			tuple.push_back(qe1.name);
			tuple.push_back(*iterr);
			result.push_back(tuple);
			iterr++;
		}
	}
	if (isSynonym(qe1.type) && isConstant(qe2.type))
	{
		//(synonym,constant)
		set<string> s1 = mypkb.getFollowedBy(qe2.name);
		set<string>::iterator iterr = s1.begin();
		while (iterr != s1.end())
		{
			vector<string> tuple;
			tuple.push_back(*iterr);
			tuple.push_back(qe2.name);
			result.push_back(tuple);
			iterr++;
		}
	}
	if (isSynonym(qe1.type) && isSynonym(qe2.type))
	{
		//(synonym,synonym)
		StatementType st = convertQType(qe1.type);
		set<string>s1 = mypkb.getStatementsOfType(st);
		set<string>::iterator iterr1 = s1.begin();
		while (iterr1 != s1.end())
		{
			set<string> s2 = mypkb.getFollows(*iterr1);
			set<string>::iterator iterr2 = s2.begin();
			while (iterr2 != s2.end())
			{
				vector<string> tuple;
				tuple.push_back(*iterr1);
				tuple.push_back(*iterr2);
				result.push_back(tuple);
				iterr2++;
			}
			iterr1++;
		}
	}
	if (isConstant(qe1.type) && isUnderscore(qe2.type))
	{
		//(constant,"_")
		set<string> s2 = mypkb.getFollows(qe1.name);
		set<string>::iterator iterr = s2.begin();
		while (iterr != s2.end())
		{
			vector<string> tuple;
			tuple.push_back(qe1.name);
			tuple.push_back(*iterr);
			result.push_back(tuple);
			iterr++;
		}
	}
	if (isUnderscore(qe1.type) && isConstant(qe2.type))
	{
		//("_",constant)
		set<string> s1 = mypkb.getFollowedBy(qe2.name);
		set<string>::iterator iterr = s1.begin();
		while (iterr != s1.end())
		{
			vector<string> tuple;
			tuple.push_back(*iterr);
			tuple.push_back(qe2.name);
			result.push_back(tuple);
			iterr++;
		}
	}
	if (isUnderscore(qe1.type) && isSynonym(qe2.type))
	{
		//("_",synonym)
		StatementType st = convertQType(qe2.type);
		set<string>s2 = mypkb.getStatementsOfType(st);
		set<string>::iterator iterr1 = s2.begin();
		while (iterr1 != s2.end())
		{
			set<string> s1 = mypkb.getFollowedBy(*iterr1);
			set<string>::iterator iterr2 = s1.begin();
			while (iterr2 != s1.end())
			{
				vector<string> tuple;
				tuple.push_back(*iterr2);
				tuple.push_back(*iterr1);
				result.push_back(tuple);
				iterr2++;
			}
			iterr1++;
		}
	}
	if (isSynonym(qe1.type) && isUnderscore(qe2.type))
	{
		//(synonym,"_")
		StatementType st = convertQType(qe1.type);
		set<string>s1 = mypkb.getStatementsOfType(st);
		set<string>::iterator iterr1 = s1.begin();
		while (iterr1 != s1.end())
		{
			set<string> s2 = mypkb.getFollows(*iterr1);
			set<string>::iterator iterr2 = s2.begin();
			while (iterr2 != s2.end())
			{
				vector<string> tuple;
				tuple.push_back(*iterr1);
				tuple.push_back(*iterr2);
				result.push_back(tuple);
				iterr2++;
			}
			iterr1++;
		}
	}
	if (isUnderscore(qe1.type) && isUnderscore(qe2.type))
	{
		//("_","_") wait for pkb to return whole table

	}
	if (!result.empty())
	{
		ClauseResult clauseResult(false, false, titles, result);
		return clauseResult;
	}
	else
	{
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
	if (isConstant(qe1.type) && isConstant(qe2.type))
	{
		//both are constants
		bool bValue = mypkb.follows(qe1.name, qe2.name);
		ClauseResult clauseResult(true, bValue);
		return clauseResult;
	}

	if (isConstant(qe1.type) && isSynonym(qe2.type))
	{
		//(constant,synonym)
		set<string> s2 = mypkb.getFollowsT(qe1.name);
		set<string>::iterator iterr = s2.begin();
		while (iterr != s2.end())
		{
			vector<string> tuple;
			tuple.push_back(qe1.name);
			tuple.push_back(*iterr);
			result.push_back(tuple);
			iterr++;
		}
		if (!result.empty())
		{
			ClauseResult clauseResult(false, false, titles, result);
			return clauseResult;
		}
		else
		{
			return ClauseResult(false, false);
		}

	}
	if (isSynonym(qe1.type) && isConstant(qe2.type))
	{
		//(synonym,constant)
		set<string> s1 = mypkb.getFollowedByT(qe2.name);
		set<string>::iterator iterr = s1.begin();
		while (iterr != s1.end()) 
		{
			vector<string> tuple;
			tuple.push_back(*iterr);
			tuple.push_back(qe2.name);
			result.push_back(tuple);
			iterr++;
		}
	}
	if (isSynonym(qe1.type) && isSynonym(qe2.type))
	{
		//(synonym,synonym)
		StatementType st = convertQType(qe1.type);
		set<string>s1 = mypkb.getStatementsOfType(st);
		set<string>::iterator iterr1 = s1.begin();
		while (iterr1 != s1.end())
		{
			set<string> s2 = mypkb.getFollowsT(*iterr1);
			set<string>::iterator iterr2 = s2.begin();
			while (iterr2 != s2.end())
			{
				vector<string> tuple;
				tuple.push_back(*iterr1);
				tuple.push_back(*iterr2);
				result.push_back(tuple);
				iterr2++;
			}
			iterr1++;
		}
	}
	if (isConstant(qe1.type) && isUnderscore(qe2.type))
	{
		//(constant,"_")
		set<string> s2 = mypkb.getFollowsT(qe1.name);
		set<string>::iterator iterr = s2.begin();
		while (iterr != s2.end())
		{
			vector<string> tuple;
			tuple.push_back(qe1.name);
			tuple.push_back(*iterr);
			result.push_back(tuple);
			iterr++;
		}
	}
	if (isUnderscore(qe1.type) && isConstant(qe2.type))
	{
		//("_",constant)
		set<string> s1 = mypkb.getFollowedByT(qe2.name);
		set<string>::iterator iterr = s1.begin();
		while (iterr != s1.end())
		{
			vector<string> tuple;
			tuple.push_back(*iterr);
			tuple.push_back(qe2.name);
			result.push_back(tuple);
			iterr++;
		}
	}
	if (isUnderscore(qe1.type) && isSynonym(qe2.type))
	{
		//("_",synonym)
		StatementType st = convertQType(qe2.type);
		set<string>s2 = mypkb.getStatementsOfType(st);
		set<string>::iterator iterr1 = s2.begin();
		while (iterr1 != s2.end())
		{
			set<string> s1 = mypkb.getFollowedByT(*iterr1);
			set<string>::iterator iterr2 = s1.begin();
			while (iterr2 != s1.end())
			{
				vector<string> tuple;
				tuple.push_back(*iterr2);
				tuple.push_back(*iterr1);
				result.push_back(tuple);
				iterr2++;
			}
			iterr1++;
		}
	}
	if (isSynonym(qe1.type) && isUnderscore(qe2.type))
	{
		//(synonym,"_")
		StatementType st = convertQType(qe1.type);
		set<string>s1 = mypkb.getStatementsOfType(st);
		set<string>::iterator iterr1 = s1.begin();
		while (iterr1 != s1.end())
		{
			set<string> s2 = mypkb.getFollowsT(*iterr1);
			set<string>::iterator iterr2 = s2.begin();
			while (iterr2 != s2.end())
			{
				vector<string> tuple;
				tuple.push_back(*iterr1);
				tuple.push_back(*iterr2);
				result.push_back(tuple);
				iterr2++;
			}
			iterr1++;
		}
	}
	if (isUnderscore(qe1.type) && isUnderscore(qe2.type))
	{
		//("_","_") wait for pkb to return whole table

	}
	if (!result.empty())
	{
		ClauseResult clauseResult(false, false, titles, result);
		return clauseResult;
	}
	else
	{
		return ClauseResult(false, false);
	}
}

ClauseResult PqlEvaluator::getAssPatern(Clause c) {
	vector<QueryEntity>::iterator iter1 = c.parameters.begin();
	vector<QueryEntity>::iterator iter2 = c.parameters.begin();
	iter2++;
	QueryEntity qe1 = *iter1;
	QueryEntity qe2 = *iter2;
	if (isConstant(qe1.type) && isConstant(qe2.type))
	{
		//both are constants

	}

	if (isConstant(qe1.type) && isSynonym(qe2.type))
	{
		//(constant,synonym)

	}
	if (isSynonym(qe1.type) && isConstant(qe2.type))
	{
		//(synonym,constant)

	}
	if (isSynonym(qe1.type) && isSynonym(qe2.type))
	{
		//(synonym,synonym)

	}
	if (isConstant(qe1.type) && isUnderscore(qe2.type))
	{
		//(constant,"_")

	}
	if (isUnderscore(qe1.type) && isConstant(qe2.type))
	{
		//("_",constant)

	}
	if (isUnderscore(qe1.type) && isSynonym(qe2.type))
	{
		//("_",synonym)

	}
	if (isSynonym(qe1.type) && isUnderscore(qe2.type))
	{
		//(synonym,"_")

	}
	if (isUnderscore(qe1.type) && isUnderscore(qe2.type))
	{
		//("_","_")

	}
	return ClauseResult(false, false);
}

bool PqlEvaluator::isSynonym(QueryEntityType q) {
  return q != QueryEntityType::Line && q != QueryEntityType::Name &&
         q != QueryEntityType::Expression && q != QueryEntityType::Boolean;
}

bool PqlEvaluator::isUnderscore(QueryEntityType q)
{
	return q == QueryEntityType::Underscore;


}
bool PqlEvaluator::isConstant(QueryEntityType q)
{
	return !isSynonym(q) && !isUnderscore(q);


}

StatementType PqlEvaluator::convertQType(QueryEntityType q)
  {
	if (q == QueryEntityType::While) return StatementType::While;
	if (q == QueryEntityType::If) return StatementType::If;
	if (q == QueryEntityType::Read) return StatementType::Read;
	if (q == QueryEntityType::Print) return StatementType::Print;
	if (q == QueryEntityType::Call) return StatementType::Call;
	if (q == QueryEntityType::Assign) return StatementType::Assign;
	if (q == QueryEntityType::Stmt) return StatementType::Stmt;
  }
