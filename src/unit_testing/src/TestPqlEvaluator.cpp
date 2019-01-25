
#include "PqlEvaluator.h"

#include "catch.hpp"
using namespace std;

TEST_CASE("PqlEvaluator Constructor Test") {

	PqlEvaluator pe;

	REQUIRE(pe.getQueryCount() == 0);
}


TEST_CASE("PqlEvaluator pass query Test") {

	PqlEvaluator pe;
	pe.evaluateQuery("msg");

	REQUIRE(pe.getParsedQuery() == "msg");
	REQUIRE(pe.getQueryCount() == 1);
}