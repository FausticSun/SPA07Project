#include <PqlEvaluator.h>
#include "PQLParser.h"

PqlEvaluator::PqlEvaluator(const PKB& pkb) { this->mypkb = pkb; }

PqlEvaluator::~PqlEvaluator() = default;

list<string> PqlEvaluator::evaluateQuery(string query) {
  string target;
  list<string> results;
  


  return results;
}

list<string> PqlEvaluator::executeQuery(vector<Clause> &clauses) {
	vector<Clause>::iterator iter = clauses.begin();
    for(iter;iter!=clauses.end();iter++)
    {
      if(iter->clauseType == ClauseType::ModifiesS)
      {
		  getModifies(*iter);
      }
      else if (iter->clauseType == ClauseType::UsesS)
      {
		  getUses(*iter);
      }
      else if (iter->clauseType == ClauseType::Parent)
      {
		  getParent(*iter);
      }
      else if (iter->clauseType == ClauseType::ParentT)
      {
		  getParentS(*iter);
      }
      else if (iter->clauseType == ClauseType::Follows)
      {
		  getFollows(*iter);
      }
      else if (iter->clauseType == ClauseType::FollowsT)
      {
		  getFollowsS(*iter);
      }
      else if (iter->clauseType == ClauseType::AssignPatt)
      {
		  getAssPatern(*iter);
      }

    }
}

ClauseResult PqlEvaluator::getModifies(Clause c)
{
	
	vector<QueryEntity>::iterator iter1 = c.parameters.begin();
	vector<QueryEntity>::iterator iter2 = c.parameters.begin();
	iter2++;
  if( !isSynonym(iter1->type) && !isSynonym(iter2->type))
  {
	  bool bValue = mypkb.uses(iter1->name, iter2->name);
	  vector<QueryEntity> temp{};
          ClauseResult result(true, bValue);
		  return result;
  }

  if(!isSynonym(iter1->type))
  {
    
  }
  else if(!isSynonym(iter2->type))
  {
    
  }

}
ClauseResult PqlEvaluator::getUses(Clause c)
{
  
}
ClauseResult PqlEvaluator::getParent(Clause c)
{
  
}
ClauseResult PqlEvaluator::getParentS(Clause c)
{
  
}
ClauseResult PqlEvaluator::getFollows(Clause c)
{
  
}
ClauseResult PqlEvaluator::getFollowsS(Clause c)
{
  
}
ClauseResult PqlEvaluator::getAssPatern(Clause c)
{
  
}
bool PqlEvaluator::isSynonym(QueryEntityType q)
{
  return q != QueryEntityType::Line && q != QueryEntityType::Name &&
	  q != QueryEntityType::Underscore && q != QueryEntityType::Expression
	  &&q != QueryEntityType::Boolean;
}

//set<string> PqlEvaluator::getData(Clause c)
//{
//  set<string> results;
//  if(c.clauseType==ClauseType::ModifiesS)
//  {
//    
//  }
//  else if(c.clauseType == ClauseType::UsesS)
//  {
//    
//  }
//  else if (c.clauseType == ClauseType::Parent)
//  {
//
//  }
//  else if (c.clauseType == ClauseType::ParentT)
//  {
//
//  }
//  else if (c.clauseType == ClauseType::Follows)
//  {
//
//  }
//  else if (c.clauseType == ClauseType::FollowsT)
//  {
//
//  }
//  else if(c.clauseType == ClauseType::AssignPatt)
//  {
//    
//  }
// 
//}
//
//
