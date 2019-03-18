#include "ExprParser.h"
#include "GeneralLexer.h"
#include "catch.hpp"
#include <sstream>

TEST_CASE("Empty expression") {
  std::string expr = R"()";
  std::stringstream ss;
  ss << expr;
  std::list<Token> tokens = Lexer::tokenize(ss);
  auto postfix = Parser::parseExpr(tokens);
  auto postfixString = Parser::tokensToString(postfix);
  REQUIRE(postfixString == " ");
}

TEST_CASE("Simple expression") {
  std::string expr = R"(1 + 2)";
  std::stringstream ss;
  ss << expr;
  std::list<Token> tokens = Lexer::tokenize(ss);
  auto postfix = Parser::parseExpr(tokens);
  auto postfixString = Parser::tokensToString(postfix);
  REQUIRE(postfixString == " 1 2 + ");
}

TEST_CASE("Left associativity") {
  std::string expr = R"(1 + 2 + 3)";
  std::stringstream ss;
  ss << expr;
  std::list<Token> tokens = Lexer::tokenize(ss);
  auto postfix = Parser::parseExpr(tokens);
  auto postfixString = Parser::tokensToString(postfix);
  REQUIRE(postfixString == " 1 2 + 3 + ");
}

TEST_CASE("Different precedence") {
  std::string expr = R"(1 + 2 * 3)";
  std::stringstream ss;
  ss << expr;
  std::list<Token> tokens = Lexer::tokenize(ss);
  auto postfix = Parser::parseExpr(tokens);
  auto postfixString = Parser::tokensToString(postfix);
  REQUIRE(postfixString == " 1 2 3 * + ");
}

TEST_CASE("Single set of parentheses") {
  std::string expr = R"(1 + (2 + 3))";
  std::stringstream ss;
  ss << expr;
  std::list<Token> tokens = Lexer::tokenize(ss);
  auto postfix = Parser::parseExpr(tokens);
  auto postfixString = Parser::tokensToString(postfix);
  REQUIRE(postfixString == " 1 2 3 + + ");
}

TEST_CASE("Multiple nest parentheses") {
  std::string expr =
      R"(1 + (2 + (3 + (4 + (5 + (6 + (7 + (8 + (9 + 10)))))))))";
  std::stringstream ss;
  ss << expr;
  std::list<Token> tokens = Lexer::tokenize(ss);
  auto postfix = Parser::parseExpr(tokens);
  auto postfixString = Parser::tokensToString(postfix);
  REQUIRE(postfixString == " 1 2 3 4 5 6 7 8 9 10 + + + + + + + + + ");
}

TEST_CASE("All arithmetic operators") {
  std::string expr = R"(1 * 2 + 3 / 4 - 5 % 6)";
  std::stringstream ss;
  ss << expr;
  std::list<Token> tokens = Lexer::tokenize(ss);
  auto postfix = Parser::parseExpr(tokens);
  auto postfixString = Parser::tokensToString(postfix);
  REQUIRE(postfixString == " 1 2 * 3 4 / + 5 6 % - ");
}

TEST_CASE("Single relational operator") {
  std::string expr = R"(a == b)";
  std::stringstream ss;
  ss << expr;
  std::list<Token> tokens = Lexer::tokenize(ss);
  auto postfix = Parser::parseExpr(tokens);
  auto postfixString = Parser::tokensToString(postfix);
  REQUIRE(postfixString == " a b == ");
}
