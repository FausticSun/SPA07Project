#include "PqlEvaluator.h"

PqlEvaluator::PqlEvaluator(const PKB& pkb) {
	this -> mypkb = pkb;
}

PqlEvaluator::~PqlEvaluator() = default;

list<string> PqlEvaluator::evaluateQuery(string input) {
	PQLParser pp(input);
	queue<pair<PQLTokenType, string>> d_queue = pp.getDeclarationQueue();
	queue<pair<PQLTokenType, string>> s_queue = pp.getSelectQueue();
	if (d_queue.front().first == PQLTokenType::Keyword && d_queue.front().second == "variable")
	{
		d_queue.pop();
		s_queue.pop();
		if (s_queue.front().first == PQLTokenType::Identifier && s_queue.front().second == d_queue.front().second) {

			set<string> vtble = mypkb.getVarTable();
			list<string> results(vtble.begin(), vtble.end());
			return results;
		}

	}
	if (d_queue.front().first == PQLTokenType::Keyword && d_queue.front().second == "procedure")
	{
		d_queue.pop();
		s_queue.pop();
		if (s_queue.front().first == PQLTokenType::Identifier && s_queue.front().second == d_queue.front().second) {
			set<string> proble = mypkb.getProcTable();
			list<string> results(proble.begin(), proble.end());
			return results;
		}

	}
	
}