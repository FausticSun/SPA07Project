#include "PQLLexer.h"

#include "catch.hpp"

using namespace PQLLexerToken;
using namespace std;

TEST_CASE("Testing constant a") {
  //const string input = "constant a; Select a";
  const string input = "constant a; Select a pattern a(\"v\", \"a % b * (c + d) + e * f / (g - h)\")";

  queue<pair<TokenType, string>> res;
  PQLLexer p(input);

  res = p.getTokenQueue();

  SECTION("1") {

    REQUIRE(res.front().first == TokenType::Keyword);
    REQUIRE(res.front().second == "constant");
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

TEST_CASE("Testing procedure a") {
  const string input = "procedure a; Select a";
  queue<pair<TokenType, string>> res;
  PQLLexer p(input);

  res = p.getTokenQueue();

  SECTION("1") {

    REQUIRE(res.front().first == TokenType::Keyword);
    REQUIRE(res.front().second == "procedure");
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

TEST_CASE("Testing read a") {
  const string input = "read a; Select a";
  // const string input = "read a; Select BOOLEAN with \"hallo\" = a.progName";
  queue<pair<TokenType, string>> res;
  PQLLexer p(input);

  res = p.getTokenQueue();

  SECTION("Selectqueue's first pair is KEYWORD: variable") {

    REQUIRE(res.front().first == TokenType::Keyword);
    REQUIRE(res.front().second == "read");
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

TEST_CASE("Testing prog_line a") {
  const string input = "prog_line a; Select a";
  // const string input = "stmt	  s       ,      s1			; Select
  // s such 	that Follows(s1, s)"; const string input = "prog_line a; Select
  // a pattern a(a, _\"(a b)\"_) such that Uses(w, \"ff\")"; const string input
  // = "assign a, a1 Select v such that Modifies(\"SpecificationTest\", \"x\")";
  queue<pair<TokenType, string>> res;
  PQLLexer p(input);

  res = p.getTokenQueue();

  SECTION("Selectqueue's first pair is KEYWORD: prog_line") {

    REQUIRE(res.front().first == TokenType::Keyword);
    REQUIRE(res.front().second == "prog_line");
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

TEST_CASE("Testing while a") {
  const string input =
      "while a; Select a"; // const string input = "while a; Select a";
  queue<pair<TokenType, string>> res;
  PQLLexer p(input);

  res = p.getTokenQueue();

  SECTION("1") {

    REQUIRE(res.front().first == TokenType::Keyword);
    REQUIRE(res.front().second == "while");
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

TEST_CASE("Testing if a") {
  const string input = "if a; Select a";
  queue<pair<TokenType, string>> res;
  PQLLexer p(input);

  res = p.getTokenQueue();

  SECTION("1") {

    REQUIRE(res.front().first == TokenType::Keyword);
    REQUIRE(res.front().second == "if");
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

TEST_CASE("Testing assign a") {
  const string input = "assign a; Select a";
  queue<pair<TokenType, string>> res;
  PQLLexer p(input);

  res = p.getTokenQueue();

  SECTION("1") {

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
    REQUIRE(res.front().second == "Select");
    res.pop();
    REQUIRE(res.front().first == TokenType::Identifier);
    REQUIRE(res.front().second == "a");
    res.pop();
  }
}

TEST_CASE("Testing read r") {
  const string input = "read r; Select r";
  queue<pair<TokenType, string>> res;
  PQLLexer p(input);

  res = p.getTokenQueue();

  SECTION("1") {

    REQUIRE(res.front().first == TokenType::Keyword);
    REQUIRE(res.front().second == "read");
    res.pop();
    REQUIRE(res.front().first == TokenType::Identifier);
    REQUIRE(res.front().second == "r");
    res.pop();
    REQUIRE(res.front().first == TokenType::Separator);
    REQUIRE(res.front().second == ";");
    res.pop();

    REQUIRE(res.front().first == TokenType::Keyword);
    REQUIRE(res.front().second == "Select");
    res.pop();
    REQUIRE(res.front().first == TokenType::Identifier);
    REQUIRE(res.front().second == "r");
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

TEST_CASE("Test wrong format Modifies with (\"\",)") {
	const string input =
		"assign a; variable v; Select a such that Modifies (\"x\", v)";

	queue<pair<TokenType, string>> res;
	PQLLexer p(input);

	res = p.getTokenQueue();

	SECTION("1") {

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
		REQUIRE(res.front().second == "variable");
		res.pop();
		REQUIRE(res.front().first == TokenType::Identifier);
		REQUIRE(res.front().second == "v");
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
		REQUIRE(res.front().first == TokenType::Keyword);
		REQUIRE(res.front().second == "such that");
		res.pop();
		REQUIRE(res.front().first == TokenType::Keyword);
		REQUIRE(res.front().second == "Modifies");
		res.pop();
		REQUIRE(res.front().first == TokenType::Separator);
		REQUIRE(res.front().second == "(");
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
		REQUIRE(res.front().first == TokenType::Separator);
		REQUIRE(res.front().second == ",");
		res.pop();
		REQUIRE(res.front().first == TokenType::Identifier);
		REQUIRE(res.front().second == "v");
		res.pop();
		REQUIRE(res.front().first == TokenType::Separator);
		REQUIRE(res.front().second == ")");
		res.pop();
	}
}

TEST_CASE("Test wrong format Uses with (\"\",)") {
	const string input =
		"assign a; variable v; Select a such that Uses (\"x\", v)";

	queue<pair<TokenType, string>> res;
	PQLLexer p(input);

	res = p.getTokenQueue();

	SECTION("1") {

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
		REQUIRE(res.front().second == "variable");
		res.pop();
		REQUIRE(res.front().first == TokenType::Identifier);
		REQUIRE(res.front().second == "v");
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
		REQUIRE(res.front().first == TokenType::Keyword);
		REQUIRE(res.front().second == "such that");
		res.pop();
		REQUIRE(res.front().first == TokenType::Keyword);
		REQUIRE(res.front().second == "Uses");
		res.pop();
		REQUIRE(res.front().first == TokenType::Separator);
		REQUIRE(res.front().second == "(");
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
		REQUIRE(res.front().first == TokenType::Separator);
		REQUIRE(res.front().second == ",");
		res.pop();
		REQUIRE(res.front().first == TokenType::Identifier);
		REQUIRE(res.front().second == "v");
		res.pop();
		REQUIRE(res.front().first == TokenType::Separator);
		REQUIRE(res.front().second == ")");
		res.pop();
	}
}

TEST_CASE("Test wrong format Modifies with (a,)") {
	const string input =
		"assign a; variable v; Select a such that Modifies (a, v)";

	queue<pair<TokenType, string>> res;
	PQLLexer p(input);

	res = p.getTokenQueue();

	SECTION("1") {

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
		REQUIRE(res.front().second == "variable");
		res.pop();
		REQUIRE(res.front().first == TokenType::Identifier);
		REQUIRE(res.front().second == "v");
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
		REQUIRE(res.front().first == TokenType::Keyword);
		REQUIRE(res.front().second == "such that");
		res.pop();
		REQUIRE(res.front().first == TokenType::Keyword);
		REQUIRE(res.front().second == "Modifies");
		res.pop();
		REQUIRE(res.front().first == TokenType::Separator);
		REQUIRE(res.front().second == "(");
		res.pop();
		REQUIRE(res.front().first == TokenType::Identifier);
		REQUIRE(res.front().second == "a");
		res.pop();
		REQUIRE(res.front().first == TokenType::Separator);
		REQUIRE(res.front().second == ",");
		res.pop();
		REQUIRE(res.front().first == TokenType::Identifier);
		REQUIRE(res.front().second == "v");
		res.pop();
		REQUIRE(res.front().first == TokenType::Separator);
		REQUIRE(res.front().second == ")");
		res.pop();
	}
}

TEST_CASE("Test wrong format Uses with (a,)") {
	const string input = "assign a; variable v; Select a such that Uses (a, v)";

	queue<pair<TokenType, string>> res;
	PQLLexer p(input);

	res = p.getTokenQueue();

	SECTION("1") {

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
		REQUIRE(res.front().second == "variable");
		res.pop();
		REQUIRE(res.front().first == TokenType::Identifier);
		REQUIRE(res.front().second == "v");
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
		REQUIRE(res.front().first == TokenType::Keyword);
		REQUIRE(res.front().second == "such that");
		res.pop();
		REQUIRE(res.front().first == TokenType::Keyword);
		REQUIRE(res.front().second == "Uses");
		res.pop();
		REQUIRE(res.front().first == TokenType::Separator);
		REQUIRE(res.front().second == "(");
		res.pop();
		REQUIRE(res.front().first == TokenType::Identifier);
		REQUIRE(res.front().second == "a");
		res.pop();
		REQUIRE(res.front().first == TokenType::Separator);
		REQUIRE(res.front().second == ",");
		res.pop();
		REQUIRE(res.front().first == TokenType::Identifier);
		REQUIRE(res.front().second == "v");
		res.pop();
		REQUIRE(res.front().first == TokenType::Separator);
		REQUIRE(res.front().second == ")");
		res.pop();
	}
}

TEST_CASE("Testing tuple with 6 element") {
	const string input = "call c1, c2, c3, c4; assign a, a1, a222; Select <c1.procName, a, a1,c2.procName,c3.procName,a222,c4.procName>";
	// const string input = "read a; Select BOOLEAN with \"hallo\" = a.progName";
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
		REQUIRE(res.front().second == ",");
		res.pop();
		REQUIRE(res.front().first == TokenType::Identifier);
		REQUIRE(res.front().second == "c3");
		res.pop();
		REQUIRE(res.front().first == TokenType::Separator);
		REQUIRE(res.front().second == ",");
		res.pop();
		REQUIRE(res.front().first == TokenType::Identifier);
		REQUIRE(res.front().second == "c4");
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
		REQUIRE(res.front().second == ",");
		res.pop();
		REQUIRE(res.front().first == TokenType::Identifier);
		REQUIRE(res.front().second == "a1");
		res.pop();
		REQUIRE(res.front().first == TokenType::Separator);
		REQUIRE(res.front().second == ",");
		res.pop();
		REQUIRE(res.front().first == TokenType::Identifier);
		REQUIRE(res.front().second == "a222");
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
		REQUIRE(res.front().second == "a");
		res.pop();
		REQUIRE(res.front().first == TokenType::Separator);
		REQUIRE(res.front().second == ",");
		res.pop();
		REQUIRE(res.front().first == TokenType::Identifier);
		REQUIRE(res.front().second == "a1");
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
		REQUIRE(res.front().second == "c3");
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
		REQUIRE(res.front().second == "a222");
		res.pop();
		REQUIRE(res.front().first == TokenType::Separator);
		REQUIRE(res.front().second == ",");
		res.pop();
		REQUIRE(res.front().first == TokenType::Identifier);
		REQUIRE(res.front().second == "c4");
		res.pop();
		REQUIRE(res.front().first == TokenType::Separator);
		REQUIRE(res.front().second == ".");
		res.pop();
		REQUIRE(res.front().first == TokenType::Identifier);
		REQUIRE(res.front().second == "procName");
		res.pop();
		REQUIRE(res.front().first == TokenType::Separator);
		REQUIRE(res.front().second == ">");
		res.pop();
	}
}


TEST_CASE("Testing tuple with 3 element") {
	const string input = "call c1, c2; assign a222; Select <c1.procName,c2.procName,a222>";
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
		REQUIRE(res.front().second == "a222");
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
		REQUIRE(res.front().second == "a222");
		res.pop();
		REQUIRE(res.front().first == TokenType::Separator);
		REQUIRE(res.front().second == ">");
		res.pop();
	}
}
