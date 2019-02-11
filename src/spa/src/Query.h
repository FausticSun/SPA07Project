#pragma once
#include "Entity.h"

enum class QueryEntityType {
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
  Expression,
  Boolean,
};
enum class ClauseType {
  Follows,
  FollowsT,
  Parent,
  ParentT,
  UsesS,
  ModifiesS,
  AssignPatt,
};

struct QueryEntity {
  QueryEntity(QueryEntityType type, std::string name = "")
      : type(type), name(name){};
  QueryEntityType type;
  std::string name;
};

struct Clause {
  Clause(ClauseType clauseType, QueryEntity firstRef, QueryEntity secondRef)
      : clauseType(ClauseType), firstRef(firstRef), secondRef(secondRef){};
  ClauseType clauseType;
  QueryEntity firstRef;
  QueryEntity secondRef;
};

struct Query {
  Query();
  vector<QueryEntityType> selectors;
  vector<Clause> clauses;
};
