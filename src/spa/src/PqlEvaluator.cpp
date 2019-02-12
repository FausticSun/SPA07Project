#include <PqlEvaluator.h>
#include "PQLParser.h"

PqlEvaluator::PqlEvaluator(const PKB& pkb) { this->mypkb = pkb; }

PqlEvaluator::~PqlEvaluator() = default;

list<string> PqlEvaluator::evaluateQuery(string query) {
  string target;
  list<string> results;
  


  return results;
}

set<string> PqlEvaluator::getData(Clause c)
{
  set<string> results;
  if(c.clauseType==ClauseType::ModifiesS)
  {
    
  }
  else if(c.clauseType == ClauseType::UsesS)
  {
    
  }
  else if (c.clauseType == ClauseType::Parent)
  {

  }
  else if (c.clauseType == ClauseType::ParentT)
  {

  }
  else if (c.clauseType == ClauseType::Follows)
  {

  }
  else if (c.clauseType == ClauseType::FollowsT)
  {

  }
  else if(c.clauseType == ClauseType::AssignPatt)
  {
    
  }
 
}


