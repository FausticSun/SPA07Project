#include "PQLParser.h"

#include "catch.hpp"

using namespace std;

TEST_CASE("Testing basic declaration variable") {
  const string input = "variable a;";
  queue<pair<DeclarationType, string>> res;
  PQLParser p(input);

  res = p.getDeclarationQueue();
  SECTION("Selectqueue's first pair is KEYWORD: variable") {

    REQUIRE(res.front().first == DeclarationType::Variable);
  }

  SECTION("Selectqueue's second pair is Identifier: a") {
    REQUIRE(res.front().second == "a");
    res.pop();
  }
}

TEST_CASE("Testing basic select") {
  const string input = "select a";
  // queue<tuple<RelationType, string, string>> res;
  string t;
  PQLParser p(input);
  // res = p.getSelectQueue();
  t = p.getTarget();
  SECTION("discover the targer is a") { REQUIRE(t == "a"); }
}

TEST_CASE("Testing basic declaration variable and select this variable") {
  const string input = "variable a; select a";
  queue<pair<DeclarationType, string>> resd;
  queue<tuple<RelationType, string, string>> ress;
  string t;

  PQLParser p(input);
  resd = p.getDeclarationQueue();
  ress = p.getSelectQueue();
  t = p.getTarget();
  SECTION("Selectqueue's first pair is KEYWORD: variable") {

    REQUIRE(resd.front().first == DeclarationType::Variable);
    REQUIRE(resd.front().second == "a");
  }

  SECTION("Target is a") { REQUIRE(t == "a"); }
}

TEST_CASE("Testing basic declaration procedure") {
  const string input = "procedure a;";
  const string input1 = "select a";
  queue<pair<DeclarationType, string>> res;
  queue<tuple<RelationType, string, string>> res1;
  string t;
  PQLParser p(input);

  res = p.getDeclarationQueue();
  res1 = p.getSelectQueue();
  t = p.getTarget();
  SECTION("Selectqueue's first pair is Procedure: a") {

    REQUIRE(res.front().first == DeclarationType::Procedure);
    REQUIRE(res.front().second == "a");
  }
}

TEST_CASE("Testing basic declaration procedure and select this procedure") {
  const string input = "procedure a; select a";
  queue<pair<DeclarationType, string>> resd;
  queue<tuple<RelationType, string, string>> ress;
  string t;
  PQLParser p(input);
  resd = p.getDeclarationQueue();
  ress = p.getSelectQueue();
  t = p.getTarget();
  SECTION("Selectqueue's first pair is KEYWORD: procedure") {

    REQUIRE(resd.front().first == DeclarationType::Procedure);
    REQUIRE(resd.front().second == "a");
  }

  SECTION("Target is a") { REQUIRE(t == "a"); }
}
