#include "GeneralLexer.h"
#include "PQLParser.h"
#include "catch.hpp"
#include <sstream>

using Lexer::Token;
using Lexer::TokenType;

TEST_CASE("Empty query") {
  std::string pql = R"()";
  std::stringstream ss;
  ss << pql;
  std::list<Token> tokens = Lexer::tokenize(ss);
  REQUIRE_THROWS(Parser::parsePQL(tokens));
}

TEST_CASE("All delcarations") {
  std::string pql = R"(
  stmt s; read r; print p; call cl;
  while w; if ifs; assign a; variable v;
  constant c; prog_line pl; procedure pr;
  Select c
  )";
  std::stringstream ss;
  ss << pql;
  std::list<Token> tokens = Lexer::tokenize(ss);
  auto query = Parser::parsePQL(tokens);
}

TEST_CASE("Reused declaration") {
  std::string pql = R"(
  prog_line p; procedure p;
  Select p
  )";
  std::stringstream ss;
  ss << pql;
  std::list<Token> tokens = Lexer::tokenize(ss);
  REQUIRE_THROWS(Parser::parsePQL(tokens));
}

TEST_CASE("Multiple declarations") {
  std::string pql = R"(
  read r1, r2; print p1, p2;
  Select p1
  )";
  std::stringstream ss;
  ss << pql;
  std::list<Token> tokens = Lexer::tokenize(ss);
  auto query = Parser::parsePQL(tokens);
}

TEST_CASE("Invalid declaration") {
  std::string pql = R"(
  read r1, r2, print p1, p2;
  Select p1
  )";
  std::stringstream ss;
  ss << pql;
  std::list<Token> tokens = Lexer::tokenize(ss);
  REQUIRE_THROWS(Parser::parsePQL(tokens));
}

TEST_CASE("Single select") {
  std::string pql = R"(
  stmt s;
  Select s
  )";
  std::stringstream ss;
  ss << pql;
  std::list<Token> tokens = Lexer::tokenize(ss);
  auto query = Parser::parsePQL(tokens);
  auto selectors = query.selectors;
  REQUIRE(std::find(selectors.begin(), selectors.end(),
                    QueryEntity(QueryEntityType::Stmt, "s")) !=
          selectors.end());
}

TEST_CASE("Tuple select") {
  std::string pql = R"(
  stmt s1, s2, s3;
  Select <s1, s2, s3>
  )";
  std::stringstream ss;
  ss << pql;
  std::list<Token> tokens = Lexer::tokenize(ss);
  auto query = Parser::parsePQL(tokens);
  auto selectors = query.selectors;
  REQUIRE(std::find(selectors.begin(), selectors.end(),
                    QueryEntity(QueryEntityType::Stmt, "s1")) !=
          selectors.end());
  REQUIRE(std::find(selectors.begin(), selectors.end(),
                    QueryEntity(QueryEntityType::Stmt, "s2")) !=
          selectors.end());
  REQUIRE(std::find(selectors.begin(), selectors.end(),
                    QueryEntity(QueryEntityType::Stmt, "s3")) !=
          selectors.end());
}

TEST_CASE("Boolean select") {
  std::string pql = R"(
  Select BOOLEAN
  )";
  std::stringstream ss;
  ss << pql;
  std::list<Token> tokens = Lexer::tokenize(ss);
  auto query = Parser::parsePQL(tokens);
  auto selectors = query.selectors;
  REQUIRE(std::find(selectors.begin(), selectors.end(),
                    QueryEntity(QueryEntityType::Boolean)) != selectors.end());
}

TEST_CASE("Invalid selector") {
  std::string pql = R"(
  Select a
  )";
  std::stringstream ss;
  ss << pql;
  std::list<Token> tokens = Lexer::tokenize(ss);
  REQUIRE_THROWS(Parser::parsePQL(tokens));
}

TEST_CASE("Attribute select") {
  std::string pql = R"(
  stmt s;
  Select s.stmt#
  )";
  std::stringstream ss;
  ss << pql;
  std::list<Token> tokens = Lexer::tokenize(ss);
  auto query = Parser::parsePQL(tokens);
  auto selectors = query.selectors;
  REQUIRE(std::find(selectors.begin(), selectors.end(),
                    QueryEntity(QueryEntityType::Attrref, "s.stmt#",
                                QueryEntityType::Stmt)) != selectors.end());
}

TEST_CASE("Invalid attribute select") {
  std::string pql = R"(
  stmt s;
  Select s.value
  )";
  std::stringstream ss;
  ss << pql;
  std::list<Token> tokens = Lexer::tokenize(ss);
  REQUIRE_THROWS(Parser::parsePQL(tokens));
}

TEST_CASE("All delcarations while selecting all") {
  std::string pql = R"(
  stmt s; read r; print p; call cl;
  while w; if ifs; assign a; variable v;
  constant c; prog_line pl; procedure pr;
  Select <s, r, p, cl, w, ifs, a, v, c, pl, pr>
  )";
  std::stringstream ss;
  ss << pql;
  std::list<Token> tokens = Lexer::tokenize(ss);
  auto query = Parser::parsePQL(tokens);
  auto selectors = query.selectors;
  REQUIRE(std::find(selectors.begin(), selectors.end(),
                    QueryEntity(QueryEntityType::Stmt, "s")) !=
          selectors.end());
  REQUIRE(std::find(selectors.begin(), selectors.end(),
                    QueryEntity(QueryEntityType::Read, "r")) !=
          selectors.end());
  REQUIRE(std::find(selectors.begin(), selectors.end(),
                    QueryEntity(QueryEntityType::Print, "p")) !=
          selectors.end());
  REQUIRE(std::find(selectors.begin(), selectors.end(),
                    QueryEntity(QueryEntityType::Call, "cl")) !=
          selectors.end());
  REQUIRE(std::find(selectors.begin(), selectors.end(),
                    QueryEntity(QueryEntityType::While, "w")) !=
          selectors.end());
  REQUIRE(std::find(selectors.begin(), selectors.end(),
                    QueryEntity(QueryEntityType::If, "ifs")) !=
          selectors.end());
  REQUIRE(std::find(selectors.begin(), selectors.end(),
                    QueryEntity(QueryEntityType::Assign, "a")) !=
          selectors.end());
  REQUIRE(std::find(selectors.begin(), selectors.end(),
                    QueryEntity(QueryEntityType::Variable, "v")) !=
          selectors.end());
  REQUIRE(std::find(selectors.begin(), selectors.end(),
                    QueryEntity(QueryEntityType::Constant, "c")) !=
          selectors.end());
  REQUIRE(std::find(selectors.begin(), selectors.end(),
                    QueryEntity(QueryEntityType::Progline, "pl")) !=
          selectors.end());
  REQUIRE(std::find(selectors.begin(), selectors.end(),
                    QueryEntity(QueryEntityType::Procedure, "pr")) !=
          selectors.end());
}

TEST_CASE("Single such that relation") {
  std::string pql = R"(
  stmt s1, s2;
  Select <s1, s2>
  such that Follows(s1, s2)
  )";
  std::stringstream ss;
  ss << pql;
  std::list<Token> tokens = Lexer::tokenize(ss);
  auto query = Parser::parsePQL(tokens);
  auto clauses = query.clauses;
  std::vector<Clause> toVerify = {
      Clause(ClauseType::Follows, {QueryEntity(QueryEntityType::Stmt, "s1"),
                                   QueryEntity(QueryEntityType::Stmt, "s2")})};
  REQUIRE(clauses == toVerify);
}

TEST_CASE("All such that relation") {
  std::string pql = R"(
  stmt s1, s2; assign a1, a2; procedure p1, p2; variable v;
  Select <s1, s2>
  such that Follows(s1, s2)
  and Follows*(s1, s2)
  and Parent(s1, s2)
  and Parent*(s1, s2)
  and Uses(s1, v)
  and Uses(p1, v)
  and Modifies(s1, v)
  and Modifies(p1, v)
  and Calls(p1, p2)
  and Calls*(p1, p2)
  and Next(s1, s2)
  and Next*(s1, s2)
  and Affects(a1, a2)
  and Affects*(a1, a2)
  )";
  std::stringstream ss;
  ss << pql;
  std::list<Token> tokens = Lexer::tokenize(ss);
  auto query = Parser::parsePQL(tokens);
  auto clauses = query.clauses;
  std::vector<Clause> toVerify = {
      Clause(ClauseType::Follows, {QueryEntity(QueryEntityType::Stmt, "s1"),
                                   QueryEntity(QueryEntityType::Stmt, "s2")}),
      Clause(ClauseType::FollowsT, {QueryEntity(QueryEntityType::Stmt, "s1"),
                                    QueryEntity(QueryEntityType::Stmt, "s2")}),
      Clause(ClauseType::Parent, {QueryEntity(QueryEntityType::Stmt, "s1"),
                                  QueryEntity(QueryEntityType::Stmt, "s2")}),
      Clause(ClauseType::ParentT, {QueryEntity(QueryEntityType::Stmt, "s1"),
                                   QueryEntity(QueryEntityType::Stmt, "s2")}),
      Clause(ClauseType::UsesS, {QueryEntity(QueryEntityType::Stmt, "s1"),
                                 QueryEntity(QueryEntityType::Variable, "v")}),
      Clause(ClauseType::UsesS, {QueryEntity(QueryEntityType::Procedure, "p1"),
                                 QueryEntity(QueryEntityType::Variable, "v")}),
      Clause(ClauseType::ModifiesS,
             {QueryEntity(QueryEntityType::Stmt, "s1"),
              QueryEntity(QueryEntityType::Variable, "v")}),
      Clause(ClauseType::ModifiesS,
             {QueryEntity(QueryEntityType::Procedure, "p1"),
              QueryEntity(QueryEntityType::Variable, "v")}),
      Clause(ClauseType::Calls,
             {QueryEntity(QueryEntityType::Procedure, "p1"),
              QueryEntity(QueryEntityType::Procedure, "p2")}),
      Clause(ClauseType::CallsT,
             {QueryEntity(QueryEntityType::Procedure, "p1"),
              QueryEntity(QueryEntityType::Procedure, "p2")}),
      Clause(ClauseType::Next, {QueryEntity(QueryEntityType::Stmt, "s1"),
                                QueryEntity(QueryEntityType::Stmt, "s2")}),
      Clause(ClauseType::NextT, {QueryEntity(QueryEntityType::Stmt, "s1"),
                                 QueryEntity(QueryEntityType::Stmt, "s2")}),
      Clause(ClauseType::Affects, {QueryEntity(QueryEntityType::Assign, "a1"),
                                   QueryEntity(QueryEntityType::Assign, "a2")}),
      Clause(ClauseType::AffectsT,
             {QueryEntity(QueryEntityType::Assign, "a1"),
              QueryEntity(QueryEntityType::Assign, "a2")})};
  REQUIRE(clauses == toVerify);
}

TEST_CASE("Different entities in a relation") {
  std::string pql = R"(
  stmt s1, s2;
  Select <s1, s2>
  such that Follows(s1, s2)
  and Follows(_, s2)
  and Follows(s1, _)
  and Follows(_, _)
  and Follows(1, s2)
  and Follows(s1, 2)
  and Follows(1, 2)
  )";
  std::stringstream ss;
  ss << pql;
  std::list<Token> tokens = Lexer::tokenize(ss);
  auto query = Parser::parsePQL(tokens);
  auto clauses = query.clauses;
  std::vector<Clause> toVerify = {
      Clause(ClauseType::Follows, {QueryEntity(QueryEntityType::Stmt, "s1"),
                                   QueryEntity(QueryEntityType::Stmt, "s2")}),
      Clause(ClauseType::Follows,
             {QueryEntity(QueryEntityType::Underscore, "_"),
              QueryEntity(QueryEntityType::Stmt, "s2")}),
      Clause(ClauseType::Follows,
             {QueryEntity(QueryEntityType::Stmt, "s1"),
              QueryEntity(QueryEntityType::Underscore, "_")}),
      Clause(ClauseType::Follows,
             {QueryEntity(QueryEntityType::Underscore, "_"),
              QueryEntity(QueryEntityType::Underscore, "_")}),
      Clause(ClauseType::Follows, {QueryEntity(QueryEntityType::Line, "1"),
                                   QueryEntity(QueryEntityType::Stmt, "s2")}),
      Clause(ClauseType::Follows, {QueryEntity(QueryEntityType::Stmt, "s1"),
                                   QueryEntity(QueryEntityType::Line, "2")}),
      Clause(ClauseType::Follows, {QueryEntity(QueryEntityType::Line, "1"),
                                   QueryEntity(QueryEntityType::Line, "2")})};
  REQUIRE(clauses == toVerify);
}

TEST_CASE("Invalid _ in uses") {
  std::string pql = R"(
  variable v;
  such that Uses(_, v)
  )";
  std::stringstream ss;
  ss << pql;
  std::list<Token> tokens = Lexer::tokenize(ss);
  REQUIRE_THROWS(Parser::parsePQL(tokens));
}

TEST_CASE("Invalid _ in modifies") {
  std::string pql = R"(
  variable v;
  such that Modifies(_, v)
  )";
  std::stringstream ss;
  ss << pql;
  std::list<Token> tokens = Lexer::tokenize(ss);
  REQUIRE_THROWS(Parser::parsePQL(tokens));
}

TEST_CASE("Invalid entity in follows") {
  std::string pql = R"(
  variable v1, v2;
  such that Follows(v1, v2)
  )";
  std::stringstream ss;
  ss << pql;
  std::list<Token> tokens = Lexer::tokenize(ss);
  REQUIRE_THROWS(Parser::parsePQL(tokens));
}

TEST_CASE("Invalid entity in calls") {
  std::string pql = R"(
  stmt s1, s2;
  such that Calls(s1, s2)
  )";
  std::stringstream ss;
  ss << pql;
  std::list<Token> tokens = Lexer::tokenize(ss);
  REQUIRE_THROWS(Parser::parsePQL(tokens));
}

TEST_CASE("Single assign pattern") {
  std::string pql = R"(
  assign a; variable v;
  Select <a, v>
  pattern a(v,_)
  )";
  std::stringstream ss;
  ss << pql;
  std::list<Token> tokens = Lexer::tokenize(ss);
  auto query = Parser::parsePQL(tokens);
  auto clauses = query.clauses;
  std::vector<Clause> toVerify = {Clause(
      ClauseType::AssignPatt, {QueryEntity(QueryEntityType::Assign, "a"),
                               QueryEntity(QueryEntityType::Variable, "v"),
                               QueryEntity(QueryEntityType::Underscore, "_")})};
  REQUIRE(clauses == toVerify);
}

TEST_CASE("Various assign pattern") {
  std::string pql = R"(
  assign a; variable v;
  Select <a, v>
  pattern a(v,_)
  and a(v,"1+1")
  and a(v,_"1+1"_)
  and a("a",_)
  and a(_,_)
  )";
  std::stringstream ss;
  ss << pql;
  std::list<Token> tokens = Lexer::tokenize(ss);
  auto query = Parser::parsePQL(tokens);
  auto clauses = query.clauses;
  std::vector<Clause> toVerify = {
      Clause(ClauseType::AssignPatt,
             {QueryEntity(QueryEntityType::Assign, "a"),
              QueryEntity(QueryEntityType::Variable, "v"),
              QueryEntity(QueryEntityType::Underscore, "_")}),
      Clause(ClauseType::AssignPatt,
             {QueryEntity(QueryEntityType::Assign, "a"),
              QueryEntity(QueryEntityType::Variable, "v"),
              QueryEntity(QueryEntityType::Expression, " 1 1 + ")}),
      Clause(ClauseType::AssignPatt,
             {QueryEntity(QueryEntityType::Assign, "a"),
              QueryEntity(QueryEntityType::Variable, "v"),
              QueryEntity(QueryEntityType::Expression, "_ 1 1 + _")}),
      Clause(ClauseType::AssignPatt,
             {QueryEntity(QueryEntityType::Assign, "a"),
              QueryEntity(QueryEntityType::Name, "a"),
              QueryEntity(QueryEntityType::Underscore, "_")}),
      Clause(ClauseType::AssignPatt,
             {QueryEntity(QueryEntityType::Assign, "a"),
              QueryEntity(QueryEntityType::Underscore, "_"),
              QueryEntity(QueryEntityType::Underscore, "_")})};
  REQUIRE(clauses == toVerify);
}

TEST_CASE("Various patterns") {
  std::string pql = R"(
  assign a; variable v; while w; if ifs;
  Select <a, w, ifs>
  pattern a(v,_)
  and w(v,_)
  and ifs(v,_,_)
  )";
  std::stringstream ss;
  ss << pql;
  std::list<Token> tokens = Lexer::tokenize(ss);
  auto query = Parser::parsePQL(tokens);
  auto clauses = query.clauses;
  std::vector<Clause> toVerify = {
      Clause(ClauseType::AssignPatt,
             {QueryEntity(QueryEntityType::Assign, "a"),
              QueryEntity(QueryEntityType::Variable, "v"),
              QueryEntity(QueryEntityType::Underscore, "_")}),
      Clause(ClauseType::WhilePatt,
             {QueryEntity(QueryEntityType::While, "w"),
              QueryEntity(QueryEntityType::Variable, "v")}),
      Clause(ClauseType::IfPatt,
             {QueryEntity(QueryEntityType::If, "ifs"),
              QueryEntity(QueryEntityType::Variable, "v")})};
  REQUIRE(clauses == toVerify);
}

TEST_CASE("Single with clause") {
  std::string pql = R"(
  assign a1, a2;
  Select a1
  with a1.stmt# = a2.stmt#
  )";
  std::stringstream ss;
  ss << pql;
  std::list<Token> tokens = Lexer::tokenize(ss);
  auto query = Parser::parsePQL(tokens);
  auto clauses = query.clauses;
  std::vector<Clause> toVerify = {
      Clause(ClauseType::With,
             {QueryEntity(QueryEntityType::Attrref, "a1.stmt#",
                          QueryEntityType::Assign),
              QueryEntity(QueryEntityType::Attrref, "a2.stmt#",
                          QueryEntityType::Assign)}),
  };
  REQUIRE(clauses == toVerify);
}

TEST_CASE("Various with clause") {
  std::string pql = R"(
  procedure proc; call c; variable v;
  read r; print p; constant const; stmt s;
  while w; if ifs; assign a;
  prog_line pl;
  Select p
  with proc.procName = c.procName
  and v.varName = r.varName
  and p.varName = "abc"
  and const.value = s.stmt#
  and r.stmt# = p.stmt#
  and c.stmt# = w.stmt#
  and ifs.stmt# = a.stmt#
  and pl = 5
  )";
  std::stringstream ss;
  ss << pql;
  std::list<Token> tokens = Lexer::tokenize(ss);
  auto query = Parser::parsePQL(tokens);
  auto clauses = query.clauses;
  std::vector<Clause> toVerify = {
      Clause(ClauseType::With,
             {QueryEntity(QueryEntityType::Attrref, "proc.procName",
                          QueryEntityType::Procedure),
              QueryEntity(QueryEntityType::Attrref, "c.procName",
                          QueryEntityType::Call)}),
      Clause(ClauseType::With,
             {QueryEntity(QueryEntityType::Attrref, "v.varName",
                          QueryEntityType::Variable),
              QueryEntity(QueryEntityType::Attrref, "r.varName",
                          QueryEntityType::Read)}),
      Clause(ClauseType::With,
             {QueryEntity(QueryEntityType::Attrref, "p.varName",
                          QueryEntityType::Print),
              QueryEntity(QueryEntityType::Name, "abc")}),
      Clause(ClauseType::With,
             {QueryEntity(QueryEntityType::Attrref, "const.value",
                          QueryEntityType::Constant),
              QueryEntity(QueryEntityType::Attrref, "s.stmt#",
                          QueryEntityType::Stmt)}),
      Clause(ClauseType::With,
             {QueryEntity(QueryEntityType::Attrref, "r.stmt#",
                          QueryEntityType::Read),
              QueryEntity(QueryEntityType::Attrref, "p.stmt#",
                          QueryEntityType::Print)}),
      Clause(ClauseType::With,
             {QueryEntity(QueryEntityType::Attrref, "c.stmt#",
                          QueryEntityType::Call),
              QueryEntity(QueryEntityType::Attrref, "w.stmt#",
                          QueryEntityType::While)}),
      Clause(ClauseType::With,
             {QueryEntity(QueryEntityType::Attrref, "ifs.stmt#",
                          QueryEntityType::If),
              QueryEntity(QueryEntityType::Attrref, "a.stmt#",
                          QueryEntityType::Assign)}),
      Clause(ClauseType::With,
             {QueryEntity(QueryEntityType::Progline, "pl"),
              QueryEntity(QueryEntityType::Line, "5")})};
  REQUIRE(clauses == toVerify);
}
