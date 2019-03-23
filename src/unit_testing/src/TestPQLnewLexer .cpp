#include "PQLLexer.h"

#include "catch.hpp"

using namespace PQLLexerToken;
using namespace std;

TEST_CASE("Testing basic declaration variable") {
	const string input = "  variable a; Select a";
	queue<pair<TokenType, string>> res;
	PQLLexer p(input);

	res = p.getTokenQueue();

	SECTION("Selectqueue's first pair is KEYWORD: variable") {

		REQUIRE(res.front().first == TokenType::Keyword);
		REQUIRE(res.front().second == "variable");
		res.pop();
		REQUIRE(res.front().first == TokenType::Identifier);
		REQUIRE(res.front().second == "a");
		res.pop();
		REQUIRE(res.front().first == TokenType::Separator);
		REQUIRE(res.front().second == ";");
		res.pop();

		REQUIRE(res.front().first == TokenType::Keyword);
		REQUIRE(res.front().second == "Select");
		res.pop();
		REQUIRE(res.front().first == TokenType::Identifier);
		REQUIRE(res.front().second == "a");
		res.pop();
	}
}

TEST_CASE("Testing with clause and tuple with \"x\"") {
	const string input = "call c1, c2; variable v; prog_line pl; Select <c1.procName,c2.procName,v> with v.varName = \"x\"";
	queue<pair<TokenType, string>> res;
	PQLLexer p(input);

	res = p.getTokenQueue();

	SECTION("test for result") {

		REQUIRE(res.front().first == TokenType::Keyword);
		REQUIRE(res.front().second == "call");
		res.pop();
		REQUIRE(res.front().first == TokenType::Identifier);
		REQUIRE(res.front().second == "c1");
		res.pop();
		REQUIRE(res.front().first == TokenType::Separator);
		REQUIRE(res.front().second == ",");
		res.pop();
		REQUIRE(res.front().first == TokenType::Identifier);
		REQUIRE(res.front().second == "c2");
		res.pop();
		REQUIRE(res.front().first == TokenType::Separator);
		REQUIRE(res.front().second == ";");
		res.pop();
		REQUIRE(res.front().first == TokenType::Keyword);
		REQUIRE(res.front().second == "variable");
		res.pop();
		REQUIRE(res.front().first == TokenType::Identifier);
		REQUIRE(res.front().second == "v");
		res.pop();
		REQUIRE(res.front().first == TokenType::Separator);
		REQUIRE(res.front().second == ";");
		res.pop();
		REQUIRE(res.front().first == TokenType::Keyword);
		REQUIRE(res.front().second == "prog_line");
		res.pop();
		REQUIRE(res.front().first == TokenType::Identifier);
		REQUIRE(res.front().second == "pl");
		res.pop();
		REQUIRE(res.front().first == TokenType::Separator);
		REQUIRE(res.front().second == ";");
		res.pop();
		REQUIRE(res.front().first == TokenType::Keyword);
		REQUIRE(res.front().second == "Select");
		res.pop();
		REQUIRE(res.front().first == TokenType::Separator);
		REQUIRE(res.front().second == "<");
		res.pop();
		REQUIRE(res.front().first == TokenType::Identifier);
		REQUIRE(res.front().second == "c1");
		res.pop();
		REQUIRE(res.front().first == TokenType::Separator);
		REQUIRE(res.front().second == ".");
		res.pop();
		REQUIRE(res.front().first == TokenType::Identifier);
		REQUIRE(res.front().second == "procName");
		res.pop();
		REQUIRE(res.front().first == TokenType::Separator);
		REQUIRE(res.front().second == ",");
		res.pop();
		REQUIRE(res.front().first == TokenType::Identifier);
		REQUIRE(res.front().second == "c2");
		res.pop();
		REQUIRE(res.front().first == TokenType::Separator);
		REQUIRE(res.front().second == ".");
		res.pop();
		REQUIRE(res.front().first == TokenType::Identifier);
		REQUIRE(res.front().second == "procName");
		res.pop();
		REQUIRE(res.front().first == TokenType::Separator);
		REQUIRE(res.front().second == ",");
		res.pop();
		REQUIRE(res.front().first == TokenType::Identifier);
		REQUIRE(res.front().second == "v");
		res.pop();
		REQUIRE(res.front().first == TokenType::Separator);
		REQUIRE(res.front().second == ">");
		res.pop();
		REQUIRE(res.front().first == TokenType::Keyword);
		REQUIRE(res.front().second == "with");
		res.pop();
		REQUIRE(res.front().first == TokenType::Identifier);
		REQUIRE(res.front().second == "v");
		res.pop();
		REQUIRE(res.front().first == TokenType::Separator);
		REQUIRE(res.front().second == ".");
		res.pop();
		REQUIRE(res.front().first == TokenType::Identifier);
		REQUIRE(res.front().second == "varName");
		res.pop();
		REQUIRE(res.front().first == TokenType::Separator);
		REQUIRE(res.front().second == "=");
		res.pop();
		REQUIRE(res.front().first == TokenType::Separator);
		REQUIRE(res.front().second == "\"");
		res.pop();
		REQUIRE(res.front().first == TokenType::Identifier);
		REQUIRE(res.front().second == "x");
		res.pop();
		REQUIRE(res.front().first == TokenType::Separator);
		REQUIRE(res.front().second == "\"");
		res.pop();

	}
}


TEST_CASE("Testing with clause and tuple") {
	const string input = "call c1, c2; assign a; prog_line pl; Select <c1.procName,c2.procName,a> with a.stmt# = pl";
	queue<pair<TokenType, string>> res;
	PQLLexer p(input);

	res = p.getTokenQueue();

	SECTION("test for result") {

		REQUIRE(res.front().first == TokenType::Keyword);
		REQUIRE(res.front().second == "call");
		res.pop();
		REQUIRE(res.front().first == TokenType::Identifier);
		REQUIRE(res.front().second == "c1");
		res.pop();
		REQUIRE(res.front().first == TokenType::Separator);
		REQUIRE(res.front().second == ",");
		res.pop();
		REQUIRE(res.front().first == TokenType::Identifier);
		REQUIRE(res.front().second == "c2");
		res.pop();
		REQUIRE(res.front().first == TokenType::Separator);
		REQUIRE(res.front().second == ";");
		res.pop();
		REQUIRE(res.front().first == TokenType::Keyword);
		REQUIRE(res.front().second == "assign");
		res.pop();
		REQUIRE(res.front().first == TokenType::Identifier);
		REQUIRE(res.front().second == "a");
		res.pop();
		REQUIRE(res.front().first == TokenType::Separator);
		REQUIRE(res.front().second == ";");
		res.pop();
		REQUIRE(res.front().first == TokenType::Keyword);
		REQUIRE(res.front().second == "prog_line");
		res.pop();
		REQUIRE(res.front().first == TokenType::Identifier);
		REQUIRE(res.front().second == "pl");
		res.pop();
		REQUIRE(res.front().first == TokenType::Separator);
		REQUIRE(res.front().second == ";");
		res.pop();
		REQUIRE(res.front().first == TokenType::Keyword);
		REQUIRE(res.front().second == "Select");
		res.pop();
		REQUIRE(res.front().first == TokenType::Separator);
		REQUIRE(res.front().second == "<");
		res.pop();
		REQUIRE(res.front().first == TokenType::Identifier);
		REQUIRE(res.front().second == "c1");
		res.pop();
		REQUIRE(res.front().first == TokenType::Separator);
		REQUIRE(res.front().second == ".");
		res.pop();
		REQUIRE(res.front().first == TokenType::Identifier);
		REQUIRE(res.front().second == "procName");
		res.pop();
		REQUIRE(res.front().first == TokenType::Separator);
		REQUIRE(res.front().second == ",");
		res.pop();
		REQUIRE(res.front().first == TokenType::Identifier);
		REQUIRE(res.front().second == "c2");
		res.pop();
		REQUIRE(res.front().first == TokenType::Separator);
		REQUIRE(res.front().second == ".");
		res.pop();
		REQUIRE(res.front().first == TokenType::Identifier);
		REQUIRE(res.front().second == "procName");
		res.pop();
		REQUIRE(res.front().first == TokenType::Separator);
		REQUIRE(res.front().second == ",");
		res.pop();
		REQUIRE(res.front().first == TokenType::Identifier);
		REQUIRE(res.front().second == "a");
		res.pop();
		REQUIRE(res.front().first == TokenType::Separator);
		REQUIRE(res.front().second == ">");
		res.pop();
		REQUIRE(res.front().first == TokenType::Keyword);
		REQUIRE(res.front().second == "with");
		res.pop();
		REQUIRE(res.front().first == TokenType::Identifier);
		REQUIRE(res.front().second == "a");
		res.pop();
		REQUIRE(res.front().first == TokenType::Separator);
		REQUIRE(res.front().second == ".");
		res.pop();
		REQUIRE(res.front().first == TokenType::Identifier);
		REQUIRE(res.front().second == "stmt#");
		res.pop();
		REQUIRE(res.front().first == TokenType::Separator);
		REQUIRE(res.front().second == "=");
		res.pop();
		REQUIRE(res.front().first == TokenType::Identifier);
		REQUIRE(res.front().second == "pl");
		res.pop();

	}
}
