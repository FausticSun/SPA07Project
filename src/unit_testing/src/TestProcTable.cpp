
#include "PKB.h"

#include "catch.hpp"
using namespace std;


TEST_CASE("ProcTable Test") {

	ProcTable pt;

	pt.add("x");

	REQUIRE(pt.contains("x") == true);
	REQUIRE(pt.toString() == "x");

	ProcTable pt2;
	// empty procedure list
	REQUIRE(pt2.toString() == "");
	pt2.add("x");
	pt2.add("y");
	pt2.add("z");
	REQUIRE(pt2.toString() == "x, y, z");
}


