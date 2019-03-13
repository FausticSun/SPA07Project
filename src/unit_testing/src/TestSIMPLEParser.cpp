#include "GeneralLexer.h"
#include "SIMPLEParser.h"
#include "catch.hpp"
#include <sstream>

TEST_CASE("Empty program") {
  std::string program = R"()";
  std::stringstream ss;
  ss << program;
  std::list<Token> tokens = Lexer::tokenize(ss);
  REQUIRE_THROWS(Parser::parseSIMPLE(tokens));
}

TEST_CASE("Single empty procedure") {
  std::string program = R"(
  procedure main {
  }
  )";
  std::stringstream ss;
  ss << program;
  std::list<Token> tokens = Lexer::tokenize(ss);
  REQUIRE_THROWS(Parser::parseSIMPLE(tokens));
}

TEST_CASE("Read statement") {
  std::string program = R"(
  procedure main {
    read a;
  }
  )";
  std::stringstream ss;
  ss << program;
  std::list<Token> tokens = Lexer::tokenize(ss);
  auto pkb = Parser::parseSIMPLE(tokens);
  auto varTable = pkb->getVarTable();
  REQUIRE(varTable.getData().size() == 1);
  REQUIRE(varTable.getData().count({"a"}) == 1);
}
