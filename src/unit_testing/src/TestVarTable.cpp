
#include "PKB.h"
#include "PKB.cpp"

#include "catch.hpp"
using namespace std;


TEST_CASE("2nd Test") {

	VarTable vt;

	REQUIRE(vt.numOfVars == 0);

	vt.add("x");
	
	REQUIRE(vt.contains("x") == true);
	REQUIRE(vt.getAll() == "x");

	VarTable vt2;
	// empty variable list
	REQUIRE(vt2.getAll() == ""); 
	vt2.add("x");
	vt2.add("y");
	vt2.add("z");
	REQUIRE(vt2.getAll() == "x, y, z");
}


