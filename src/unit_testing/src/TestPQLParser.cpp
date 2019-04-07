#include "GeneralLexer.h"
#include "PQLParser.h"
#include "catch.hpp"
#include <sstream>

using Lexer::Token;
using Lexer::TokenType;

TEST_CASE("Empty query") {
  std::string pql = R"()";
  std::stringstream ss;
  ss << pql;
  std::list<Token> tokens = Lexer::tokenize(ss);
  REQUIRE_THROWS(Parser::parsePQL(tokens));
}
