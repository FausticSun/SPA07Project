#pragma once

#include <PKB.h>
#include <iostream>
#include <list>
#include <string>
#include <vector>
#include "Query.h"

using namespace std;

enum class DesignEntityType {
  Stmt,
  Read,
  Print,
  Call,
  While,
  If,
  Assign,
  Variable,
  Constant,
  Procedure,
};
enum class RelationshipType {
  Follows,
  FollowsT,
  Parent,
  Uses,
  UsesP,
  ModifiesS,
  ModifiesP,
};

class PqlEvaluator {
public:
  PqlEvaluator(const PKB& pkb);
  ~PqlEvaluator();
  list<string> evaluateQuery(string query);
  

private:
  PKB mypkb;
  set<string> getData(Clause c);
  /*list<Relation> getUses(Entity first, Entity second);
  list<Relation> getModifies(Entity first, Entity second);
  list<Relation> getFollows(Entity first, Entity second);
  list<Relation> getParents(Entity first, Entity second);*/
  
};