
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
bool equals(vector<string> &a,vector<string> &b) {
	return a == b;
}

bool equals(QueryEntity a, QueryEntity b) {
	return a == b;
}

bool contains(vector<vector<string>> &s, vector<string> &q) {
	for(int i=0;i<s.size();i++) {
	  if(s[i]==q) {
		  return true;
	  }
	}
	return false;
}

bool equalsTable(vector<vector<string>> &s, vector<vector<string>> &q) {
	if (s.size() != q.size()) {
		return false;
	};
  for(int j = 0;j<q.size();j++) {
    if(!contains(s,q[j])) {
		return false;
    }
  }
  
  return true;
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
SCENARIO("target not in clauses") {
	PKB pkb;
	/*pkb.insertConstant();*/
  //stmt 1-9
	pkb.insertStatement("1", StatementType::Assign);
	pkb.insertStatement("2", StatementType::If);
	pkb.insertStatement("3", StatementType::While);
	pkb.insertStatement("4", StatementType::Stmt);
	pkb.insertStatement("5", StatementType::Call);
	pkb.insertStatement("6", StatementType::Print);
	pkb.insertStatement("7", StatementType::Read);
	pkb.insertStatement("8", StatementType::Assign);
	pkb.insertStatement("9", StatementType::Assign);
  //procedure: mian one two three four
	pkb.insertProc("main");
	pkb.insertProc("one");
	pkb.insertProc("two");
	pkb.insertProc("three");
	pkb.insertProc("four");
  //var a,x,y,z,i
	pkb.insertVar("a");
	pkb.insertVar("x");
	pkb.insertVar("y");
	pkb.insertVar("z");
	pkb.insertVar("i");
  //set uses
	pkb.setUses(1,"x");
	pkb.setUses(2, "a");
	pkb.setUses(4, "z");
	pkb.setUses(5, "i");
  //set modifies
	pkb.setModifies(1,"y");
	pkb.setModifies(2, "x");
	pkb.setModifies(3, "a");
	pkb.setModifies(4, "z");
  //set follows
	pkb.setFollows(1, 2);
	pkb.setFollows(2, 3);
	pkb.setFollows(4, 5);
	pkb.setFollows(5, 6);
  //set follow*
	pkb.setFollowsT(1,2);
	pkb.setFollowsT(1, 3);
	pkb.setFollowsT(4, 5);
	pkb.setFollowsT(4, 6);
  //set parent
	pkb.setParent(1, 2);
	pkb.setParent(2, 3);
	pkb.setParent(1, 4);
	pkb.setParent(5, 6);
  //set parent*
	pkb.setParentT(1, 2);
	pkb.setParentT(1, 3);
	pkb.setParentT(1, 4);
	pkb.setParentT(2, 3);
	pkb.setParentT(5, 6);
	PqlEvaluator pe(pkb);

	Query q;
	QueryEntity stmt(QueryEntityType::Stmt, "s");
	vector<QueryEntity> sele;
	vector<Clause> clause;
	sele.push_back(stmt);
	q.setQuery(stmt, sele, clause);
	list<string> result = pe.executeQuery(q);
}

SCENARIO("Merge Tables") {
  WHEN("join with empty table") {
	  QueryEntity a{ QueryEntityType::Stmt,"s" };
	  QueryEntity b{ QueryEntityType::Assign,"a" };
	  QueryEntity c{ QueryEntityType::Assign,"a1" };
	  vector<QueryEntity> title1{ a,b };
	  vector<QueryEntity> title2{ a,c };
	  vector<vector<string>> table1{ {"1","2"},{"1","3"},{"1","4"},{"2","3"},{"2","5"},{"7","8"} };
	  vector<vector<string>> table2;
	  ClauseResult c1(false, false, title1, table1);
	  ClauseResult c2(false, false, title2, table2);
	  vector<ClauseResult> clauseResutls{ c1,c2 };
	  MergeTables mt(clauseResutls);
	  ClauseResult finalTable = mt.getResultTables();
    SECTION("check title") {
		REQUIRE(finalTable.titles.size() == 3);
		REQUIRE(equals(finalTable.titles[0], a));
		REQUIRE(equals(finalTable.titles[1], b));
		REQUIRE(equals(finalTable.titles[2], c));
    }
    SECTION("check content") {
		REQUIRE(finalTable.resultTable.size() == 0);
    }
  }
  WHEN("no colum join") {
	  QueryEntity a{ QueryEntityType::Stmt,"s" };
	  QueryEntity b{ QueryEntityType::Assign,"a" };
	  QueryEntity c{ QueryEntityType::Assign,"a1" };
	  QueryEntity d{ QueryEntityType::While,"w" };
	  vector<QueryEntity> title1{ a,b };
	  vector<QueryEntity> title2{ c,d };
	  vector<vector<string>> table1{ {"1","2"},{"1","3"},{"1","4"}};
	  vector<vector<string>> table2{ {"1","2"},{"1","3"},{"1","4"}};
	  ClauseResult c1(false, false, title1, table1);
	  ClauseResult c2(false, false, title2, table2);
	  vector<ClauseResult> clauseResutls{ c1,c2 };
	  MergeTables mt(clauseResutls);
	  ClauseResult finalTable = mt.getResultTables();
	  SECTION("check title") {
		  REQUIRE(finalTable.titles.size() == 4);
		  REQUIRE(equals(finalTable.titles[0], a));
		  REQUIRE(equals(finalTable.titles[1], b));
		  REQUIRE(equals(finalTable.titles[2], c));
		  REQUIRE(equals(finalTable.titles[3], d));
	  }
	  SECTION("check content") {
		  vector<vector<string>> expected{{"1","2","1","2"},{"1","2","1","3"},{"1","2","1","4"},
		    {"1","3","1","2"},{"1","3","1","3"},{"1","3","1","4"},{"1","4","1","2"},
		    {"1","4","1","3"},{"1","4","1","4"} };
		  REQUIRE(finalTable.resultTable.size() == 9);
		  REQUIRE(equalsTable(finalTable.resultTable, expected));
	  }
  }
  WHEN("one column join") {
	  QueryEntity a{ QueryEntityType::Stmt,"s" };
	  QueryEntity b{ QueryEntityType::Assign,"a" };
	  QueryEntity c{ QueryEntityType::Assign,"a1" };
	  vector<QueryEntity> title1{ a,b };
	  vector<QueryEntity> title2{ a,c };
	  vector<vector<string>> table1{ {"1","2"},{"1","3"},{"1","4"},{"2","3"},{"2","5"},{"7","8"} };
	  vector<vector<string>> table2{ {"1","8"},{"1","9"},{"1","11"},{"2","4"},{"4","5"},{"4","8"} };
	  ClauseResult c1(false, false, title1, table1);
	  ClauseResult c2(false, false, title2, table2);
	  vector<ClauseResult> clauseResutls{ c1,c2 };
	  MergeTables mt(clauseResutls);
	  ClauseResult finalTable = mt.getResultTables();
	  SECTION("check titles") {
		  REQUIRE(finalTable.titles.size() == 3);
		  REQUIRE(equals(finalTable.titles[0], a));
		  REQUIRE(equals(finalTable.titles[1], b));
		  REQUIRE(equals(finalTable.titles[2], c));

	  }
	  SECTION("check content") {
		  vector<vector<string>> expected{ {"1","2","8"},{"1","3","8"} ,{"1","4","8"} ,{"1","2","9"},{"1","3","9"} ,{"1","4","9"},
		  {"1","2","11"},{"1","3","11"} ,{"1","4","11"} ,{"2","3","4"},{"2","5","4"} };
		  REQUIRE(equalsTable(finalTable.resultTable, expected));
	  }
  }
	


}
