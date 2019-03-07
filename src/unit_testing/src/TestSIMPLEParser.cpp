#include "GeneralLexer.h"
#include "SIMPLEParser.h"
#include "catch.hpp"

SCENARIO("Empty program") {
  std::list<Token> tokens;
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
