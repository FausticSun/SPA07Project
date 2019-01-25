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
	try {
		//call a set of API in PKB
		queries.push_back(query);
		this -> queryCount++;
	}
	catch (const char* msg) {
		return msg;
	}

	return results;
}

