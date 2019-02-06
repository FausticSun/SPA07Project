
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
			list<string> result = pe.evaluateQuery(PQL);
			REQUIRE(result.front() == "alice");
			result.pop_front();
			REQUIRE(result.front() == "Bob");
			result.pop_front();
			REQUIRE(result.front() == "Ju");
			result.pop_front();
			REQUIRE(result.front() == "ben");
			result.pop_front();
			REQUIRE(result.empty());
                           
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
			REQUIRE(result.front() == "main");
			result.pop_front();
			REQUIRE(result.front() == "readp");
			result.pop_front();
			REQUIRE(result.front() == "getp");
			result.pop_front();
			REQUIRE(result.empty());
		}
	}

}