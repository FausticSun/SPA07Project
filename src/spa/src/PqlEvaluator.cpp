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
			list<string> results;
			int i = 0;
			set<string>::iterator it = mypkb.getVarTable().begin();
			set<string>::iterator its = mypkb.getVarTable().end();
                        while(it!=its)
                        {
							cout << *it << endl;
							it++;
                        }
			return results;
		}

	}
	if (d_queue.front().first == PQLTokenType::Keyword && d_queue.front().second == "procedure")
	{
		d_queue.pop();
		s_queue.pop();
		if (s_queue.front().first == PQLTokenType::Identifier && s_queue.front().second == d_queue.front().second) {
			list<string> results;
			for (set<string>::iterator it = mypkb.getProcTable().begin(); it != mypkb.getProcTable().end(); ++it)
			{
				results.push_back(*it);

			}
			return results;
		}

	}
	
}