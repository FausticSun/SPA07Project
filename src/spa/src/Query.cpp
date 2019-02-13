#include <Query.h>

bool Clause::isValid()
{
  if ((clauseType == ClauseType::UsesS || clauseType == ClauseType::ModifiesS)) {
    return false;
  }
  return true;
}

Query::Query() = default;
Query::~Query() = default;

bool Query::isValid()
{
  if (!clauses.empty()) {
    for (std::vector<Clause>::iterator iter = clauses.begin();
      iter != clauses.end(); ++iter) {
      if (!iter->isValid()) {
	      return false;
      }
     

    }
  }
}

bool Query::isEntity(QueryEntityType q1)
{
	return q1 != QueryEntityType::Line && q1 != QueryEntityType::Name &&
		q1 != QueryEntityType::Underscore && q1 != QueryEntityType::Expression
                &&q1 != QueryEntityType::Boolean;
}
