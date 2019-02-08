#include "PQLParser.h"

#include "catch.hpp"

using namespace std;

TEST_CASE("Testing basic declaration variable") {
  const string input = "variable a;";
  queue<pair<TokenType, string>> res;
  PQLParser p(input);

  res = p.getDeclarationQueue();
  SECTION("Selectqueue's first pair is KEYWORD: variable") {

    REQUIRE(res.front().first == TokenType::Keyword);
    REQUIRE(res.front().second == "variable");
  }

  SECTION("Selectqueue's second pair is Identifier: a") {
    res.pop();
    REQUIRE(res.front().first == TokenType::Identifier);
    REQUIRE(res.front().second == "a");
  }
}

TEST_CASE("Testing basic select") {
  const string input = "select a";
  queue<pair<TokenType, string>> res;
  PQLParser p(input);
  res = p.getSelectQueue();
  SECTION("Selectqueue's first pair is KEYWORD: select") {

    REQUIRE(res.front().first == TokenType::Keyword);
    REQUIRE(res.front().second == "selection");
  }

  SECTION("Selectqueue's second pair is Identifier: a") {
    res.pop();
    REQUIRE(res.front().first == TokenType::Identifier);
    REQUIRE(res.front().second == "a");
  }
}

TEST_CASE("Testing basic declaration variable and select this variable") {
  const string input = "variable a; select a";
  queue<pair<TokenType, string>> resd, ress;
  PQLParser p(input);
  resd = p.getDeclarationQueue();
  ress = p.getSelectQueue();
  SECTION("Selectqueue's first pair is KEYWORD: variable") {

    REQUIRE(resd.front().first == TokenType::Keyword);
    REQUIRE(resd.front().second == "variable");
  }

  SECTION("Selectqueue's second pair is Identifier: a") {
    resd.pop();
    REQUIRE(resd.front().first == TokenType::Identifier);
    REQUIRE(resd.front().second == "a");
  }

  SECTION("Selectqueue's first pair is KEYWORD: select") {

    REQUIRE(ress.front().first == TokenType::Keyword);
    REQUIRE(ress.front().second == "selection");
  }

  SECTION("Selectqueue's second pair is Identifier: a") {
    ress.pop();
    REQUIRE(ress.front().first == TokenType::Identifier);
    REQUIRE(ress.front().second == "a");
  }
}

TEST_CASE("Testing basic declaration procedure") {
  const string input = "procedure a;";
  const string input1 = "select a";
  queue<pair<TokenType, string>> res;
  queue<pair<TokenType, string>> res1;
  PQLParser p(input);

  res = p.getDeclarationQueue();
  SECTION("Selectqueue's first pair is KEYWORD: procedure") {

    REQUIRE(res.front().first == TokenType::Keyword);
    REQUIRE(res.front().second == "procedure");
  }

  SECTION("Selectqueue's second pair is Identifier: a") {
    res.pop();
    REQUIRE(res.front().first == TokenType::Identifier);
    REQUIRE(res.front().second == "a");
  }
}

TEST_CASE("Testing basic declaration procedure and select this procedure") {
  const string input = "procedure a; select a";
  queue<pair<TokenType, string>> resd, ress;
  PQLParser p(input);
  resd = p.getDeclarationQueue();
  ress = p.getSelectQueue();
  SECTION("Selectqueue's first pair is KEYWORD: procedure") {

    REQUIRE(resd.front().first == TokenType::Keyword);
    REQUIRE(resd.front().second == "procedure");
  }

  SECTION("Selectqueue's second pair is Identifier: a") {
    resd.pop();
    REQUIRE(resd.front().first == TokenType::Identifier);
    REQUIRE(resd.front().second == "a");
  }

  SECTION("Selectqueue's first pair is KEYWORD: select") {

    REQUIRE(ress.front().first == TokenType::Keyword);
    REQUIRE(ress.front().second == "selection");
  }

  SECTION("Selectqueue's second pair is Identifier: a") {
    ress.pop();
    REQUIRE(ress.front().first == TokenType::Identifier);
    REQUIRE(ress.front().second == "a");
  }
}
