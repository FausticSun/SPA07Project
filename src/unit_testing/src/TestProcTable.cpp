
#include "PKB.h"

#include "catch.hpp"
using namespace std;

/**
TEST_CASE("ProcTable Test") {

  ProcTable pt;

  queue<Token *> tokens;
  Token t0(TokenType::Procedure, "procedure", 1);
  tokens.push(&t0);
  Token t1(TokenType::Identifier, "readPoint", 1);
  tokens.push(&t1);
  Token t2(TokenType::Separator, "{", 1);
  tokens.push(&t2);
  Token t3(TokenType::Read, "read", 2);
  tokens.push(&t3);
  Token t4(TokenType::Identifier, "x", 2);
  tokens.push(&t4);
  Token t5(TokenType::Separator, ";", 2);
  tokens.push(&t5);
  Token t6(TokenType::Read, "read", 3);
  tokens.push(&t6);
  Token t7(TokenType::Identifier, "y", 3);
  tokens.push(&t7);
  Token t8(TokenType::Separator, ";", 3);
  tokens.push(&t8);
  Token t9(TokenType::Separator, "}", 4);
  tokens.push(&t9);

  pt.buildProcTable(tokens);

  REQUIRE(pt.contains("readPoint") == true);
  REQUIRE(pt.contains("x") == false);
  REQUIRE(pt.toString() == "readPoint");
}
**/
