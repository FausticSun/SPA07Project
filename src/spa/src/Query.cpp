#include <Query.h>

bool Clause::isValid() {
  if ((clauseType == ClauseType::UsesS ||
       clauseType == ClauseType::ModifiesS)) {
    return false;
  }
  return true;
}

void Query::setQuery(QueryEntity t, std::vector<QueryEntity> s,
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

bool Query::isValid() {
  if (!clauses.empty()) {
    for (std::vector<Clause>::iterator iter = clauses.begin();
         iter != clauses.end(); ++iter) {
      if (!iter->isValid()) {
        return false;
      }
    }
  }
}

bool Query::isEntity(QueryEntityType q1) {
  return q1 != QueryEntityType::Line && q1 != QueryEntityType::Name &&
         q1 != QueryEntityType::Underscore &&
         q1 != QueryEntityType::Expression && q1 != QueryEntityType::Boolean;
}
