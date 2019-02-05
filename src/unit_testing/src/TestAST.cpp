
#include "TNode.h"

#include "catch.hpp"
using namespace std;
void require(bool b) {
    REQUIRE(b);
}

TEST_CASE(" Test") {
	
    require(1 == 1);
}


