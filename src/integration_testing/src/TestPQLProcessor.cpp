#include "catch.hpp"
#include <PqlEvaluator.h>
using namespace std;

PKB buildPKB() {
  PKB pkb;
  /*pkb.insertConstant();*/
  // stmt 1-9
  pkb.insertStatement("1", StatementType::Assign);
  pkb.insertStatement("2", StatementType::If);
  pkb.insertStatement("3", StatementType::While);
  pkb.insertStatement("4", StatementType::Stmt);
  pkb.insertStatement("5", StatementType::Call);
  pkb.insertStatement("6", StatementType::Print);
  pkb.insertStatement("7", StatementType::Read);
  pkb.insertStatement("8", StatementType::Assign);
  pkb.insertStatement("9", StatementType::Assign);
  // procedure: mian one two three four
  pkb.insertProc("main");
  pkb.insertProc("one");
  pkb.insertProc("two");
  pkb.insertProc("three");
  pkb.insertProc("four");
  // var a,x,y,z,i
  pkb.insertVar("a");
  pkb.insertVar("x");
  pkb.insertVar("y");
  pkb.insertVar("z");
  pkb.insertVar("i");
  // set uses
  pkb.setUses(1, "x");
  pkb.setUses(2, "a");
  pkb.setUses(4, "z");
  pkb.setUses(5, "i");
  // set modifies
  pkb.setModifies(1, "y");
  pkb.setModifies(2, "x");
  pkb.setModifies(3, "a");
  pkb.setModifies(4, "z");
  // set follows
  pkb.setFollows(1, 2);
  pkb.setFollows(8, 3);
  pkb.setFollows(2, 3);
  pkb.setFollows(4, 5);
  pkb.setFollows(4, 6);
  // set follow*
  pkb.setFollowsT(1, 2);
  pkb.setFollowsT(1, 3);
  pkb.setFollowsT(4, 5);
  pkb.setFollowsT(4, 6);
  pkb.setFollowsT(1, 6);
  // set parent
  pkb.setParent(1, 2);
  pkb.setParent(2, 3);
  pkb.setParent(1, 4);
  pkb.setParent(5, 6);
  // set parent*
  pkb.setParentT(1, 2);
  pkb.setParentT(1, 3);
  pkb.setParentT(1, 4);
  pkb.setParentT(2, 3);
  pkb.setParentT(5, 6);
  // set pattern
  string rhs = "x y * z i * + ";
  string lfs = "a";
  pkb.insertAssign(1, lfs, rhs);
  rhs = "x y * z * i * ";
  lfs = "a";
  pkb.insertAssign(8, lfs, rhs);
  rhs = "a y z * i + * ";
  lfs = "x";
  pkb.insertAssign(9, lfs, rhs);
  return pkb;
}

SCENARIO("test for simple queries") {
  PKB pkb = buildPKB();
  PqlEvaluator pe(pkb);
  SECTION("Get variable") {
    string query = "variable a; Select a";
    list<string> result = pe.evaluateQuery(query);
    REQUIRE(result.size() == 5);
  }
  SECTION("get stmt") {
    string query = "stmt s; Select s";
    list<string> result = pe.evaluateQuery(query);
    REQUIRE(result.size() == 9);
  }
  SECTION("get procedure") {
    string query = "procedure p; Select p";
    list<string> result = pe.evaluateQuery(query);
    REQUIRE(result.size() == 5);
  }
  SECTION("get read") {
    string query = "read p; Select p";
    list<string> result = pe.evaluateQuery(query);
    REQUIRE(result.size() == 1);
  }
}

SCENARIO("test for query with one clause") {
  PKB pkb = buildPKB();
  PqlEvaluator pe(pkb);

  SECTION("Uses") {
    string query = "stmt s; Select s such that Uses (s,\"a\")";
    list<string> result = pe.evaluateQuery(query);
    REQUIRE(result.size() == 1);
    REQUIRE(result.front() == "2");
  }
  SECTION("Modifies") {
    string query = "stmt s; Select s such that Modifies (s,\"a\")";
    list<string> result = pe.evaluateQuery(query);
    REQUIRE(result.size() == 1);
    REQUIRE(result.front() == "3");
  }
  SECTION("Follows") {
    string query = "stmt s; Select s such that Follows (s,2)";
    list<string> result = pe.evaluateQuery(query);
    REQUIRE(result.size() == 1);
    REQUIRE(result.front() == "1");
  }
  SECTION("FollowsT") {
    string query = "stmt s; Select s such that Follows* (s,6)";
    list<string> result = pe.evaluateQuery(query);
    REQUIRE(result.size() == 2);
    REQUIRE(result.front() == "1");
    result.pop_front();
    REQUIRE(result.front() == "4");
  }
  SECTION("Parent") {
    string query = "stmt s; Select s such that Parent (s,4)";
    list<string> result = pe.evaluateQuery(query);
    REQUIRE(result.size() == 1);
    REQUIRE(result.front() == "1");
  }
  SECTION("ParentT") {
    string query = "stmt s; Select s such that Parent* (s,3)";
    list<string> result = pe.evaluateQuery(query);
    REQUIRE(result.size() == 2);
    REQUIRE(result.front() == "1");
    result.pop_front();
    REQUIRE(result.front() == "2");
  }
  SECTION("Pattern") {
    string query = "assign s; Select s pattern s(\"a\",_\"x*y\"_)";
    list<string> result = pe.evaluateQuery(query);
    REQUIRE(result.size() == 2);
    REQUIRE(result.front() == "1");
    result.pop_front();
    REQUIRE(result.front() == "8");
  }
}
SCENARIO("test for query with two caluses") {
  PKB pkb = buildPKB();
  PqlEvaluator pe(pkb);
  SECTION("Uses and Pattern") {
    string query =
        "assign s; Select s such that Uses(s,\"x\") pattern s(\"a\",_\"x*y\"_)";
    list<string> result = pe.evaluateQuery(query);
    REQUIRE(result.size() == 1);
    REQUIRE(result.front() == "1");
  }
  SECTION("Modifies and Pattern") {
    string query = "assign s; Select s such that Modifies(s,\"y\") pattern "
                   "s(\"a\",_\"x*y\"_)";
    list<string> result = pe.evaluateQuery(query);
    REQUIRE(result.size() == 1);
    REQUIRE(result.front() == "1");
  }
  SECTION("Parent and Pattern") {
    string query =
        "assign s; Select s such that Parent(s,4) pattern s(\"a\",_\"x*y\"_)";
    list<string> result = pe.evaluateQuery(query);
    REQUIRE(result.size() == 1);
    REQUIRE(result.front() == "1");
  }
  SECTION("Parent* and Pattern") {
    string query =
        "assign s; Select s such that Parent*(s,3) pattern s(\"a\",_\"x*y\"_)";
    list<string> result = pe.evaluateQuery(query);
    REQUIRE(result.size() == 1);
    REQUIRE(result.front() == "1");
  }
  SECTION("Follows and Pattern") {
    string query = "assign s; Select s such that Follows(s,3) pattern "
                   "s(\"a\",_\"x*y*z\"_)";
    list<string> result = pe.evaluateQuery(query);
    REQUIRE(result.size() == 1);
    REQUIRE(result.front() == "8");
  }
  SECTION("Follows* and Pattern") {
    string query = "assign s; Select s such that Follows*(s,6) pattern "
                   "s(\"a\",\"x*y+z*i\")";
    list<string> result = pe.evaluateQuery(query);
    REQUIRE(result.size() == 1);
    REQUIRE(result.front() == "1");
  }
}
