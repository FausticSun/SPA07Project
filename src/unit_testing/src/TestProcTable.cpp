
#include "PKB.h"

#include "catch.hpp"
using namespace std;


TEST_CASE("ProcTable Test") {

	ProcTable pt;

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

	pt.buildProcTable(tokens);

	REQUIRE(pt.contains("readPoint") == true);
	REQUIRE(pt.contains("x") == false);
	REQUIRE(pt.toString() == "readPoint");
}


