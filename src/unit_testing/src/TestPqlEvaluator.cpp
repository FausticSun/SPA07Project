
#include "PqlEvaluator.h"
#include "catch.hpp"
#include <algorithm>
using namespace std;

SCENARIO("Selecting variable command") {
  string PQL = "variable v; select v;";
  PqlEvaluator pe;
  WHEN("PKB is contructed") {}
}

SCENARIO("Selecting procedure command") {
  string PQL = "procedure p;select p;";
  PqlEvaluator pe();
  WHEN("PKB is contructed") {}
}
