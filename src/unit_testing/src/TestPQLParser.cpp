#include "PQLParser.h"

#include"catch.hpp"

using namespace std;

TEST_CASE("Testing basic select")
{
	const string input = "select a";
	queue<pair<TokenType, string>> res;
	PQLParser p(input);
	res = p.getSelectQueue();
	SECTION("Selectqueue's first pair is KEYWORD: select") {

		REQUIRE(res.front().first == TokenType::Keyword);
		REQUIRE(res.front().second == "selection");
	}

        SECTION("Selectqueue's second pair is Identifier: a")
	{
			res.pop();
			REQUIRE(res.front().first == TokenType::Identifier);
			REQUIRE(res.front().second == "a");
	}
        
}

TEST_CASE("Testing basic declaration variable")
{
	const string input = "variable a;";
	queue<pair<TokenType, string>> res;
	PQLParser p(input);
	res = p.getDeclarationQueue();
	SECTION("Selectqueue's first pair is KEYWORD: variable") {

		REQUIRE(res.front().first == TokenType::Keyword);
		REQUIRE(res.front().second == "variable");
	}

	SECTION("Selectqueue's second pair is Identifier: a")
	{
		res.pop();
		REQUIRE(res.front().first == TokenType::Identifier);
		REQUIRE(res.front().second == "a");
	}

}

TEST_CASE("Testing basic declaration procedure")
{
	const string input = "procedure a;";
	queue<pair<TokenType, string>> res;
	PQLParser p(input);
	res = p.getDeclarationQueue();
	SECTION("Selectqueue's first pair is KEYWORD: procedure") {

		REQUIRE(res.front().first == TokenType::Keyword);
		REQUIRE(res.front().second == "procedure");
	}

	SECTION("Selectqueue's second pair is Identifier: a")
	{
		res.pop();
		REQUIRE(res.front().first == TokenType::Identifier);
		REQUIRE(res.front().second == "a");
	}

}
