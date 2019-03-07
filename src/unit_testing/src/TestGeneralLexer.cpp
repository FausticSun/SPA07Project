#include "GeneralLexer.h"
#include "catch.hpp"
#include <sstream>

SCENARIO("Single alphabetic identifier") {
  std::stringstream ss;
  ss << "Test" << '\n';
  auto tokens = Lexer::tokenize(ss);
  REQUIRE(tokens.begin()->type == TokenType::Identifier);
  REQUIRE(tokens.begin()->value == "Test");
}

SCENARIO("Single alphanumeric identifier") {
  std::stringstream ss;
  ss << "Test123" << '\n';
  auto tokens = Lexer::tokenize(ss);
  REQUIRE(tokens.begin()->type == TokenType::Identifier);
  REQUIRE(tokens.begin()->value == "Test123");
}

SCENARIO("Single number") {
  std::stringstream ss;
  ss << "12345" << '\n';
  auto tokens = Lexer::tokenize(ss);
  REQUIRE(tokens.begin()->type == TokenType::Number);
  REQUIRE(tokens.begin()->value == "12345");
}

SCENARIO("Invalid identifier") {
  std::stringstream ss;
  ss << "123Test" << '\n';
  REQUIRE_THROWS(Lexer::tokenize(ss));
}

SCENARIO("Consume comment with newline") {
  std::stringstream ss;
  ss << "\\\\ This is a comment" << '\n';
  auto tokens = Lexer::tokenize(ss);
  REQUIRE(tokens.empty());
}

SCENARIO("Consume comment without newline") {
  std::stringstream ss;
  ss << "\\\\ This is a comment";
  auto tokens = Lexer::tokenize(ss);
  REQUIRE(tokens.empty());
}

SCENARIO("Invalid comment with only a single backslash") {
  std::stringstream ss;
  ss << "\\ This is not a comment";
  REQUIRE_THROWS(Lexer::tokenize(ss));
}

SCENARIO("Many spaces") {
  std::stringstream ss;
  ss << "              "
     << "\n";
  auto tokens = Lexer::tokenize(ss);
  REQUIRE(tokens.empty());
}

SCENARIO("Operators") {
  std::stringstream ss;
  ss << "=+-*/%_><!&&||>=<=!===";
  auto tokens = Lexer::tokenize(ss);
  REQUIRE(tokens.size() == 16);
  for (auto it = tokens.begin(); it != tokens.end(); ++it) {
    REQUIRE(it->type == TokenType::Operator);
  }
  auto it = tokens.begin();
  REQUIRE(it->value == "=");
  REQUIRE((++it)->value == "+");
  REQUIRE((++it)->value == "-");
  REQUIRE((++it)->value == "*");
  REQUIRE((++it)->value == "/");
  REQUIRE((++it)->value == "%");
  REQUIRE((++it)->value == "_");
  REQUIRE((++it)->value == ">");
  REQUIRE((++it)->value == "<");
  REQUIRE((++it)->value == "!");
  REQUIRE((++it)->value == "&&");
  REQUIRE((++it)->value == "||");
  REQUIRE((++it)->value == ">=");
  REQUIRE((++it)->value == "<=");
  REQUIRE((++it)->value == "!=");
  REQUIRE((++it)->value == "==");
}

SCENARIO("Delimiters") {
  std::stringstream ss;
  ss << "{(\"#.,;\")}";
  auto tokens = Lexer::tokenize(ss);
  REQUIRE(tokens.size() == 10);
  for (auto it = tokens.begin(); it != tokens.end(); ++it) {
    REQUIRE(it->type == TokenType::Delimiter);
  }
  auto it = tokens.begin();
  REQUIRE(it->value == "{");
  REQUIRE((++it)->value == "(");
  REQUIRE((++it)->value == "\"");
  REQUIRE((++it)->value == "#");
  REQUIRE((++it)->value == ".");
  REQUIRE((++it)->value == ",");
  REQUIRE((++it)->value == ";");
  REQUIRE((++it)->value == "\"");
  REQUIRE((++it)->value == ")");
  REQUIRE((++it)->value == "}");
}
