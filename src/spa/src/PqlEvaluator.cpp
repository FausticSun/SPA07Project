#include "PqlEvaluator.h"

PqlEvaluator::PqlEvaluator() {
	this -> queryCount = 0;
}

PqlEvaluator::~PqlEvaluator() {}

string PqlEvaluator::getParsedQuery() {
	return queries.back();
}

int PqlEvaluator::getQueryCount() {
	return this -> queryCount;
}

Query PqlEvaluator::evaluateQuery(QueryTokens query) {
	Query results;
	/*DesignEntityType de = query.getDesignEntityType();*/
	if (isSimpleQuery(query)) {
		/*results = evaluateSimpleQuery(de);*/
	}
	return results;
}

Query PqlEvaluator::evaluateSimpleQuery(DesignEntityType de) {
	Query result;
	if (de == DesignEntityType::Variable) {
		/*result = PKB.varTable->toString();*/
	}
	else if (de == DesignEntityType::Procedure) {
		/*result = PKB.procTable->toString();*/
	}
	return result;
}

bool PqlEvaluator::isSimpleQuery(QueryTokens query) {
	bool flag = true;
	//if there is no clauses then it should be simple query
	return flag;
}

Query::Query() {}

Query::Query(list<string> des, string tar,Clause rel,Clause pat) {
	this->DesignEntities = des;
	this->targetDesignEntities = tar;
	this->relClause = rel;
	this->patClause = pat;
}

Query::~Query() {}



