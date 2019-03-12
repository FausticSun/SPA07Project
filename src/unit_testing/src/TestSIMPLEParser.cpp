#include "GeneralLexer.h"
#include "SIMPLEParser.h"
#include "catch.hpp"
#include <sstream>

SCENARIO("Empty program") {
  std::string program = R"()";
  std::stringstream ss;
  ss << program;
  std::list<Token> tokens = Lexer::tokenize(ss);
  REQUIRE_THROWS(Parser::parseSIMPLE(tokens));
}

SCENARIO("Program with an empty procedure") {
  std::list<Token> tokens;
  tokens.push_back({TokenType::Identifier, "procedure"});
  tokens.push_back({TokenType::Identifier, "main"});
  tokens.push_back({TokenType::Delimiter, "{"});
  tokens.push_back({TokenType::Delimiter, "}"});
  REQUIRE_THROWS(Parser::parseSIMPLE(tokens));
}
