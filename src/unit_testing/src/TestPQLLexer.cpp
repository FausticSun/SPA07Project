
#include "PQLLexer.h"

#include "catch.hpp"

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

TEST_CASE("Testing variable a") {
  const string input = "variable a; Select a";
  queue<pair<TokenType, string>> res;
  PQLLexer p(input);

  res = p.getTokenQueue();

  SECTION("1") {

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

TEST_CASE("Testing variable with 2 parameters version 1") {
  const string input = "variable a, a2; Select a";
  queue<pair<TokenType, string>> res;
  PQLLexer p(input);

  res = p.getTokenQueue();

  SECTION("1") {

    REQUIRE(res.front().first == TokenType::Keyword);
    REQUIRE(res.front().second == "variable");
    res.pop();
    REQUIRE(res.front().first == TokenType::Identifier);
    REQUIRE(res.front().second == "a");
    res.pop();
    REQUIRE(res.front().first == TokenType::Separator);
    REQUIRE(res.front().second == ",");
    res.pop();
    REQUIRE(res.front().first == TokenType::Identifier);
    REQUIRE(res.front().second == "a2");
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

TEST_CASE("Testing variable with 2 parameters version 2") {
  const string input = "variable a , a2; Select a";
  queue<pair<TokenType, string>> res;
  PQLLexer p(input);

  res = p.getTokenQueue();

  SECTION("1") {

    REQUIRE(res.front().first == TokenType::Keyword);
    REQUIRE(res.front().second == "variable");
    res.pop();
    REQUIRE(res.front().first == TokenType::Identifier);
    REQUIRE(res.front().second == "a");
    res.pop();
    REQUIRE(res.front().first == TokenType::Separator);
    REQUIRE(res.front().second == ",");
    res.pop();
    REQUIRE(res.front().first == TokenType::Identifier);
    REQUIRE(res.front().second == "a2");
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

TEST_CASE("Testing variable with 2 parameters version 3") {
  const string input = "variable a,a2; Select a";
  queue<pair<TokenType, string>> res;
  PQLLexer p(input);

  res = p.getTokenQueue();

  SECTION("1") {

    REQUIRE(res.front().first == TokenType::Keyword);
    REQUIRE(res.front().second == "variable");
    res.pop();
    REQUIRE(res.front().first == TokenType::Identifier);
    REQUIRE(res.front().second == "a");
    res.pop();
    REQUIRE(res.front().first == TokenType::Separator);
    REQUIRE(res.front().second == ",");
    res.pop();
    REQUIRE(res.front().first == TokenType::Identifier);
    REQUIRE(res.front().second == "a2");
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

TEST_CASE("Testing while with 2 parameters version 1") {
  const string input = "while p, p2; Select p";
  queue<pair<TokenType, string>> res;
  PQLLexer p(input);

  res = p.getTokenQueue();

  SECTION("1") {

    REQUIRE(res.front().first == TokenType::Keyword);
    REQUIRE(res.front().second == "while");
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

TEST_CASE("Testing while with 2 parameters version 2") {
  const string input = "while p , p2; Select p";
  queue<pair<TokenType, string>> res;
  PQLLexer p(input);

  res = p.getTokenQueue();

  SECTION("1") {

    REQUIRE(res.front().first == TokenType::Keyword);
    REQUIRE(res.front().second == "while");
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

TEST_CASE("Testing if with 2 parameters version 1") {
  const string input = "if p, p2; Select p";
  queue<pair<TokenType, string>> res;
  PQLLexer p(input);

  res = p.getTokenQueue();

  SECTION("1") {

    REQUIRE(res.front().first == TokenType::Keyword);
    REQUIRE(res.front().second == "if");
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

TEST_CASE("Testing if with 2 parameters version 2") {
  const string input = "if p , p2; Select p";
  queue<pair<TokenType, string>> res;
  PQLLexer p(input);

  res = p.getTokenQueue();

  SECTION("1") {

    REQUIRE(res.front().first == TokenType::Keyword);
    REQUIRE(res.front().second == "if");
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

TEST_CASE("Testing stmt with 2 parameters version 1") {
  const string input = "stmt p, p2; Select p";
  queue<pair<TokenType, string>> res;
  PQLLexer p(input);

  res = p.getTokenQueue();

  SECTION("1") {

    REQUIRE(res.front().first == TokenType::Keyword);
    REQUIRE(res.front().second == "stmt");
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

TEST_CASE("Testing stmt with 2 parameters version 2") {
  const string input = "stmt p , p2; Select p";
  queue<pair<TokenType, string>> res;
  PQLLexer p(input);

  res = p.getTokenQueue();

  SECTION("1") {

    REQUIRE(res.front().first == TokenType::Keyword);
    REQUIRE(res.front().second == "stmt");
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

TEST_CASE("Testing assign with 2 parameters version 1") {
  const string input = "assign p, p2; Select p";
  queue<pair<TokenType, string>> res;
  PQLLexer p(input);

  res = p.getTokenQueue();

  SECTION("1") {

    REQUIRE(res.front().first == TokenType::Keyword);
    REQUIRE(res.front().second == "assign");
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

TEST_CASE("Testing assign with 2 parameters version 2") {
  const string input = "assign p , p2; Select p";
  queue<pair<TokenType, string>> res;
  PQLLexer p(input);

  res = p.getTokenQueue();

  SECTION("1") {

    REQUIRE(res.front().first == TokenType::Keyword);
    REQUIRE(res.front().second == "assign");
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

TEST_CASE("Testing constant with 2 parameters version 1") {
  const string input = "constant p, p2; Select p";
  queue<pair<TokenType, string>> res;
  PQLLexer p(input);

  res = p.getTokenQueue();

  SECTION("1") {

    REQUIRE(res.front().first == TokenType::Keyword);
    REQUIRE(res.front().second == "constant");
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

TEST_CASE("Testing constant with 2 parameters version 2") {
  const string input = "constant p , p2; Select p";
  queue<pair<TokenType, string>> res;
  PQLLexer p(input);

  res = p.getTokenQueue();

  SECTION("1") {

    REQUIRE(res.front().first == TokenType::Keyword);
    REQUIRE(res.front().second == "constant");
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

TEST_CASE("Testing procedure with 2 parameters version 1") {
  const string input = "procedure p, p2; Select p";
  queue<pair<TokenType, string>> res;
  PQLLexer p(input);

  res = p.getTokenQueue();

  SECTION("1") {

    REQUIRE(res.front().first == TokenType::Keyword);
    REQUIRE(res.front().second == "procedure");
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

TEST_CASE("Testing procedure with 2 parameters version 2") {
  const string input = "procedure p , p2; Select p";
  queue<pair<TokenType, string>> res;
  PQLLexer p(input);

  res = p.getTokenQueue();

  SECTION("1") {

    REQUIRE(res.front().first == TokenType::Keyword);
    REQUIRE(res.front().second == "procedure");
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

TEST_CASE("Testing procedure with 2 parameters version 3") {
  const string input = "procedure p , p2 ; Select p";
  queue<pair<TokenType, string>> res;
  PQLLexer p(input);

  res = p.getTokenQueue();

  SECTION("1") {

    REQUIRE(res.front().first == TokenType::Keyword);
    REQUIRE(res.front().second == "procedure");
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

TEST_CASE("Testing read with 2 parameters version 1") {
  const string input = "read p, p2; Select p";
  queue<pair<TokenType, string>> res;
  PQLLexer p(input);

  res = p.getTokenQueue();

  SECTION("1") {

    REQUIRE(res.front().first == TokenType::Keyword);
    REQUIRE(res.front().second == "read");
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

TEST_CASE("Testing read with 2 parameters version 2") {
  const string input = "read p , p2; Select p";
  queue<pair<TokenType, string>> res;
  PQLLexer p(input);

  res = p.getTokenQueue();

  SECTION("1") {

    REQUIRE(res.front().first == TokenType::Keyword);
    REQUIRE(res.front().second == "read");
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

TEST_CASE("Testing read with 2 parameters version 3") {
  const string input = "read p , p2 ; Select p";
  queue<pair<TokenType, string>> res;
  PQLLexer p(input);

  res = p.getTokenQueue();

  SECTION("1") {

    REQUIRE(res.front().first == TokenType::Keyword);
    REQUIRE(res.front().second == "read");
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

TEST_CASE("Testing variable v") {
  const string input = "variable v; Select v";
  queue<pair<TokenType, string>> res;
  PQLLexer p(input);

  res = p.getTokenQueue();

  SECTION("1") {

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
    REQUIRE(res.front().second == "v");
    res.pop();
  }
}

TEST_CASE("Testing variable v, select a with semicolumn") {
  const string input = "variable v; Select v;";

  SECTION("1") { REQUIRE_THROWS_WITH(PQLLexer(input), "no semicollumn"); }
}

TEST_CASE("Testing print p, select p with semicolumn") {
  const string input = "print p; Select p;";

  SECTION("1") { REQUIRE_THROWS_WITH(PQLLexer(input), "no semicollumn"); }
}

TEST_CASE("Testing while w") {
  const string input = "while w; Select w";
  queue<pair<TokenType, string>> res;
  PQLLexer p(input);

  res = p.getTokenQueue();

  SECTION("1") {

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
    REQUIRE(res.front().second == "w");
    res.pop();
  }
}

TEST_CASE("Testing while w, select w with semicolumn") {
  const string input = "while w; Select w;";

  SECTION("1") { REQUIRE_THROWS_WITH(PQLLexer(input), "no semicollumn"); }
}

TEST_CASE("Testing constant c") {
  const string input = "constant c; Select c";
  queue<pair<TokenType, string>> res;
  PQLLexer p(input);

  res = p.getTokenQueue();

  SECTION("1") {

    REQUIRE(res.front().first == TokenType::Keyword);
    REQUIRE(res.front().second == "constant");
    res.pop();
    REQUIRE(res.front().first == TokenType::Identifier);
    REQUIRE(res.front().second == "c");
    res.pop();
    REQUIRE(res.front().first == TokenType::Separator);
    REQUIRE(res.front().second == ";");
    res.pop();

    REQUIRE(res.front().first == TokenType::Keyword);
    REQUIRE(res.front().second == "Select");
    res.pop();
    REQUIRE(res.front().first == TokenType::Identifier);
    REQUIRE(res.front().second == "c");
    res.pop();
  }
}

TEST_CASE("Testing constant c, select c with semicolumn") {
  const string input = "constant c; Select c;";

  SECTION("1") { REQUIRE_THROWS_WITH(PQLLexer(input), "no semicollumn"); }
}

TEST_CASE("Testing Follows") {
  const string input = "assign a; while w; Select a such that Follows(w, a)";
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

TEST_CASE("Testing Follows(1, s) with semicollumn") {
  const string input =
      "assign a; stmt s; variable v; Select s such that Follows (1, s) ;";

  SECTION("1") { REQUIRE_THROWS_WITH(PQLLexer(input), "no semicollumn"); }
}

TEST_CASE("Testing Follows*(1, s)") {
  const string input =
      "assign a; stmt s; variable v; Select s such that Follows* (1, s)";
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
    REQUIRE(res.front().second == "Follows*");
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

TEST_CASE("Testing + pattern") {
  const string input = "assign a; Select a pattern a ( \"v + x * y + z * t\" , "
                       "\"v + x * y + z * t\")";
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
    REQUIRE(res.front().first == TokenType::Keyword);
    REQUIRE(res.front().second == "pattern");
    res.pop();
    REQUIRE(res.front().first == TokenType::Identifier);
    REQUIRE(res.front().second == "a");
    res.pop();
    REQUIRE(res.front().first == TokenType::Separator);
    REQUIRE(res.front().second == "(");
    res.pop();
    REQUIRE(res.front().first == TokenType::Separator);
    REQUIRE(res.front().second == "\"");
    res.pop();
    REQUIRE(res.front().first == TokenType::Identifier);
    REQUIRE(res.front().second == "v+x*y+z*t");
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
    REQUIRE(res.front().second == "v+x*y+z*t");
    res.pop();
    REQUIRE(res.front().first == TokenType::Separator);
    REQUIRE(res.front().second == "\"");
    res.pop();
    REQUIRE(res.front().first == TokenType::Separator);
    REQUIRE(res.front().second == ")");
    res.pop();
  }
}

TEST_CASE("Testing Parents(3, s)") {
  const string input =
      "assign a; stmt s; variable v; Select s such that Parent (3, s)";
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
    REQUIRE(res.front().second == "Parent");
    res.pop();
    REQUIRE(res.front().first == TokenType::Separator);
    REQUIRE(res.front().second == "(");
    res.pop();
    REQUIRE(res.front().first == TokenType::Identifier);
    REQUIRE(res.front().second == "3");
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

TEST_CASE("Testing Parent*(3, s)") {
  const string input =
      "assign a; stmt s; variable v; Select s such that Parent* (3, s)";
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
    REQUIRE(res.front().second == "Parent*");
    res.pop();
    REQUIRE(res.front().first == TokenType::Separator);
    REQUIRE(res.front().second == "(");
    res.pop();
    REQUIRE(res.front().first == TokenType::Identifier);
    REQUIRE(res.front().second == "3");
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

TEST_CASE("Testing Modifies(3, s)") {
  const string input =
      "assign a; stmt s; variable v; Select s such that Modifies (3, s)";
  queue<pair<TokenType, string>> res;
  PQLLexer p(input);

  res = p.getTokenQueue();

  SECTION("test modifes") {

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
    REQUIRE(res.front().second == "Modifies");
    res.pop();
    REQUIRE(res.front().first == TokenType::Separator);
    REQUIRE(res.front().second == "(");
    res.pop();
    REQUIRE(res.front().first == TokenType::Identifier);
    REQUIRE(res.front().second == "3");
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

TEST_CASE("Testing Modifies(a, 'x')") {
  const string input =
      "assign a; stmt s; variable v; Select a such that Modifies (a, \"x\")";
  queue<pair<TokenType, string>> res;
  PQLLexer p(input);

  res = p.getTokenQueue();

  SECTION("2") {

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
    REQUIRE(res.front().second == ")");
    res.pop();
  }
}

TEST_CASE("Testing Modifies('x', 'x')") {
  const string input = "assign a; stmt s; variable v; Select a such that "
                       "Modifies (\"x\", \"x\")";
  queue<pair<TokenType, string>> res;
  PQLLexer p(input);

  res = p.getTokenQueue();

  SECTION("2") {

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
    REQUIRE(res.front().second == ")");
    res.pop();
  }
}

TEST_CASE("Testing Uses with Quo") {
  const string input =
      "assign a; stmt s; variable v; Select a such that Uses (a, \"xa\")";
  queue<pair<TokenType, string>> res;
  PQLLexer p(input);

  res = p.getTokenQueue();

  SECTION("2") {

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
    REQUIRE(res.front().first == TokenType::Separator);
    REQUIRE(res.front().second == "\"");
    res.pop();
    REQUIRE(res.front().first == TokenType::Identifier);
    REQUIRE(res.front().second == "xa");
    res.pop();
    REQUIRE(res.front().first == TokenType::Separator);
    REQUIRE(res.front().second == "\"");
    res.pop();
    REQUIRE(res.front().first == TokenType::Separator);
    REQUIRE(res.front().second == ")");
    res.pop();
  }
}

TEST_CASE("Testing pattern with Quo") {
  const string input = "assign a , b , c , d ; Select a pattern a ( _ , \"v\")";
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
    REQUIRE(res.front().second == ",");
    res.pop();
    REQUIRE(res.front().first == TokenType::Identifier);
    REQUIRE(res.front().second == "b");
    res.pop();
    REQUIRE(res.front().first == TokenType::Separator);
    REQUIRE(res.front().second == ",");
    res.pop();
    REQUIRE(res.front().first == TokenType::Identifier);
    REQUIRE(res.front().second == "c");
    res.pop();
    REQUIRE(res.front().first == TokenType::Separator);
    REQUIRE(res.front().second == ",");
    res.pop();
    REQUIRE(res.front().first == TokenType::Identifier);
    REQUIRE(res.front().second == "d");
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
    REQUIRE(res.front().second == "pattern");
    res.pop();
    REQUIRE(res.front().first == TokenType::Identifier);
    REQUIRE(res.front().second == "a");
    res.pop();
    REQUIRE(res.front().first == TokenType::Separator);
    REQUIRE(res.front().second == "(");
    res.pop();
    REQUIRE(res.front().first == TokenType::Identifier);
    REQUIRE(res.front().second == "_");
    res.pop();
    REQUIRE(res.front().first == TokenType::Separator);
    REQUIRE(res.front().second == ",");
    res.pop();
    REQUIRE(res.front().first == TokenType::Separator);
    REQUIRE(res.front().second == "\"");
    res.pop();
    REQUIRE(res.front().first == TokenType::Identifier);
    REQUIRE(res.front().second == "v");
    res.pop();
    REQUIRE(res.front().first == TokenType::Separator);
    REQUIRE(res.front().second == "\"");
    res.pop();
    REQUIRE(res.front().first == TokenType::Separator);
    REQUIRE(res.front().second == ")");
    res.pop();
  }
}

TEST_CASE("Test wrong format pattern with variable on the right part.") {
  const string input = "assign a; variable v; Select a pattern a(\"x\", v);";

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

/*
SCENARIO("Only declare one variable a") {
  queue<Token> tokens;
  tokens.push(Token(TokenType::Identifier, "variable"));
  tokens.push(Token(TokenType::Identifier, "a"));
  tokens.push(Token(TokenType::Identifier, ";"));
  PQLParser p = PQLParser();
  Query q = p.buildQuery(tokens);
  std::vector<QueryEntity> Selectors = q.Selectors;
>>>>>>> develop:src/unit_testing/src/TestPQLParser.cpp

  WHEN("Parsed") {
    REQUIRE(Selectors.size() == 1);
    QueryEntity qe = Selectors.front();
    REQUIRE(qe.name == "a");
    REQUIRE(qe.type == QueryEntityType::Variable);
  }
}

SCENARIO("Declare 2 variables a and b") {
  queue<Token> tokens;
  tokens.push(Token(TokenType::Identifier, "variable"));
  tokens.push(Token(TokenType::Identifier, "a"));
  tokens.push(Token(TokenType::Identifier, ","));
  tokens.push(Token(TokenType::Identifier, "b"));
  tokens.push(Token(TokenType::Identifier, ";"));
  PQLParser p = PQLParser();
  Query q = p.buildQuery(tokens);
  std::vector<QueryEntity> Selectors = q.Selectors;

<<<<<<< HEAD:src/unit_testing/src/TestPQLLexer.cpp
TEST_CASE("Testing basic Select")
{
        const string input = "Select a";
        //queue<tuple<RelationType, string, string>> res;
        string t;
        PQLLexer p(input);
        //res = p.getSelectQueue();
        t = p.getTarget();
        SECTION("discover the targer is a") {

                REQUIRE(t == "a");
        }

}

TEST_CASE("Testing basic declaration variable and Select this variable")
{
        const string input = "variable a; Select a";
        queue<pair<DeclarationType, string>> resd;
        queue<tuple<RelationType, string, string>> ress;
        string t;

        PQLLexer p(input);
        resd = p.getDeclarationQueue();
        ress = p.getSelectQueue();
        t = p.getTarget();
                SECTION("Selectqueue's first pair is KEYWORD: variable") {

                REQUIRE(resd.front().first == DeclarationType::Variable);
                REQUIRE(resd.front().second == "a");
        }

        SECTION("Target is a") {

                REQUIRE(t == "a");
        }
=======
  WHEN("Parsed") {
    REQUIRE(Selectors.size() == 2);
    QueryEntity qe1 = Selectors.front();
    QueryEntity qe2 = Selectors[1];
    REQUIRE(qe1.name == "a");
    REQUIRE(qe1.type == QueryEntityType::Variable);
    REQUIRE(qe2.name == "b");
    REQUIRE(qe2.type == QueryEntityType::Variable);
  }
}

SCENARIO("Declare several different type of query entities, one for each") {
  queue<Token> tokens;
  tokens.push(Token(TokenType::Identifier, "variable"));
  tokens.push(Token(TokenType::Identifier, "a"));
  tokens.push(Token(TokenType::Identifier, ";"));
  tokens.push(Token(TokenType::Identifier, "procedure"));
  tokens.push(Token(TokenType::Identifier, "p"));
  tokens.push(Token(TokenType::Identifier, ";"));
  tokens.push(Token(TokenType::Identifier, "stmt"));
  tokens.push(Token(TokenType::Identifier, "s"));
  tokens.push(Token(TokenType::Identifier, ";"));
  PQLParser p = PQLParser();
  Query q = p.buildQuery(tokens);
  std::vector<QueryEntity> Selectors = q.Selectors;
>>>>>>> develop:src/unit_testing/src/TestPQLParser.cpp

  WHEN("Parsed") {
    REQUIRE(Selectors.size() == 3);
    QueryEntity qe1 = Selectors.front();
    QueryEntity qe2 = Selectors[1];
    QueryEntity qe3 = Selectors[2];
    REQUIRE(qe1.name == "a");
    REQUIRE(qe1.type == QueryEntityType::Variable);
    REQUIRE(qe2.name == "p");
    REQUIRE(qe2.type == QueryEntityType::Procedure);
    REQUIRE(qe3.name == "s");
    REQUIRE(qe3.type == QueryEntityType::Stmt);
  }
}

<<<<<<< HEAD:src/unit_testing/src/TestPQLLexer.cpp
TEST_CASE("Testing basic declaration procedure")
{
        const string input = "procedure a;";
        const string input1 = "Select a";
        queue<pair<DeclarationType, string>> res;
        queue<tuple<RelationType, string, string>> res1;
        string t;
        PQLLexer p(input);

        res = p.getDeclarationQueue();
        res1 = p.getSelectQueue();
        t = p.getTarget();
        SECTION("Selectqueue's first pair is Procedure: a") {

                REQUIRE(res.front().first == DeclarationType::Procedure);
                REQUIRE(res.front().second == "a");
        }
=======
SCENARIO("Declare several different type of query entities, several for each") {
  queue<Token> tokens;
  tokens.push(Token(TokenType::Identifier, "read"));
  tokens.push(Token(TokenType::Identifier, "r1"));
  tokens.push(Token(TokenType::Identifier, ","));
  tokens.push(Token(TokenType::Identifier, "r2"));
  tokens.push(Token(TokenType::Identifier, ";"));
  tokens.push(Token(TokenType::Identifier, "if"));
  tokens.push(Token(TokenType::Identifier, "ifs1"));
  tokens.push(Token(TokenType::Identifier, ","));
  tokens.push(Token(TokenType::Identifier, "ifs2"));
  tokens.push(Token(TokenType::Identifier, ";"));
  tokens.push(Token(TokenType::Identifier, "constant"));
  tokens.push(Token(TokenType::Identifier, "c1"));
  tokens.push(Token(TokenType::Identifier, ","));
  tokens.push(Token(TokenType::Identifier, "c2"));
  tokens.push(Token(TokenType::Identifier, ";"));
  PQLParser p = PQLParser();
  Query q = p.buildQuery(tokens);
  std::vector<QueryEntity> Selectors = q.Selectors;
>>>>>>> develop:src/unit_testing/src/TestPQLParser.cpp

  WHEN("Parsed") {
    REQUIRE(Selectors.size() == 6);
    QueryEntity qe1 = Selectors.front();
    QueryEntity qe2 = Selectors[1];
    QueryEntity qe3 = Selectors[2];
    QueryEntity qe4 = Selectors[3];
    QueryEntity qe5 = Selectors[4];
    QueryEntity qe6 = Selectors[5];
    REQUIRE(qe1.name == "r1");
    REQUIRE(qe1.type == QueryEntityType::Read);
    REQUIRE(qe2.name == "r2");
    REQUIRE(qe2.type == QueryEntityType::Read);
    REQUIRE(qe3.name == "ifs1");
    REQUIRE(qe3.type == QueryEntityType::If);
    REQUIRE(qe4.name == "ifs2");
    REQUIRE(qe4.type == QueryEntityType::If);
    REQUIRE(qe5.name == "c1");
    REQUIRE(qe5.type == QueryEntityType::Constant);
    REQUIRE(qe6.name == "c2");
    REQUIRE(qe6.type == QueryEntityType::Constant);
  }
}

SCENARIO("Select all variables") {
  queue<Token> tokens;
  tokens.push(Token(TokenType::Identifier, "variable"));
  tokens.push(Token(TokenType::Identifier, "a"));
  tokens.push(Token(TokenType::Identifier, ";"));
  tokens.push(Token(TokenType::Identifier, "Select"));
  tokens.push(Token(TokenType::Identifier, "a"));
  PQLParser p = PQLParser();
  Query q = p.buildQuery(tokens);
  std::vector<QueryEntity> Selectors = q.Selectors;
  QueryEntity tar = q.target;

<<<<<<< HEAD:src/unit_testing/src/TestPQLLexer.cpp
TEST_CASE("Testing basic declaration procedure and Select this procedure")
{
        const string input = "procedure a; Select a";
        queue<pair<DeclarationType, string>> resd;
        queue<tuple<RelationType, string, string>> ress;
        string t;
        PQLLexer p(input);
        resd = p.getDeclarationQueue();
        ress = p.getSelectQueue();
        t = p.getTarget();
        SECTION("Selectqueue's first pair is KEYWORD: procedure") {

                REQUIRE(resd.front().first == DeclarationType::Procedure);
                REQUIRE(resd.front().second == "a");
        }


        SECTION("Target is a") {

                REQUIRE(t == "a");
        }

}


  WHEN("Parsed") {
    REQUIRE(Selectors.size() == 1);
    QueryEntity qe = Selectors.front();
    REQUIRE(qe.name == "a");
    REQUIRE(qe.type == QueryEntityType::Variable);
    REQUIRE(tar.name == "a");
    REQUIRE(tar.type == QueryEntityType::Variable);
  }
}

SCENARIO("Including one such that claUse") {
  queue<Token> tokens;
  tokens.push(Token(TokenType::Identifier, "assign"));
  tokens.push(Token(TokenType::Identifier, "a"));
  tokens.push(Token(TokenType::Identifier, ";"));
  tokens.push(Token(TokenType::Identifier, "Select"));
  tokens.push(Token(TokenType::Identifier, "a"));
  tokens.push(Token(TokenType::Identifier, "such that"));
  tokens.push(Token(TokenType::Identifier, "Follows"));
  tokens.push(Token(TokenType::Identifier, "("));
  tokens.push(Token(TokenType::Identifier, "a"));
  tokens.push(Token(TokenType::Identifier, ","));
  tokens.push(Token(TokenType::Identifier, "5"));
  tokens.push(Token(TokenType::Identifier, ")"));
  PQLParser p = PQLParser();
  Query q = p.buildQuery(tokens);
  std::vector<QueryEntity> Selectors = q.Selectors;
  std::vector<ClaUse> claUses = q.claUses;
  QueryEntity tar = q.target;

  WHEN("Parsed") {
    REQUIRE(Selectors.size() == 1);
    QueryEntity qe = Selectors.front();
    REQUIRE(qe.name == "a");
    REQUIRE(qe.type == QueryEntityType::Assign);
    REQUIRE(tar.name == "a");
    REQUIRE(tar.type == QueryEntityType::Assign);
    REQUIRE(claUses.size() == 1);
    ClaUse c = claUses.front();
    std::vector<QueryEntity> parameters = c.parameters;
    REQUIRE(c.claUseType == ClaUseType::Follows);
    REQUIRE(parameters[0].name == "a");
    REQUIRE(parameters[0].type == QueryEntityType::Assign);
    REQUIRE(parameters[1].name == "5");
    REQUIRE(parameters[1].type == QueryEntityType::Line);
  }
}
>>>>>>> develop:src/unit_testing/src/TestPQLParser.cpp

// TEST_CASE("Testing basic declaration variable") {
//  const string input = "variable a;";
//  queue<pair<DeclarationType, string>> res;
//  PQLParser p(input);
//
//  res = p.getDeclarationQueue();
//  SECTION("Selectqueue's first pair is KEYWORD: variable") {
//
//    REQUIRE(res.front().first == DeclarationType::Variable);
//  }
//
//  SECTION("Selectqueue's second pair is Identifier: a") {
//    REQUIRE(res.front().second == "a");
//    res.pop();
//  }
//}
//
// TEST_CASE("Testing basic Select") {
//  const string input = "Select a";
//  // queue<tuple<RelationType, string, string>> res;
//  string t;
//  PQLParser p(input);
//  // res = p.getSelectQueue();
//  t = p.getTarget();
//  SECTION("discover the targer is a") { REQUIRE(t == "a"); }
//}
//
// TEST_CASE("Testing basic declaration variable and Select this variable") {
//  const string input = "variable a; Select a";
//  queue<pair<DeclarationType, string>> resd;
//  queue<tuple<RelationType, string, string>> ress;
//  string t;
//
//  PQLParser p(input);
//  resd = p.getDeclarationQueue();
//  ress = p.getSelectQueue();
//  t = p.getTarget();
//  SECTION("Selectqueue's first pair is KEYWORD: variable") {
//
//    REQUIRE(resd.front().first == DeclarationType::Variable);
//    REQUIRE(resd.front().second == "a");
//  }
//
//  SECTION("Target is a") { REQUIRE(t == "a"); }
//}
//
// TEST_CASE("Testing basic declaration procedure") {
//  const string input = "procedure a;";
//  const string input1 = "Select a";
//  queue<pair<DeclarationType, string>> res;
//  queue<tuple<RelationType, string, string>> res1;
//  string t;
//  PQLParser p(input);
//
//  res = p.getDeclarationQueue();
//  res1 = p.getSelectQueue();
//  t = p.getTarget();
//  SECTION("Selectqueue's first pair is Procedure: a") {
//
//    REQUIRE(res.front().first == DeclarationType::Procedure);
//    REQUIRE(res.front().second == "a");
//  }
//}
//
// TEST_CASE("Testing basic declaration procedure and Select this procedure") {
//  const string input = "procedure a; Select a";
//  queue<pair<DeclarationType, string>> resd;
//  queue<tuple<RelationType, string, string>> ress;
//  string t;
//  PQLParser p(input);
//  resd = p.getDeclarationQueue();
//  ress = p.getSelectQueue();s
//  t = p.getTarget();
//  SECTION("Selectqueue's first pair is KEYWORD: procedure") {
//
//    REQUIRE(resd.front().first == DeclarationType::Procedure);
//    REQUIRE(resd.front().second == "a");
//  }
//
//  SECTION("Target is a") { REQUIRE(t == "a"); }
//}
*/