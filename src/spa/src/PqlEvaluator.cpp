#include "PqlEvaluator.h"

PqlEvaluator::PqlEvaluator() { this->queryCount = 0; }

PqlEvaluator::~PqlEvaluator() = default;

string PqlEvaluator::getParsedQuery() { return queries.back(); }

int PqlEvaluator::getQueryCount() { return this->queryCount; }

Query PqlEvaluator::evaluateQuery() {
  Query results;
  return results;
}

Query::Query() {}

Query::Query(list<string> des, string tar, Clause rel, Clause pat) {
  this->DesignEntities = des;
  this->targetDesignEntities = tar;
  this->relClause = rel;
  this->patClause = pat;
}

Query::~Query() {}
