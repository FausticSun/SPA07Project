
#include "PKB.h"

#include "catch.hpp"
using namespace std;


TEST_CASE("VarTable Test") {

	VarTable vt;

	queue<pair<TokenType, string>> tokens;
	tokens.push(make_pair(TokenType::Keyword, "procedure"));
	tokens.push(make_pair(TokenType::Identifier, "readPoint"));
	tokens.push(make_pair(TokenType::Separator, "{"));
	tokens.push(make_pair(TokenType::Keyword, "read"));
	tokens.push(make_pair(TokenType::Identifier, "x"));
	tokens.push(make_pair(TokenType::Separator, ";"));
	tokens.push(make_pair(TokenType::Keyword, "read"));
	tokens.push(make_pair(TokenType::Identifier, "y"));
	tokens.push(make_pair(TokenType::Separator, ";"));
	tokens.push(make_pair(TokenType::Separator, "}"));

	vt.buildVarTable(tokens);

	REQUIRE(vt.contains("x") == true);
	REQUIRE(vt.contains("readPoint") == false);
	REQUIRE(vt.toString() == "x, y");
}


