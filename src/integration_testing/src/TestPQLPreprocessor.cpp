#include "catch.hpp"
#include <PQLLexer.h>
#include <PQLParser.h>
using namespace std;

SCENARIO("only declare one synonym") {
  PQLParser parser = PQLParser();
  queue<QueryToken> qq = parser.parse("variable a; Select a");
  parser.buildQuery(qq);
  Query q = parser.getQuery();
  REQUIRE(q.selectors.front().name == "a");
}
//
// SCENARIO("declare sevaral synonyms, one for each type") {
//	PQLParser parser = PQLParser();
//	queue<QueryToken> qq = parser.parse("variable a; stmt s; Select s");
//	parser.buildQuery(qq);
//	Query q = parser.getQuery();
//	REQUIRE(q.selectors.front().name == "a");
//	REQUIRE(q.selectors.front().type == QueryEntityType::Variable);
//	REQUIRE(q.selectors[1].name == "s");
//	REQUIRE(q.selectors[1].type == QueryEntityType::Stmt);
//}
//
// SCENARIO("declare sevaral synonyms, sevaral for each type") {
//	PQLParser parser = PQLParser();
//	queue<QueryToken> qq = parser.parse("while w1, w2; procedure p1, p2,
// p3;"); 	parser.buildQuery(qq); 	Query q = parser.getQuery();
//	REQUIRE(q.selectors.front().name == "w1");
//	REQUIRE(q.selectors.front().type == QueryEntityType::While);
//	REQUIRE(q.selectors[1].name == "w2");
//	REQUIRE(q.selectors[1].type == QueryEntityType::While);
//	REQUIRE(q.selectors[2].name == "p1");
//	REQUIRE(q.selectors[2].type == QueryEntityType::Procedure);
//	REQUIRE(q.selectors[3].name == "p2");
//	REQUIRE(q.selectors[3].type == QueryEntityType::Procedure);
//	REQUIRE(q.selectors[4].name == "p3");
//	REQUIRE(q.selectors[4].type == QueryEntityType::Procedure);
//}

SCENARIO("Select all variables") {
  PQLParser parser = PQLParser();
  queue<QueryToken> qq = parser.parse("variable a; Select a");
  parser.buildQuery(qq);
  Query q = parser.getQuery();
  REQUIRE(q.selectors.front().name == "a");
  REQUIRE(q.selectors.front().type == QueryEntityType::Variable);
  QueryEntity tar = q.target;
  REQUIRE(tar.name == "a");
  REQUIRE(tar.type == QueryEntityType::Variable);
}

SCENARIO("Including one such that clause") {
  PQLParser parser = PQLParser();
  queue<QueryToken> qq = parser.parse(
      "variable a1 , a2; if ifs; Select ifs such that Follows(ifs ,12)");
  parser.buildQuery(qq);
  Query q = parser.getQuery();
  REQUIRE(q.selectors.front().name == "a1");
  REQUIRE(q.selectors.front().type == QueryEntityType::Variable);
  QueryEntity tar = q.target;
  REQUIRE(tar.name == "ifs");
  REQUIRE(tar.type == QueryEntityType::If);
  REQUIRE(q.clauses.size() == 1);
  Clause c = q.clauses.front();
  std::vector<QueryEntity> parameters = c.parameters;
  REQUIRE(c.clauseType == ClauseType::Follows);
  REQUIRE(parameters[0].name == "ifs");
  REQUIRE(parameters[0].type == QueryEntityType::If);
  REQUIRE(parameters[1].name == "12");
  REQUIRE(parameters[1].type == QueryEntityType::Line);
}