
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

TEST_CASE("Testing constant a") {
  const string input = "constant a; Select a";
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

TEST_CASE("Testing while a without selection") {
  const string input = "while a;";
  queue<pair<TokenType, string>> res;

  SECTION("1") {

    REQUIRE_THROWS_WITH(PQLLexer(input), "no selction after the declaration.");
    // REQUIRE_THROWS_WITH(p.getTokenQueue(), "no selction after the
    // declaration.");
  }
}

TEST_CASE("Testing while a, b without semicollumn") {
  const string input = "while a, b Selct a;";
  queue<pair<TokenType, string>> res;

  SECTION("1") {

    REQUIRE_THROWS_WITH(PQLLexer(input), "missing semicollumn in declaration!");
    // REQUIRE_THROWS_WITH(p.getTokenQueue(), "no selction after the
    // declaration.");
  }
}

TEST_CASE("Testing assign a, b without semicollumn") {
  const string input = "assign a, b Selct a;";
  queue<pair<TokenType, string>> res;

  SECTION("1") {

    REQUIRE_THROWS_WITH(PQLLexer(input), "missing semicollumn in declaration!");
    // REQUIRE_THROWS_WITH(p.getTokenQueue(), "no selction after the
    // declaration.");
  }
}

TEST_CASE("Testing read a, b without semicollumn") {
  const string input = "read a, b Selct a;";
  queue<pair<TokenType, string>> res;

  SECTION("1") {

    REQUIRE_THROWS_WITH(PQLLexer(input), "missing semicollumn in declaration!");
    // REQUIRE_THROWS_WITH(p.getTokenQueue(), "no selction after the
    // declaration.");
  }
}

TEST_CASE("Testing print a, b without semicollumn") {
  const string input = "print a, b Selct a;";
  queue<pair<TokenType, string>> res;

  SECTION("1") {

    REQUIRE_THROWS_WITH(PQLLexer(input), "missing semicollumn in declaration!");
    // REQUIRE_THROWS_WITH(p.getTokenQueue(), "no selction after the
    // declaration.");
  }
}

TEST_CASE("Testing constant a, b without semicollumn") {
  const string input = "constant a, b Selct a;";
  queue<pair<TokenType, string>> res;

  SECTION("1") {

    REQUIRE_THROWS_WITH(PQLLexer(input), "missing semicollumn in declaration!");
    // REQUIRE_THROWS_WITH(p.getTokenQueue(), "no selction after the
    // declaration.");
  }
}

TEST_CASE("Testing prog_line a, b without semicollumn") {
  const string input = "prog_line a, b Selct a;";
  queue<pair<TokenType, string>> res;

  SECTION("1") {

    REQUIRE_THROWS_WITH(PQLLexer(input), "missing semicollumn in declaration!");
    // REQUIRE_THROWS_WITH(p.getTokenQueue(), "no selction after the
    // declaration.");
  }
}

TEST_CASE("Testing call a, b without semicollumn") {
  const string input = "call a, b Selct a;";
  queue<pair<TokenType, string>> res;

  SECTION("1") {

    REQUIRE_THROWS_WITH(PQLLexer(input), "missing semicollumn in declaration!");
    // REQUIRE_THROWS_WITH(p.getTokenQueue(), "no selction after the
    // declaration.");
  }
}

TEST_CASE("Testing variable a, b without semicollumn") {
  const string input = "variable a, b Selct a;";
  queue<pair<TokenType, string>> res;

  SECTION("1") {

    REQUIRE_THROWS_WITH(PQLLexer(input), "missing semicollumn in declaration!");
    // REQUIRE_THROWS_WITH(p.getTokenQueue(), "no selction after the
    // declaration.");
  }
}

TEST_CASE("Testing procedure a, b without semicollumn") {
  const string input = "procedure a, b Selct a;";
  queue<pair<TokenType, string>> res;

  SECTION("1") {

    REQUIRE_THROWS_WITH(PQLLexer(input), "missing semicollumn in declaration!");
    // REQUIRE_THROWS_WITH(p.getTokenQueue(), "no selction after the
    // declaration.");
  }
}

TEST_CASE("Testing while a, follows a unexpected keyword") {
  const string input = "while a; hello a";
  queue<pair<TokenType, string>> res;

  SECTION("1") {

    REQUIRE_THROWS_WITH(PQLLexer(input),
                        "should follows a new declaration or selection");
    // REQUIRE_THROWS_WITH(p.getTokenQueue(), "no selction after the
    // declaration.");
  }
}

TEST_CASE("Testing assign a, follows a unexpected keyword") {
  const string input = "assign a; hello a";
  queue<pair<TokenType, string>> res;

  SECTION("1") {

    REQUIRE_THROWS_WITH(PQLLexer(input),
                        "should follows a new declaration or selection");
    // REQUIRE_THROWS_WITH(p.getTokenQueue(), "no selction after the
    // declaration.");
  }
}

TEST_CASE("Testing read a, follows a unexpected keyword") {
  const string input = "read a; hedsasdllo a";
  queue<pair<TokenType, string>> res;

  SECTION("1") {

    REQUIRE_THROWS_WITH(PQLLexer(input),
                        "should follows a new declaration or selection");
    // REQUIRE_THROWS_WITH(p.getTokenQueue(), "no selction after the
    // declaration.");
  }
}

TEST_CASE("Testing prog_line a, follows a unexpected keyword") {
  const string input = "prog_line a; hedsasdllo a";
  queue<pair<TokenType, string>> res;

  SECTION("1") {

    REQUIRE_THROWS_WITH(PQLLexer(input),
                        "should follows a new declaration or selection");
    // REQUIRE_THROWS_WITH(p.getTokenQueue(), "no selction after the
    // declaration.");
  }
}

TEST_CASE("Testing print a, follows a unexpected keyword") {
  const string input = "print a; 21222323hello a";
  queue<pair<TokenType, string>> res;

  SECTION("1") {

    REQUIRE_THROWS_WITH(PQLLexer(input),
                        "should follows a new declaration or selection");
    // REQUIRE_THROWS_WITH(p.getTokenQueue(), "no selction after the
    // declaration.");
  }
}

TEST_CASE("Testing if ifs, follows a unexpected keyword") {
  const string input = "if ifs; 324fhdfdfasfello ifs";
  queue<pair<TokenType, string>> res;

  SECTION("1") {

    REQUIRE_THROWS_WITH(PQLLexer(input),
                        "should follows a new declaration or selection");
    // REQUIRE_THROWS_WITH(p.getTokenQueue(), "no selction after the
    // declaration.");
  }
}

TEST_CASE("Testing vairable a, follows a unexpected keyword") {
  const string input = "variable a; hahahello a";
  queue<pair<TokenType, string>> res;

  SECTION("1") {

    REQUIRE_THROWS_WITH(PQLLexer(input),
                        "should follows a new declaration or selection");
    // REQUIRE_THROWS_WITH(p.getTokenQueue(), "no selction after the
    // declaration.");
  }
}

TEST_CASE("Testing stmt s, follows a unexpected keyword") {
  const string input = "stmt s; stmthello s";
  queue<pair<TokenType, string>> res;

  SECTION("1") {

    REQUIRE_THROWS_WITH(PQLLexer(input),
                        "should follows a new declaration or selection");
    // REQUIRE_THROWS_WITH(p.getTokenQueue(), "no selction after the
    // declaration.");
  }
}

TEST_CASE("Testing constant c, follows a unexpected keyword") {
  const string input = "constant c; helconstantlo c";
  queue<pair<TokenType, string>> res;

  SECTION("1") {

    REQUIRE_THROWS_WITH(PQLLexer(input),
                        "should follows a new declaration or selection");
    // REQUIRE_THROWS_WITH(p.getTokenQueue(), "no selction after the
    // declaration.");
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

TEST_CASE("Testing if a, select a with semicolumn") {
  const string input = "if a; Select a;";

  SECTION("1") { REQUIRE_THROWS_WITH(PQLLexer(input), "no semicollumn"); }
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

TEST_CASE("Testing assign a, select a with semicolumn") {
  const string input = "assign a; Select a;";

  SECTION("1") { REQUIRE_THROWS_WITH(PQLLexer(input), "no semicollumn"); }
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

TEST_CASE("Testing read r, select r with semicolumn") {
  const string input = "read r; Select r;";

  SECTION("1") { REQUIRE_THROWS_WITH(PQLLexer(input), "no semicollumn"); }
}

TEST_CASE("Testing pattern following a unexpected string") {
  const string input = "assign a; Select a pattern a(\"x\", _) wrong";

  SECTION("1") {
    REQUIRE_THROWS_WITH(PQLLexer(input),
                        "should be pattern or such that or and");
  }
}

TEST_CASE("Testing print a") {
  const string input = "print p; Select p";
  queue<pair<TokenType, string>> res;
  PQLLexer p(input);

  res = p.getTokenQueue();

  SECTION("1") {

    REQUIRE(res.front().first == TokenType::Keyword);
    REQUIRE(res.front().second == "print");
    res.pop();
    REQUIRE(res.front().first == TokenType::Identifier);
    REQUIRE(res.front().second == "p");
    res.pop();
    REQUIRE(res.front().first == TokenType::Separator);
    REQUIRE(res.front().second == ";");
    res.pop();

    REQUIRE(res.front().first == TokenType::Keyword);
    REQUIRE(res.front().second == "Select");
    res.pop();
    REQUIRE(res.front().first == TokenType::Identifier);
    REQUIRE(res.front().second == "p");
    res.pop();
  }
}

TEST_CASE("Testing print with 2 parameters version 1") {
  const string input = "print p, p2; Select p";
  queue<pair<TokenType, string>> res;
  PQLLexer p(input);

  res = p.getTokenQueue();

  SECTION("1") {

    REQUIRE(res.front().first == TokenType::Keyword);
    REQUIRE(res.front().second == "print");
    res.pop();
    REQUIRE(res.front().first == TokenType::Identifier);
    REQUIRE(res.front().second == "p");
    res.pop();
    REQUIRE(res.front().first == TokenType::Separator);
    REQUIRE(res.front().second == ",");
    res.pop();
    REQUIRE(res.front().first == TokenType::Identifier);
    REQUIRE(res.front().second == "p2");
    res.pop();
    REQUIRE(res.front().first == TokenType::Separator);
    REQUIRE(res.front().second == ";");
    res.pop();

    REQUIRE(res.front().first == TokenType::Keyword);
    REQUIRE(res.front().second == "Select");
    res.pop();
    REQUIRE(res.front().first == TokenType::Identifier);
    REQUIRE(res.front().second == "p");
    res.pop();
  }
}

TEST_CASE("Testing print with 2 parameters version 2") {
  const string input = "print p , p2; Select p";
  queue<pair<TokenType, string>> res;
  PQLLexer p(input);

  res = p.getTokenQueue();

  SECTION("1") {

    REQUIRE(res.front().first == TokenType::Keyword);
    REQUIRE(res.front().second == "print");
    res.pop();
    REQUIRE(res.front().first == TokenType::Identifier);
    REQUIRE(res.front().second == "p");
    res.pop();
    REQUIRE(res.front().first == TokenType::Separator);
    REQUIRE(res.front().second == ",");
    res.pop();
    REQUIRE(res.front().first == TokenType::Identifier);
    REQUIRE(res.front().second == "p2");
    res.pop();
    REQUIRE(res.front().first == TokenType::Separator);
    REQUIRE(res.front().second == ";");
    res.pop();

    REQUIRE(res.front().first == TokenType::Keyword);
    REQUIRE(res.front().second == "Select");
    res.pop();
    REQUIRE(res.front().first == TokenType::Identifier);
    REQUIRE(res.front().second == "p");
    res.pop();
  }
}

TEST_CASE("Testing print with 2 parameters version 3") {
  const string input = "print p,p2; Select p";
  queue<pair<TokenType, string>> res;
  PQLLexer p(input);

  res = p.getTokenQueue();

  SECTION("1") {

    REQUIRE(res.front().first == TokenType::Keyword);
    REQUIRE(res.front().second == "print");
    res.pop();
    REQUIRE(res.front().first == TokenType::Identifier);
    REQUIRE(res.front().second == "p");
    res.pop();
    REQUIRE(res.front().first == TokenType::Separator);
    REQUIRE(res.front().second == ",");
    res.pop();
    REQUIRE(res.front().first == TokenType::Identifier);
    REQUIRE(res.front().second == "p2");
    res.pop();
    REQUIRE(res.front().first == TokenType::Separator);
    REQUIRE(res.front().second == ";");
    res.pop();

    REQUIRE(res.front().first == TokenType::Keyword);
    REQUIRE(res.front().second == "Select");
    res.pop();
    REQUIRE(res.front().first == TokenType::Identifier);
    REQUIRE(res.front().second == "p");
    res.pop();
  }
}

TEST_CASE("Testing Follows") {
  const string input = "assign a; while w; Select a such that Follows(w, a)";
  // const string input = "stmt	  s       ,      s1			;Select
  // s such 	that Follows(s1, s)";
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
    REQUIRE(res.front().second == "while");
    res.pop();
    REQUIRE(res.front().first == TokenType::Identifier);
    REQUIRE(res.front().second == "w");
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
    REQUIRE(res.front().second == "Follows");
    res.pop();
    REQUIRE(res.front().first == TokenType::Separator);
    REQUIRE(res.front().second == "(");
    res.pop();
    REQUIRE(res.front().first == TokenType::Identifier);
    REQUIRE(res.front().second == "w");
    res.pop();
    REQUIRE(res.front().first == TokenType::Separator);
    REQUIRE(res.front().second == ",");
    res.pop();
    REQUIRE(res.front().first == TokenType::Identifier);
    REQUIRE(res.front().second == "a");
    res.pop();
    REQUIRE(res.front().first == TokenType::Separator);
    REQUIRE(res.front().second == ")");
    res.pop();
  }
}

TEST_CASE("Testing Follows version 1") {
  const string input = "assign a; while w; Select a such that Follows(w, a )";
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
    REQUIRE(res.front().second == "while");
    res.pop();
    REQUIRE(res.front().first == TokenType::Identifier);
    REQUIRE(res.front().second == "w");
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
    REQUIRE(res.front().second == "Follows");
    res.pop();
    REQUIRE(res.front().first == TokenType::Separator);
    REQUIRE(res.front().second == "(");
    res.pop();
    REQUIRE(res.front().first == TokenType::Identifier);
    REQUIRE(res.front().second == "w");
    res.pop();
    REQUIRE(res.front().first == TokenType::Separator);
    REQUIRE(res.front().second == ",");
    res.pop();
    REQUIRE(res.front().first == TokenType::Identifier);
    REQUIRE(res.front().second == "a");
    res.pop();
    REQUIRE(res.front().first == TokenType::Separator);
    REQUIRE(res.front().second == ")");
    res.pop();
  }
}

TEST_CASE("Testing Follows(1, s)") {
  const string input =
      "assign a; stmt s; variable v; Select s such that Follows (1, s)";

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
    REQUIRE(res.front().second == "stmt");
    res.pop();
    REQUIRE(res.front().first == TokenType::Identifier);
    REQUIRE(res.front().second == "s");
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
    REQUIRE(res.front().second == "s");
    res.pop();
    REQUIRE(res.front().first == TokenType::Keyword);
    REQUIRE(res.front().second == "such that");
    res.pop();
    REQUIRE(res.front().first == TokenType::Keyword);
    REQUIRE(res.front().second == "Follows");
    res.pop();
    REQUIRE(res.front().first == TokenType::Separator);
    REQUIRE(res.front().second == "(");
    res.pop();
    REQUIRE(res.front().first == TokenType::Identifier);
    REQUIRE(res.front().second == "1");
    res.pop();
    REQUIRE(res.front().first == TokenType::Separator);
    REQUIRE(res.front().second == ",");
    res.pop();
    REQUIRE(res.front().first == TokenType::Identifier);
    REQUIRE(res.front().second == "s");
    res.pop();
    REQUIRE(res.front().first == TokenType::Separator);
    REQUIRE(res.front().second == ")");
    res.pop();
  }
}

TEST_CASE("Testing Uses with (\"(xxx)\", \"(xxx)\")") {
  const string input = "assign a; stmt s; variable v; Select s such that Uses "
                       "(\"(a + b)\", \"(a + b)\")";
  // const string input = "assign a; stmt s; variable v; Select s such that Uses
  // (\"(a + b) + (c + d)\", \"(a + b)\")";

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
    REQUIRE(res.front().second == "stmt");
    res.pop();
    REQUIRE(res.front().first == TokenType::Identifier);
    REQUIRE(res.front().second == "s");
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
    REQUIRE(res.front().second == "s");
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
    REQUIRE(res.front().second == "(a+b)");
    res.pop();
    REQUIRE(res.front().first == TokenType::Separator);
    REQUIRE(res.front().second == "\"");
    res.pop();
    REQUIRE(res.front().first == TokenType::Separator);
    REQUIRE(res.front().second == ",");
    res.pop();
    REQUIRE(res.front().first == TokenType::Separator);
    REQUIRE(res.front().second == "\"");
    res.pop();
    REQUIRE(res.front().first == TokenType::Identifier);
    REQUIRE(res.front().second == " (a+b)");
    res.pop();
    REQUIRE(res.front().first == TokenType::Separator);
    REQUIRE(res.front().second == "\"");
    res.pop();
    REQUIRE(res.front().first == TokenType::Separator);
    REQUIRE(res.front().second == ")");
    res.pop();
  }
}

TEST_CASE("Testing Follows(1, s) with semicollumn") {
  const string input =
      "assign a; stmt s; variable v; Select s such that Follows (1, s) ;";
}

TEST_CASE("Test wrong format pattern with variable on the right") {
  const string input = "assign a; variable v; Select a pattern a(\"x\", v)";

  SECTION("1") {
    REQUIRE_THROWS_WITH(PQLLexer(input), "cannot have a variable on the right");
  }
}

TEST_CASE("Test wrong format Uses without (") {
  const string input =
      "assign a; variable v; Select a such that Uses \"x\", v);";

  SECTION("1") {
    REQUIRE_THROWS_WITH(PQLLexer(input), "wrong format of selection!");
  }
}

TEST_CASE("Test wrong format Modifies without (") {
  const string input =
      "assign a; variable v; Select a such that Modifies \"x\", v);";

  SECTION("1") {
    REQUIRE_THROWS_WITH(PQLLexer(input), "wrong format of selection!");
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
