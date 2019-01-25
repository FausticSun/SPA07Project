#pragma once

#include<stdio.h>
#include<PKB.h>
#include <iostream>
#include <string>
#include <vector>
#include<list>

using namespace std;

class PqlEvaluator {
	public:
		
		PqlEvaluator();
		~PqlEvaluator();
		string getParsedQuery();
		int getQueryCount();
		string evaluateQuery(string query);

	private:
		list<string> queries;
		string results;
		static int queryCount;

};