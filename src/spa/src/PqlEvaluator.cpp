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

string PqlEvaluator::evaluateQuery(string query) {
	string results;
	DesignEntityType de = query.getDesignEntityType();
	if (isSimpleQuery(query)) {
		results = evaluateSimpleQuery(de);
	}
	return results;
}

string PqlEvaluator::evaluateSimpleQuery(DesignEntityType de) {
	string result;
	if (de == DesignEntityType::Variable) {
		result = PKB.varTable->toString();
	}
	else if (de == DesignEntityType::Procedure) {
		result = PKB.procTable->toString();
	}
	return result;
}

bool PqlEvaluator::isSimpleQuery(string query) {
	//if there is no clauses then it should be simple query
}

