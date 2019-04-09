#pragma once
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
  Progline,
  Attrref,
};

enum class ClauseType {
  Follows,
  FollowsT,
  Parent,
  ParentT,
  UsesS,
  ModifiesS,
  Next,
  NextT,
  Calls,
  CallsT,
  Affects,
  AffectsT,
  AssignPatt,
  IfPatt,
  WhilePatt,
  With,
  NextBip,
  NextBipT,
  AffectsBip,
  AffectsBipT
};

struct QueryEntity {
  QueryEntity() {
    this->type = QueryEntityType::Assign; // default
    this->name = "";
    this->attrRefSynonymType =
        QueryEntityType::Assign; // default, only used when this QueryEntity is
                                 // of type Attrref
  };

  QueryEntity(QueryEntityType type, std::string name = "")
      : type(type), name(name){};

  QueryEntity(QueryEntityType type, std::string name, QueryEntityType type2)
      : type(type), name(name), attrRefSynonymType(type2){};

  friend bool operator==(const QueryEntity &lhs, const QueryEntity &rhs) {
	  return lhs.type == rhs.type && lhs.name == rhs.name;
  }

  QueryEntityType type;
  std::string name;
  QueryEntityType attrRefSynonymType;
};

class Clause {
public:
  Clause(ClauseType clauseType, std::vector<QueryEntity> parameters)
      : clauseType(clauseType), parameters(parameters){};
  ClauseType clauseType;
  std::vector<QueryEntity> parameters;

  friend bool operator==(const Clause &lhs, const Clause &rhs) {
	  return lhs.clauseType == rhs.clauseType && lhs.parameters == rhs.parameters;
  }

  bool isValid();
};

class Query {
public:
  std::vector<QueryEntity> target;
  std::vector<QueryEntity> selectors;
  std::vector<Clause> clauses;
  void setQuery(std::vector<QueryEntity> t, std::vector<QueryEntity> s,
                std::vector<Clause> c);

  bool isValid();
  bool isEntity(QueryEntityType q);
};
