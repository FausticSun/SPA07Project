#pragma once
#include "Entity.h"
#include <queue>
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
  QueryEntity() {
    this->type = QueryEntityType::Assign; // default
    this->name = "";
  };
  QueryEntity(QueryEntityType type, std::string name = "")
      : type(type), name(name){};
  QueryEntityType type;
  std::string name;
};

class Clause {
public:
  Clause(ClauseType clauseType, std::vector<QueryEntity> parameters)
      : clauseType(clauseType), parameters(parameters){};
  ClauseType clauseType;
  std::vector<QueryEntity> parameters;

  bool isValid();
};

class Query {
public:
  QueryEntity target;
  std::vector<QueryEntity> selectors;
  std::vector<Clause> clauses;

	void setQuery(QueryEntity, std::vector<QueryEntity>, std::vector<Clause>);
	bool isValid();
	bool isEntity(QueryEntityType q);
};
