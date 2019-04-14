
#include "GeneralLexer.h"
#include "PQLEvaluator.h"
#include "PQLParser.h"
#include "catch.hpp"
#include <algorithm>
#include <sstream>
using namespace std;

bool contains(list<string> &s, string q) {
  auto iter = s.begin();
  for (iter; iter != s.end(); iter++) {
    if (*iter == q) {
      return true;
    }
  }
  return false;
}

bool equals(vector<string> &a, vector<string> &b) { return a == b; }

bool equals(QueryEntity a, QueryEntity b) { return a == b; }

bool contains(vector<vector<string>> &s, vector<string> &q) {
  for (int i = 0; i < s.size(); i++) {
    if (s[i] == q) {
      return true;
    }
  }
  return false;
}

bool equalsTable(vector<vector<string>> &s, vector<vector<string>> &q) {
  if (s.size() != q.size()) {
    return false;
  };
  for (int j = 0; j < q.size(); j++) {
    if (!contains(s, q[j])) {
      return false;
    }
  }

  return true;
}

PKB buildPKB() {
  PKB pkb;
  /*pkb.setConstant();*/
  // stmt
  pkb.setStmtType(1, StatementType::Assign);
  pkb.setStmtType(2, StatementType::Assign);
  pkb.setStmtType(3, StatementType::Assign);
  pkb.setStmtType(4, StatementType::If);
  pkb.setStmtType(5, StatementType::Call);
  pkb.setStmtType(6, StatementType::Call);
  pkb.setStmtType(7, StatementType::While);
  pkb.setStmtType(8, StatementType::If);
  pkb.setStmtType(9, StatementType::Assign);
  pkb.setStmtType(10, StatementType::Call);
  pkb.setStmtType(11, StatementType::Assign);
  pkb.setStmtType(12, StatementType::Assign);
  pkb.setStmtType(13, StatementType::Assign);
  pkb.setStmtType(14, StatementType::Print);
  pkb.setStmtType(15, StatementType::Read);

  // procedure: mian one two three four
  pkb.setProc("main", 1, 11);
  pkb.setProc("one", 11, 12);
  pkb.setProc("two", 12, 13);
  pkb.setProc("three", 13, 16);
  // var a,x,y,z,i
  pkb.setVar("x");
  pkb.setVar("a");
  pkb.setVar("b");
  pkb.setVar("c");
  // set uses
  pkb.setUses(1, "a");
  pkb.setUses(2, "b");
  pkb.setUses(3, "b");
  pkb.setUses(3, "a");
  pkb.setUses(4, "a");
  pkb.setUses(4, "b");
  pkb.setUses(5, "a");
  pkb.setUses(6, "b");
  pkb.setUses(7, "a");
  pkb.setUses(7, "x");
  pkb.setUses(7, "c");
  pkb.setUses(8, "x");
  pkb.setUses(8, "a");
  pkb.setUses(8, "c");
  pkb.setUses(9, "a");
  pkb.setUses(10, "c");
  pkb.setUses(11, "a");
  pkb.setUses(12, "b");
  pkb.setUses(13, "c");
  pkb.setUses(14, "c");

  pkb.setUses("main", "x");
  pkb.setUses("main", "a");
  pkb.setUses("main", "b");
  pkb.setUses("main", "c");
  pkb.setUses("one", "a");
  pkb.setUses("two", "b");
  pkb.setUses("three", "c");
  // set constant
  pkb.setConst(1);
  pkb.setConst(2);
  // set modifies
  pkb.setModifies(1, "x");
  pkb.setModifies(2, "a");
  pkb.setModifies(3, "x");
  pkb.setModifies(4, "x");
  pkb.setModifies(5, "x");
  pkb.setModifies(6, "x");
  pkb.setModifies(7, "a");
  pkb.setModifies(7, "x");
  pkb.setModifies(8, "x");
  pkb.setModifies(8, "a");
  pkb.setModifies(9, "a");
  pkb.setModifies(10, "x");
  pkb.setModifies(11, "x");
  pkb.setModifies(12, "x");
  pkb.setModifies(13, "x");
  pkb.setModifies(15, "x");

  pkb.setModifies("main", "x");
  pkb.setModifies("main", "a");
  pkb.setModifies("one", "x");
  pkb.setModifies("two", "x");
  pkb.setModifies("three", "x");
  // set follows
  pkb.setFollows(1, 2);
  pkb.setFollows(2, 3);
  pkb.setFollows(3, 4);
  pkb.setFollows(4, 7);
  pkb.setFollows(13, 14);
  pkb.setFollows(14, 15);

  // set follow*
  pkb.setFollowsT(1, 2);
  pkb.setFollowsT(1, 3);
  pkb.setFollowsT(1, 4);
  pkb.setFollowsT(1, 7);
  pkb.setFollowsT(2, 3);
  pkb.setFollowsT(2, 4);
  pkb.setFollowsT(2, 7);
  pkb.setFollowsT(3, 4);
  pkb.setFollowsT(3, 7);
  pkb.setFollowsT(4, 7);
  pkb.setFollowsT(13, 14);
  pkb.setFollowsT(13, 15);
  pkb.setFollowsT(14, 15);

  // set parent
  pkb.setParent(4, 5);
  pkb.setParent(4, 6);
  pkb.setParent(7, 8);
  pkb.setParent(8, 9);
  pkb.setParent(8, 10);
  // set parent*
  pkb.setParentT(4, 5);
  pkb.setParentT(4, 6);
  pkb.setParentT(7, 8);
  pkb.setParentT(7, 9);
  pkb.setParentT(7, 10);
  pkb.setParentT(8, 9);
  pkb.setParentT(8, 10);
  // set calls
  pkb.setCalls("main", "one");
  pkb.setCalls("main", "two");
  pkb.setCalls("main", "three");
  // set callsT
  pkb.setCalls("main", "one");
  pkb.setCalls("main", "two");
  pkb.setCalls("main", "three");
  // set call proc
  pkb.setCallProcName(5, "one");
  pkb.setCallProcName(6, "two");
  pkb.setCallProcName(10, "three");
  // set next
  pkb.setNext(1, 2);
  pkb.setNext(2, 3);
  pkb.setNext(3, 4);
  pkb.setNext(4, 5);
  pkb.setNext(4, 6);
  pkb.setNext(7, 8);
  pkb.setNext(8, 9);
  pkb.setNext(8, 10);
  pkb.setNext(13, 14);
  pkb.setNext(14, 15);

  return pkb;
}

// iteration 1
SCENARIO("Simple Query No Clause") {
  PKB pkb = buildPKB();
  PqlEvaluator pe(pkb);
  SECTION("Get stmt") {
    std::string pql = R"(
    stmt s;
    Select s
    )";
    std::stringstream ss;
    ss << pql;
    std::list<Token> tokens = Lexer::tokenize(ss);
    auto q = Parser::parsePQL(tokens);

    list<string> result;
    pe.executeQuery(q, result);
    REQUIRE(result.size() == 15);
    REQUIRE(contains(result, "1"));
    REQUIRE(contains(result, "2"));
    REQUIRE(contains(result, "3"));
    REQUIRE(contains(result, "4"));
    REQUIRE(contains(result, "5"));
    REQUIRE(contains(result, "6"));
    REQUIRE(contains(result, "7"));
    REQUIRE(contains(result, "8"));
    REQUIRE(contains(result, "9"));
    REQUIRE(contains(result, "10"));
    REQUIRE(contains(result, "11"));
    REQUIRE(contains(result, "12"));
    REQUIRE(contains(result, "13"));
    REQUIRE(contains(result, "14"));
    REQUIRE(contains(result, "15"));
  }
  SECTION("Get if") {
    std::string pql = R"(
    if ifs;
    Select ifs
    )";
    std::stringstream ss;
    ss << pql;
    std::list<Token> tokens = Lexer::tokenize(ss);
    auto q = Parser::parsePQL(tokens);

    list<string> result;
    pe.executeQuery(q, result);
    REQUIRE(result.size() == 2);
    REQUIRE(contains(result, "4"));
    REQUIRE(contains(result, "8"));
  }
  SECTION("Get while") {
    std::string pql = R"(
    while w;
    Select w
    )";
    std::stringstream ss;
    ss << pql;
    std::list<Token> tokens = Lexer::tokenize(ss);
    auto q = Parser::parsePQL(tokens);

    list<string> result;
    pe.executeQuery(q, result);
    REQUIRE(result.size() == 1);
    REQUIRE(contains(result, "7"));
  }
  SECTION("Get call") {
    std::string pql = R"(
    call c;
    Select c
    )";
    std::stringstream ss;
    ss << pql;
    std::list<Token> tokens = Lexer::tokenize(ss);
    auto q = Parser::parsePQL(tokens);

    list<string> result;
    pe.executeQuery(q, result);
    REQUIRE(result.size() == 3);
    REQUIRE(contains(result, "5"));
    REQUIRE(contains(result, "6"));
    REQUIRE(contains(result, "10"));
  }
  SECTION("Get print") {
    std::string pql = R"(
    print p;
    Select p
    )";
    std::stringstream ss;
    ss << pql;
    std::list<Token> tokens = Lexer::tokenize(ss);
    auto q = Parser::parsePQL(tokens);

    list<string> result;
    pe.executeQuery(q, result);
    REQUIRE(result.size() == 1);
    REQUIRE(contains(result, "14"));
  }
  SECTION("Get read") {
    std::string pql = R"(
    read r;
    Select r
    )";
    std::stringstream ss;
    ss << pql;
    std::list<Token> tokens = Lexer::tokenize(ss);
    auto q = Parser::parsePQL(tokens);

    list<string> result;
    pe.executeQuery(q, result);
    REQUIRE(result.size() == 1);
    REQUIRE(contains(result, "15"));
  }
  SECTION("Get assign") {
    std::string pql = R"(
    assign a;
    Select a
    )";
    std::stringstream ss;
    ss << pql;
    std::list<Token> tokens = Lexer::tokenize(ss);
    auto q = Parser::parsePQL(tokens);

    list<string> result;
    pe.executeQuery(q, result);
    REQUIRE(result.size() == 7);
    REQUIRE(contains(result, "1"));
    REQUIRE(contains(result, "2"));
    REQUIRE(contains(result, "3"));
    REQUIRE(contains(result, "9"));
    REQUIRE(contains(result, "11"));
    REQUIRE(contains(result, "12"));
    REQUIRE(contains(result, "13"));
  }
  SECTION("Get var") {
    std::string pql = R"(
    variable v;
    Select v
    )";
    std::stringstream ss;
    ss << pql;
    std::list<Token> tokens = Lexer::tokenize(ss);
    auto q = Parser::parsePQL(tokens);

    list<string> result;
    pe.executeQuery(q, result);
    REQUIRE(result.size() == 4);
    REQUIRE(contains(result, "x"));
    REQUIRE(contains(result, "a"));
    REQUIRE(contains(result, "b"));
    REQUIRE(contains(result, "c"));
  }
  SECTION("Get pro") {
    std::string pql = R"(
    procedure p;
    Select p
    )";
    std::stringstream ss;
    ss << pql;
    std::list<Token> tokens = Lexer::tokenize(ss);
    auto q = Parser::parsePQL(tokens);

    list<string> result;
    pe.executeQuery(q, result);
    REQUIRE(result.size() == 4);
    REQUIRE(contains(result, "main"));
    REQUIRE(contains(result, "one"));
    REQUIRE(contains(result, "two"));
    REQUIRE(contains(result, "three"));
  }
}

SCENARIO("target not in clauses") {
  PKB pkb = buildPKB();
  PqlEvaluator pe(pkb);
  SECTION("contains false clause") {
    std::string pql = R"(
    stmt s;
    Select s such that Uses(1, "x")
    )";
    std::stringstream ss;
    ss << pql;
    std::list<Token> tokens = Lexer::tokenize(ss);
    auto q = Parser::parsePQL(tokens);

    list<string> result;
    pe.executeQuery(q, result);
    REQUIRE(result.empty());
  }
  SECTION("all are true caluse") {
    std::string pql = R"(
    stmt s;
    Select s such that Uses(1, "a")
    )";
    std::stringstream ss;
    ss << pql;
    std::list<Token> tokens = Lexer::tokenize(ss);
    auto q = Parser::parsePQL(tokens);

    list<string> result;
    pe.executeQuery(q, result);
    REQUIRE(result.size() == 15);
    REQUIRE(contains(result, "1"));
    REQUIRE(contains(result, "2"));
    REQUIRE(contains(result, "3"));
    REQUIRE(contains(result, "4"));
    REQUIRE(contains(result, "5"));
    REQUIRE(contains(result, "6"));
    REQUIRE(contains(result, "7"));
    REQUIRE(contains(result, "8"));
    REQUIRE(contains(result, "9"));
    REQUIRE(contains(result, "10"));
    REQUIRE(contains(result, "11"));
    REQUIRE(contains(result, "12"));
    REQUIRE(contains(result, "13"));
    REQUIRE(contains(result, "14"));
    REQUIRE(contains(result, "15"));
  }
  SECTION("table is empty") {}
  SECTION("result is not empty") {}
}

SCENARIO("target in clause,one clause") {
  PKB pkb = buildPKB();
  PqlEvaluator pe(pkb);
  // repeated s,s notice
  WHEN("uses") {
    SECTION("cons,cons") {
      std::string pql = R"(
      Select BOOLEAN such that Uses(1, "a")
      )";
      std::stringstream ss;
      ss << pql;
      std::list<Token> tokens = Lexer::tokenize(ss);
      auto q = Parser::parsePQL(tokens);

      list<string> result;
      pe.executeQuery(q, result);
      REQUIRE(result.size() == 1);
      REQUIRE(contains(result, "TRUE"));
    }
  }
  WHEN("modifies") {
    SECTION("cons,s") {
      std::string pql = R"(
      variable v;
      Select v such that Modifies(1, v)
      )";
      std::stringstream ss;
      ss << pql;
      std::list<Token> tokens = Lexer::tokenize(ss);
      auto q = Parser::parsePQL(tokens);

      list<string> result;
      pe.executeQuery(q, result);
      REQUIRE(result.size() == 1);
      REQUIRE(contains(result, "x"));
    }
  }
  WHEN("parent") {
    SECTION("s,s") {
      std::string pql = R"(
      stmt s; if ifs;
      Select ifs such that Parent(s, ifs)
      )";
      std::stringstream ss;
      ss << pql;
      std::list<Token> tokens = Lexer::tokenize(ss);
      auto q = Parser::parsePQL(tokens);

      list<string> result;
      pe.executeQuery(q, result);
      REQUIRE(result.size() == 1);
      REQUIRE(contains(result, "8"));
    }
  }
  WHEN("parent*") {
    SECTION("s,_") {
      std::string pql = R"(
      while w; stmt s;
      Select w such that Parent*(w, _)
      )";
      std::stringstream ss;
      ss << pql;
      std::list<Token> tokens = Lexer::tokenize(ss);
      auto q = Parser::parsePQL(tokens);

      list<string> result;
      pe.executeQuery(q, result);
      REQUIRE(result.size() == 1);
      REQUIRE(contains(result, "7"));
    }
  }
  WHEN("follows") {
    SECTION("_,_") {
      std::string pql = R"(
      Select BOOLEAN such that Follows(1, _)
      )";
      std::stringstream ss;
      ss << pql;
      std::list<Token> tokens = Lexer::tokenize(ss);
      auto q = Parser::parsePQL(tokens);

      list<string> result;
      pe.executeQuery(q, result);
      REQUIRE(result.size() == 1);
      REQUIRE(contains(result, "TRUE"));
    }
  }
  WHEN("follows*") {
    SECTION("_,s") {
      std::string pql = R"(
      stmt s;
      Select s such that Follows*(_, s)
      )";
      std::stringstream ss;
      ss << pql;
      std::list<Token> tokens = Lexer::tokenize(ss);
      auto q = Parser::parsePQL(tokens);

      list<string> result;
      pe.executeQuery(q, result);
      REQUIRE(result.size() == 6);
      REQUIRE(contains(result, "2"));
      REQUIRE(contains(result, "3"));
      REQUIRE(contains(result, "4"));
      REQUIRE(contains(result, "7"));
      REQUIRE(contains(result, "14"));
      REQUIRE(contains(result, "15"));
    }
  }
}

// iteration 2
SCENARIO("simple select, no clauses") {
  PKB pkb = buildPKB();
  PqlEvaluator pe(pkb);
  SECTION("Select Boolean") {
    std::string pql = R"(
      Select BOOLEAN
      )";
    std::stringstream ss;
    ss << pql;
    std::list<Token> tokens = Lexer::tokenize(ss);
    auto q = Parser::parsePQL(tokens);

    list<string> result;
    pe.executeQuery(q, result);
    REQUIRE(result.size() == 1);
    REQUIRE(contains(result, "TRUE"));
  }
  SECTION("Select Tuple") {
    std::string pql = R"(
      procedure p; call c; while w;
      Select <p, c.procName, w>
      )";
    std::stringstream ss;
    ss << pql;
    std::list<Token> tokens = Lexer::tokenize(ss);
    auto q = Parser::parsePQL(tokens);

    list<string> result;
    pe.executeQuery(q, result);
    REQUIRE(result.size() == 12);
    REQUIRE(contains(result, "main one 7"));
    REQUIRE(contains(result, "main two 7"));
    REQUIRE(contains(result, "main three 7"));
    REQUIRE(contains(result, "one one 7"));
    REQUIRE(contains(result, "one two 7"));
    REQUIRE(contains(result, "one three 7"));
    REQUIRE(contains(result, "two one 7"));
    REQUIRE(contains(result, "two two 7"));
    REQUIRE(contains(result, "two three 7"));
    REQUIRE(contains(result, "three one 7"));
    REQUIRE(contains(result, "three two 7"));
    REQUIRE(contains(result, "three three 7"));
  }
}

SCENARIO("test one with clause evaluate") {
  PKB pkb = buildPKB();
  PqlEvaluator pe(pkb);
  SECTION("with attrref = synonym") {
    std::string pql = R"(
      prog_line pl; while w;
      Select pl with w.stmt# = pl
      )";
    std::stringstream ss;
    ss << pql;
    std::list<Token> tokens = Lexer::tokenize(ss);
    auto q = Parser::parsePQL(tokens);

    list<string> result;
    pe.executeQuery(q, result);
    REQUIRE(result.size() == 1);
    REQUIRE(contains(result, "7"));
  }
  SECTION("with attrRef = attrRef") {
    std::string pql = R"(
      while w; stmt s;
      Select <w, s> with w.stmt# = s.stmt#
      )";
    std::stringstream ss;
    ss << pql;
    std::list<Token> tokens = Lexer::tokenize(ss);
    auto q = Parser::parsePQL(tokens);

    list<string> result;
    pe.executeQuery(q, result);
    REQUIRE(result.size() == 1);
    REQUIRE(contains(result, "7 7"));
  }
  SECTION("with attrref = ident") {
    std::string pql = R"(
      read r;
      Select r with r.varName = "x"
      )";
    std::stringstream ss;
    ss << pql;
    std::list<Token> tokens = Lexer::tokenize(ss);
    auto q = Parser::parsePQL(tokens);

    list<string> result;
    pe.executeQuery(q, result);
    REQUIRE(result.size() == 1);
    REQUIRE(contains(result, "15"));
  }
  SECTION("with attrref = Integer") {
    std::string pql = R"(
      constant c;
      Select c with c.value = 1
      )";
    std::stringstream ss;
    ss << pql;
    std::list<Token> tokens = Lexer::tokenize(ss);
    auto q = Parser::parsePQL(tokens);

    list<string> result;
    pe.executeQuery(q, result);
    REQUIRE(result.size() == 1);
    REQUIRE(contains(result, "1"));
  }
  SECTION("with synonym = Integer") {
    std::string pql = R"(
      prog_line pl;
      Select pl with pl = 2
      )";
    std::stringstream ss;
    ss << pql;
    std::list<Token> tokens = Lexer::tokenize(ss);
    auto q = Parser::parsePQL(tokens);

    list<string> result;
    pe.executeQuery(q, result);
    REQUIRE(result.size() == 1);
    REQUIRE(contains(result, "2"));
  }
  SECTION("with iden = iden") {
    std::string pql = R"(
      while w;
      Select w with 1 = 1
      )";
    std::stringstream ss;
    ss << pql;
    std::list<Token> tokens = Lexer::tokenize(ss);
    auto q = Parser::parsePQL(tokens);

    list<string> result;
    pe.executeQuery(q, result);
    REQUIRE(result.size() == 1);
    REQUIRE(contains(result, "7"));
  }
}

SCENARIO("test one Next clause evaluate") {
  PKB pkb = buildPKB();
  PqlEvaluator pe(pkb);
  SECTION("(int, int)") {
    std::string pql = R"(
      Select BOOLEAN such that Next(1, 2)
      )";
    std::stringstream ss;
    ss << pql;
    std::list<Token> tokens = Lexer::tokenize(ss);
    auto q = Parser::parsePQL(tokens);

    list<string> result;
    pe.executeQuery(q, result);
    REQUIRE(result.size() == 1);
    REQUIRE(contains(result, "TRUE"));
  }
  SECTION("(syn, int)") {
    std::string pql = R"(
      while w;
      Select w such that Next(w, 8)
      )";
    std::stringstream ss;
    ss << pql;
    std::list<Token> tokens = Lexer::tokenize(ss);
    auto q = Parser::parsePQL(tokens);

    list<string> result;
    pe.executeQuery(q, result);
    REQUIRE(result.size() == 1);
    REQUIRE(contains(result, "7"));
  }
  SECTION("(syn, syn)") {
    std::string pql = R"(
      while w; stmt s;
      Select s such that Next(w, s)
      )";
    std::stringstream ss;
    ss << pql;
    std::list<Token> tokens = Lexer::tokenize(ss);
    auto q = Parser::parsePQL(tokens);

    list<string> result;
    pe.executeQuery(q, result);
    REQUIRE(result.size() == 1);
    REQUIRE(contains(result, "8"));
  }
  SECTION("(_, int)") {
    std::string pql = R"(
      Select BOOLEAN such that Next(_, 13)
      )";
    std::stringstream ss;
    ss << pql;
    std::list<Token> tokens = Lexer::tokenize(ss);
    auto q = Parser::parsePQL(tokens);

    list<string> result;
    pe.executeQuery(q, result);
    REQUIRE(result.size() == 1);
    REQUIRE(contains(result, "FALSE"));
  }
}

SCENARIO("test one Calls clause evaluate") {
  PKB pkb = buildPKB();
  PqlEvaluator pe(pkb);
  SECTION("(name, name)") {
    std::string pql = R"(
      Select BOOLEAN such that Calls("main", "one")
      )";
    std::stringstream ss;
    ss << pql;
    std::list<Token> tokens = Lexer::tokenize(ss);
    auto q = Parser::parsePQL(tokens);

    list<string> result;
    pe.executeQuery(q, result);
    REQUIRE(result.size() == 1);
    REQUIRE(contains(result, "TRUE"));
  }
  SECTION("(syn, name)") {
    std::string pql = R"(
      procedure p;
      Select p such that Calls(p, "one")
      )";
    std::stringstream ss;
    ss << pql;
    std::list<Token> tokens = Lexer::tokenize(ss);
    auto q = Parser::parsePQL(tokens);

    list<string> result;
    pe.executeQuery(q, result);
    REQUIRE(result.size() == 1);
    REQUIRE(contains(result, "main"));
  }
  SECTION("(syn, syn)") {
    std::string pql = R"(
      procedure p1, p2;
      Select p2 such that Calls(p1, p2)
      )";
    std::stringstream ss;
    ss << pql;
    std::list<Token> tokens = Lexer::tokenize(ss);
    auto q = Parser::parsePQL(tokens);

    list<string> result;
    pe.executeQuery(q, result);
    REQUIRE(result.size() == 3);
    REQUIRE(contains(result, "one"));
    REQUIRE(contains(result, "two"));
    REQUIRE(contains(result, "three"));
  }
  SECTION("(_, name)") {
    std::string pql = R"(
      Select BOOLEAN such that Calls(_, "three")
      )";
    std::stringstream ss;
    ss << pql;
    std::list<Token> tokens = Lexer::tokenize(ss);
    auto q = Parser::parsePQL(tokens);

    list<string> result;
    pe.executeQuery(q, result);
    REQUIRE(result.size() == 1);
    REQUIRE(contains(result, "TRUE"));
  }
}

SCENARIO("test one Calls* clause evaluate") {
  PKB pkb = buildPKB();
  PqlEvaluator pe(pkb);
  SECTION("(name,name)") {
    std::string pql = R"(
      Select BOOLEAN such that Calls("main", "one")
      )";
    std::stringstream ss;
    ss << pql;
    std::list<Token> tokens = Lexer::tokenize(ss);
    auto q = Parser::parsePQL(tokens);

    list<string> result;
    pe.executeQuery(q, result);
    REQUIRE(result.size() == 1);
    REQUIRE(contains(result, "TRUE"));
  }
  SECTION("(name,synonym)") {
    std::string pql = R"(
      procedure p;
      Select p such that Calls("main", p)
      )";
    std::stringstream ss;
    ss << pql;
    std::list<Token> tokens = Lexer::tokenize(ss);
    auto q = Parser::parsePQL(tokens);

    list<string> result;
    pe.executeQuery(q, result);
    REQUIRE(result.size() == 3);
    REQUIRE(contains(result, "one"));
    REQUIRE(contains(result, "two"));
    REQUIRE(contains(result, "three"));
  }
  SECTION("(name,_)") {
    std::string pql = R"(
      Select BOOLEAN such that Calls("main", _)
      )";
    std::stringstream ss;
    ss << pql;
    std::list<Token> tokens = Lexer::tokenize(ss);
    auto q = Parser::parsePQL(tokens);

    list<string> result;
    pe.executeQuery(q, result);
    REQUIRE(result.size() == 1);
    REQUIRE(contains(result, "TRUE"));
  }
  SECTION("(synonym,synonym)") {
    std::string pql = R"(
      procedure p1, p2;
      Select p1 such that Calls(p1, p2)
      )";
    std::stringstream ss;
    ss << pql;
    std::list<Token> tokens = Lexer::tokenize(ss);
    auto q = Parser::parsePQL(tokens);

    list<string> result;
    pe.executeQuery(q, result);
    REQUIRE(result.size() == 1);
    REQUIRE(contains(result, "main"));
  }
  SECTION("(synonym,name)") {
    std::string pql = R"(
      procedure p;
      Select BOOLEAN such that Calls(p, "main")
      )";
    std::stringstream ss;
    ss << pql;
    std::list<Token> tokens = Lexer::tokenize(ss);
    auto q = Parser::parsePQL(tokens);

    list<string> result;
    pe.executeQuery(q, result);
    REQUIRE(result.size() == 1);
    REQUIRE(contains(result, "FALSE"));
  }
  SECTION("(_,_)") {
    std::string pql = R"(
      procedure p;
      Select p such that Calls(_,_)
      )";
    std::stringstream ss;
    ss << pql;
    std::list<Token> tokens = Lexer::tokenize(ss);
    auto q = Parser::parsePQL(tokens);

    list<string> result;
    pe.executeQuery(q, result);
    REQUIRE(result.size() == 4);
    REQUIRE(contains(result, "main"));
    REQUIRE(contains(result, "one"));
    REQUIRE(contains(result, "two"));
    REQUIRE(contains(result, "three"));
  }
}
