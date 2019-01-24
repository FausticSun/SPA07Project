
#include "PKB.h"
#include "PKB.cpp"

#include "catch.hpp"
using namespace std;
void require(bool b) {
	REQUIRE(b);
}

TEST_CASE("2st Test") {

	VarTable vt;


	require(vt.numOfVars == 0);
}


