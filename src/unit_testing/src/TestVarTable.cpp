
#include "PKB.h"

#include "catch.hpp"
using namespace std;


TEST_CASE("VarTable Test") {

	VarTable vt;

	vt.add("x");
	
	REQUIRE(vt.contains("x") == true);
	REQUIRE(vt.toString() == "x");

	VarTable vt2;
	// empty variable list
	REQUIRE(vt2.toString() == ""); 
	vt2.add("x");
	vt2.add("y");
	vt2.add("z");
	REQUIRE(vt2.toString() == "x, y, z");
}


