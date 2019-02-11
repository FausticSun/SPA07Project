#include "Token.h"

#include "catch.hpp"

SCENARIO("Construction of Token") {

	Token* token = new Token(TokenType::Identifier, "testToken");

	SECTION("Type of Token") {
		REQUIRE(token->getType() == TokenType::Identifier);
	}

	SECTION("Name of Token") {
		REQUIRE(token->getName() == "testToken");
	}

}
