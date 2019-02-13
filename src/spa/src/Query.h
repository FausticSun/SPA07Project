#pragma once
#include "Entity.h"
#include <vector>

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
  Line,
  Name,
  Underscore,
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
    : type(type),
      name(name) {
  };
  QueryEntityType type;
  std::string name;
};

class Clause {
public:
  Clause(ClauseType clauseType,std::vector<QueryEntity> parameters)
    : clauseType(clauseType),
      parameters(parameters) {
  };
  ClauseType clauseType;
  std::vector<QueryEntity> parameters;

  bool isValid();
};

class Query {
public:
  Query();
  ~Query();
  std::string target;
  std::vector<QueryEntity> selectors;
  std::vector<Clause> clauses;

  bool isValid();
  bool isEntity(QueryEntityType q);
};
