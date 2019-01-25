#include "PqlEvaluator.h"

PqlEvaluator::PqlEvaluator() {
	PqlEvaluator::queryCount = 0;
}

PqlEvaluator::~PqlEvaluator() {}

string PqlEvaluator::getParsedQuery() {
	return queries.back();
}

int PqlEvaluator::getQueryCount() {
	return PqlEvaluator::queryCount;
}

string PqlEvaluator::evaluateQuery(string query) {
	try {
		//call a set of API in PKB
		queries.push_back(query);
		PqlEvaluator::queryCount++;
	}
	catch (const char* msg) {
		return msg;
	}

	return results;
}

