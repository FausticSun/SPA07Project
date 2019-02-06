
#include "PqlEvaluator.h"

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
		        pe.evaluateQuery(PQL);

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
			pe.evaluateQuery(PQL);

		}
	}

}