#include "Token.h"

#include "catch.hpp"

SCENARIO("Construction of Token") {

	Token* token = new Token(TokenType::Identifier, "testToken", 1);

	SECTION("Type of Token") {
		REQUIRE(token->getType() == TokenType::Identifier);
	}

	SECTION("Name of Token") {
		REQUIRE(token->getName() == "testToken");
	}

	SECTION("StatementNumber of Token") {
		REQUIRE(token->getStatementNumber() == 1);
	}

}
