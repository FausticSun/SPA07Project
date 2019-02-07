
#include "PqlEvaluator.h"
#include <algorithm>
#include "catch.hpp"
using namespace std;

SCENARIO("Selecting variable command") {
	string PQL = "variable v; select v;";
	const set<string> vars = { "alice","Bob","Ju","ben" };
	const set<string> procs = { "main","readp","getp" };
	WHEN("PKB is contructed") {
		PKB pkb;
		pkb.setPKB(vars, procs);
		PqlEvaluator pe(pkb);
        THEN("Query")
        {
			list<string> result = pe.evaluateQuery(PQL);
			list<string>::iterator iter = find(result.begin(), result.end(), "alice");
			REQUIRE(*iter == "alice");
			iter = find(result.begin(), result.end(), "Bob");
			REQUIRE(*iter == "Bob");
			iter = find(result.begin(), result.end(), "Ju");
			REQUIRE(*iter == "Ju");
			iter = find(result.begin(), result.end(), "ben");
			REQUIRE(*iter == "ben");
			int size = result.size();
			for (int i = 0; i < size; i++) {
				cout << result.front() << endl;
				result.pop_front();
			}
        }
	}
}

SCENARIO("Selecting procedure command") {
	string PQL = "procedure p;select p;";
	const set<string> vars = { "alice","Bob","Ju","ben" };
	const set<string> procs = { "main","readp","getp" };
	WHEN("PKB is contructed") {
		PKB pkb;
		pkb.setPKB(vars, procs);
		PqlEvaluator pe(pkb);
		THEN("Query")
		{
			list<string> result = pe.evaluateQuery(PQL);
			list<string>::iterator iter = find(result.begin(), result.end(), "main");
			REQUIRE(*iter == "main");
			iter = find(result.begin(), result.end(), "readp");
			REQUIRE(*iter == "readp");
			iter = find(result.begin(), result.end(), "getp");
			REQUIRE(*iter == "getp");
		}
	}

}