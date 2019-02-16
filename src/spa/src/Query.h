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
  bool operator==(QueryEntity other) {
	  return this->type == other.type && this->name == other.name;
  }
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

  void setQuery(QueryEntity t, std::vector<QueryEntity> s,
                std::vector<Clause> c) {

    this->target = t;
    for (int i = 0; i < s.size(); i++) {
      QueryEntity qe = QueryEntity(s[i].type, s[i].name);
      this->selectors.push_back(qe);
    }
    for (int j = 0; j < c.size(); j++) {
      Clause cl = Clause(c[j].clauseType, c[j].parameters);
      this->clauses.push_back(cl);
    }
  }
  bool isValid();
  bool isEntity(QueryEntityType q);
};
