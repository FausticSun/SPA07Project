#include "PqlEvaluator.h"

PqlEvaluator::PqlEvaluator() {
	PqlEvaluator::queryCount = 0;
}

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
	catch (const* char msg) {
		//handle exception thrown from PKB and return error message
		return errmsg;
	}

	return results;
}

