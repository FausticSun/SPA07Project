
#include "PqlEvaluator.h"
#include  "MergeTables.h"
#include "catch.hpp"
#include <algorithm>
using namespace std;

bool contains(list<string> &s, string q) {
	list<string>::iterator iter = s.begin();
	for (iter; iter != s.end(); iter++) {
		if (*iter == q) {
			return true;
		}
	}
	return false;
}

SCENARIO("Simple Query") {
	PKB pkb;
	/*pkb.insertConstant();*/
	pkb.insertStatement("1",StatementType::Assign);
	pkb.insertStatement("2", StatementType::If);
	pkb.insertStatement("3", StatementType::While);
	pkb.insertStatement("4", StatementType::Stmt);
	pkb.insertStatement("5", StatementType::Call);
	pkb.insertStatement("6", StatementType::Print);
	pkb.insertStatement("7", StatementType::Read);
	pkb.insertStatement("8", StatementType::Assign);
	pkb.insertStatement("9", StatementType::Assign);
	pkb.insertProc("main");
	pkb.insertProc("one");
	pkb.insertProc("two");
	pkb.insertProc("three");
	pkb.insertProc("four");
	pkb.insertVar("a");
	pkb.insertVar("x");
	pkb.insertVar("y");
	pkb.insertVar("z");
	pkb.insertVar("i");
	PqlEvaluator pe(pkb);
  SECTION("Get stmt") {
	  Query q;
	  QueryEntity stmt(QueryEntityType::Stmt, "s");
	  vector<QueryEntity> sele;
	  vector<Clause> clause;
	  sele.push_back(stmt);
	  q.setQuery(stmt, sele, clause);
	  list<string> result = pe.executeQuery(q);
	  REQUIRE(result.size() == 9);
	  REQUIRE(contains(result,"1"));
	  REQUIRE(contains(result, "2"));
	  REQUIRE(contains(result, "3"));
	  REQUIRE(contains(result, "4"));
	  REQUIRE(contains(result, "5"));
	  REQUIRE(contains(result, "6"));
	  REQUIRE(contains(result, "7"));
	  REQUIRE(contains(result, "8"));
	  REQUIRE(contains(result, "9"));
  }
  SECTION("Get if") {
	  Query q;
	  QueryEntity stmt(QueryEntityType::If, "s");
	  vector<QueryEntity> sele;
	  vector<Clause> clause;
	  sele.push_back(stmt);
	  q.setQuery(stmt, sele, clause);
	  list<string> result = pe.executeQuery(q);
	  REQUIRE(result.size() == 1);
	  REQUIRE(contains(result, "2"));
  }
 SECTION("Get while") {
	  Query q;
	  QueryEntity stmt(QueryEntityType::While, "s");
	  vector<QueryEntity> sele;
	  vector<Clause> clause;
	  sele.push_back(stmt);
	  q.setQuery(stmt, sele, clause);
	  list<string> result = pe.executeQuery(q);
	  REQUIRE(result.size() == 1);
	  REQUIRE(contains(result, "3"));
  }
    SECTION("Get call") {
	  Query q;
	  QueryEntity stmt(QueryEntityType::Call, "s");
	  vector<QueryEntity> sele;
	  vector<Clause> clause;
	  sele.push_back(stmt);
	  q.setQuery(stmt, sele, clause);
	  list<string> result = pe.executeQuery(q);
	  REQUIRE(result.size() == 1);
	  REQUIRE(contains(result, "5"));
  }
  SECTION("Get print") {
	  Query q;
	  QueryEntity stmt(QueryEntityType::Print, "s");
	  vector<QueryEntity> sele;
	  vector<Clause> clause;
	  sele.push_back(stmt);
	  q.setQuery(stmt, sele, clause);
	  list<string> result = pe.executeQuery(q);
	  REQUIRE(result.size() == 1);
	  REQUIRE(contains(result, "6"));
  }
  SECTION("Get read") {
	  Query q;
	  QueryEntity stmt(QueryEntityType::Read, "s");
	  vector<QueryEntity> sele;
	  vector<Clause> clause;
	  sele.push_back(stmt);
	  q.setQuery(stmt, sele, clause);
	  list<string> result = pe.executeQuery(q);
	  REQUIRE(result.size() == 1);
	  REQUIRE(contains(result, "7"));
  }
  SECTION("Get assign") {
	  Query q;
	  QueryEntity stmt(QueryEntityType::Assign, "s");
	  vector<QueryEntity> sele;
	  vector<Clause> clause;
	  sele.push_back(stmt);
	  q.setQuery(stmt, sele, clause);
	  list<string> result = pe.executeQuery(q);
	  REQUIRE(result.size() == 3);
	  REQUIRE(contains(result, "1"));
	  REQUIRE(contains(result, "8"));
	  REQUIRE(contains(result, "9"));
  }
  SECTION("Get var") {
	  Query q;
	  QueryEntity var(QueryEntityType::Variable, "s");
	  vector<QueryEntity> sele;
	  vector<Clause> clause;
	  sele.push_back(var);
	  q.setQuery(var, sele, clause);
	  list<string> result = pe.executeQuery(q);
	  REQUIRE(result.size() == 5);
	  REQUIRE(contains(result, "i"));
	  REQUIRE(contains(result, "x"));
	  REQUIRE(contains(result, "y"));
	  REQUIRE(contains(result, "z"));
	  REQUIRE(contains(result, "a"));
  }
  SECTION("Get pro") {
	  Query q;
	  QueryEntity pro(QueryEntityType::Procedure, "s");
	  vector<QueryEntity> sele;
	  vector<Clause> clause;
	  sele.push_back(pro);
	  q.setQuery(pro, sele, clause);
	  list<string> result = pe.executeQuery(q);
	  REQUIRE(result.size() == 5);
	  REQUIRE(contains(result, "main"));
	  REQUIRE(contains(result, "one"));
	  REQUIRE(contains(result, "two"));
	  REQUIRE(contains(result, "three"));
	  REQUIRE(contains(result, "four"));
  }

  
}
