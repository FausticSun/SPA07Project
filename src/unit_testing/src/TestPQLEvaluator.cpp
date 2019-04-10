
#include "PQLEvaluator.h"
#include "catch.hpp"
#include <algorithm>
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
    Query q;
    QueryEntity stmt(QueryEntityType::Stmt, "s");
    vector<QueryEntity> targets;
    vector<QueryEntity> sele;
    vector<Clause> clause;
    targets.push_back(stmt);
    sele.push_back(stmt);
    q.setQuery(targets, sele, clause);
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
    Query q;
    QueryEntity stmt(QueryEntityType::If, "s");
    vector<QueryEntity> targets;
    vector<QueryEntity> sele;
    vector<Clause> clause;
    targets.push_back(stmt);
    sele.push_back(stmt);
    q.setQuery(targets, sele, clause);
    list<string> result;
    pe.executeQuery(q, result);
    REQUIRE(result.size() == 2);
    REQUIRE(contains(result, "4"));
    REQUIRE(contains(result, "8"));
  }
  SECTION("Get while") {
    Query q;
    QueryEntity stmt(QueryEntityType::While, "s");
    vector<QueryEntity> targets;
    vector<QueryEntity> sele;
    vector<Clause> clause;
    targets.push_back(stmt);
    sele.push_back(stmt);
    q.setQuery(targets, sele, clause);
    list<string> result;
    pe.executeQuery(q, result);
    REQUIRE(result.size() == 1);
    REQUIRE(contains(result, "7"));
  }
  SECTION("Get call") {
    Query q;
    QueryEntity stmt(QueryEntityType::Call, "s");
    vector<QueryEntity> targets;
    vector<QueryEntity> sele;
    vector<Clause> clause;
    targets.push_back(stmt);
    sele.push_back(stmt);
    q.setQuery(targets, sele, clause);
    list<string> result;
    pe.executeQuery(q, result);
    REQUIRE(result.size() == 3);
    REQUIRE(contains(result, "5"));
    REQUIRE(contains(result, "6"));
    REQUIRE(contains(result, "10"));
  }
  SECTION("Get print") {
    Query q;
    QueryEntity stmt(QueryEntityType::Print, "s");
    vector<QueryEntity> targets;
    vector<QueryEntity> sele;
    vector<Clause> clause;
    targets.push_back(stmt);
    sele.push_back(stmt);
    q.setQuery(targets, sele, clause);
    list<string> result;
    pe.executeQuery(q, result);
    REQUIRE(result.size() == 1);
    REQUIRE(contains(result, "14"));
  }
  SECTION("Get read") {
    Query q;
    QueryEntity stmt(QueryEntityType::Read, "s");
    vector<QueryEntity> targets;
    vector<QueryEntity> sele;
    vector<Clause> clause;
    targets.push_back(stmt);
    sele.push_back(stmt);
    q.setQuery(targets, sele, clause);
    list<string> result;
    pe.executeQuery(q, result);
    REQUIRE(result.size() == 1);
    REQUIRE(contains(result, "15"));
  }
  SECTION("Get assign") {
    Query q;
    QueryEntity stmt(QueryEntityType::Assign, "s");
    vector<QueryEntity> targets;
    vector<QueryEntity> sele;
    vector<Clause> clause;
    targets.push_back(stmt);
    sele.push_back(stmt);
    q.setQuery(targets, sele, clause);
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
    Query q;
    QueryEntity var(QueryEntityType::Variable, "s");
    vector<QueryEntity> targets;
    vector<QueryEntity> sele;
    vector<Clause> clause;
    targets.push_back(var);
    sele.push_back(var);
    q.setQuery(targets, sele, clause);
    list<string> result;
    pe.executeQuery(q, result);
    REQUIRE(result.size() == 4);
    REQUIRE(contains(result, "x"));
    REQUIRE(contains(result, "a"));
    REQUIRE(contains(result, "b"));
    REQUIRE(contains(result, "c"));
  }
  SECTION("Get pro") {
    Query q;
    QueryEntity pro(QueryEntityType::Procedure, "s");
    vector<QueryEntity> targets;
    vector<QueryEntity> sele;
    vector<Clause> clause;
    targets.push_back(pro);
    sele.push_back(pro);
    q.setQuery(targets, sele, clause);
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
    Query q;
    QueryEntity stm(QueryEntityType::Stmt, "s");
    QueryEntity s1(QueryEntityType::Line, "1");
    QueryEntity s2(QueryEntityType::Name, "x");
    vector<QueryEntity> qes{s1, s2};
    Clause c(ClauseType::UsesS, qes);
    vector<QueryEntity> targets;
    vector<QueryEntity> sele;
    vector<Clause> clause;
    targets.push_back(stm);
    sele.push_back(stm);
    clause.push_back(c);
    q.setQuery(targets, sele, clause);
    list<string> result;
    pe.executeQuery(q, result);
    REQUIRE(result.empty());
  }
  SECTION("all are true caluse") {
    Query q;
    QueryEntity stm(QueryEntityType::Stmt, "s");
    QueryEntity s1(QueryEntityType::Line, "1");
    QueryEntity s2(QueryEntityType::Name, "a");
    vector<QueryEntity> qes{s1, s2};
    Clause c(ClauseType::UsesS, qes);
    vector<QueryEntity> sele;
    vector<QueryEntity> targets;
    vector<Clause> clause;
    targets.push_back(stm);
    sele.push_back(stm);
    clause.push_back(c);
    q.setQuery(targets, sele, clause);
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
      Query q;
      QueryEntity n1(QueryEntityType::Line, "1");
      QueryEntity n2(QueryEntityType::Name, "a");
      QueryEntity boolean(QueryEntityType::Boolean, "");
      QueryEntity t1(QueryEntityType::Stmt, "s");
      Clause c1(ClauseType::UsesS, vector<QueryEntity>{n1, n2});
      vector<QueryEntity> targets;
      vector<QueryEntity> sele;
      vector<Clause> clause;
      targets.push_back(boolean);
      clause.push_back(c1);
      q.setQuery(targets, sele, clause);
      list<string> result;
      pe.executeQuery(q, result);
      REQUIRE(result.size() == 1);
      REQUIRE(contains(result, "TRUE"));
    }
  }
  WHEN("modifies") {
    SECTION("cons,s") {
      Query q;
      QueryEntity n1(QueryEntityType::Line, "1");
      QueryEntity n2(QueryEntityType::Variable, "v");
      QueryEntity boolean(QueryEntityType::Boolean, "");
      QueryEntity t1(QueryEntityType::Stmt, "s");
      Clause c1(ClauseType::ModifiesS, vector<QueryEntity>{n1, n2});
      vector<QueryEntity> targets;
      vector<QueryEntity> sele;
      vector<Clause> clause;
      sele.push_back(n2);
      targets.push_back(n2);
      clause.push_back(c1);
      q.setQuery(targets, sele, clause);
      list<string> result;
      pe.executeQuery(q, result);
      REQUIRE(result.size() == 1);
      REQUIRE(contains(result, "x"));
    }
  }
  WHEN("parent") {
    SECTION("s,s") {
      Query q;
      QueryEntity n1(QueryEntityType::Stmt, "s");
      QueryEntity n2(QueryEntityType::If, "ifs");
      QueryEntity boolean(QueryEntityType::Boolean, "");
      Clause c1(ClauseType::Parent, vector<QueryEntity>{n1, n2});
      vector<QueryEntity> targets;
      vector<QueryEntity> sele;
      vector<Clause> clause;
      sele.push_back(n1);
      sele.push_back(n2);
      targets.push_back(n2);
      clause.push_back(c1);
      q.setQuery(targets, sele, clause);
      list<string> result;
      pe.executeQuery(q, result);
      REQUIRE(result.size() == 1);
      REQUIRE(contains(result, "8"));
    }
  }
  WHEN("parent*") {
    SECTION("s,_") {
      Query q;
      QueryEntity n1(QueryEntityType::While, "w");
      QueryEntity n2(QueryEntityType::Underscore, "_");
      QueryEntity boolean(QueryEntityType::Boolean, "");
      QueryEntity t1(QueryEntityType::Stmt, "s");
      Clause c1(ClauseType::ParentT, vector<QueryEntity>{n1, n2});
      vector<QueryEntity> targets;
      vector<QueryEntity> sele;
      vector<Clause> clause;
      sele.push_back(n1);
      targets.push_back(n1);
      clause.push_back(c1);
      q.setQuery(targets, sele, clause);
      list<string> result;
      pe.executeQuery(q, result);
      REQUIRE(result.size() == 1);
      REQUIRE(contains(result, "7"));
    }
  }
  WHEN("follows") {
    SECTION("_,_") {
      Query q;
      QueryEntity n1(QueryEntityType::Line, "1");
      QueryEntity n2(QueryEntityType::Underscore, "_");
      QueryEntity boolean(QueryEntityType::Boolean, "");
      QueryEntity t1(QueryEntityType::Stmt, "s");
      Clause c1(ClauseType::Follows, vector<QueryEntity>{n2, n2});
      vector<QueryEntity> targets;
      vector<QueryEntity> sele;
      vector<Clause> clause;
      sele.push_back(n2);
      targets.push_back(boolean);
      clause.push_back(c1);
      q.setQuery(targets, sele, clause);
      list<string> result;
      pe.executeQuery(q, result);
      REQUIRE(result.size() == 1);
      REQUIRE(contains(result, "TRUE"));
    }
  }
  WHEN("follows*") {
    SECTION("_,s") {
      Query q;
      QueryEntity n1(QueryEntityType::Underscore, "_");
      QueryEntity n2(QueryEntityType::Stmt, "s");
      QueryEntity boolean(QueryEntityType::Boolean, "");
      QueryEntity t1(QueryEntityType::Stmt, "s");
      Clause c1(ClauseType::FollowsT, vector<QueryEntity>{n1, n2});
      vector<QueryEntity> targets;
      vector<QueryEntity> sele;
      vector<Clause> clause;
      sele.push_back(n2);
      targets.push_back(n2);
      clause.push_back(c1);
      q.setQuery(targets, sele, clause);
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
    Query q;
    QueryEntity boolean(QueryEntityType::Boolean, "");
    QueryEntity ass(QueryEntityType::Assign, "a");
    vector<QueryEntity> targets;
    vector<QueryEntity> sele;
    vector<Clause> clause;
    targets.push_back(boolean);
    sele.push_back(ass);
    q.setQuery(targets, sele, clause);
    list<string> result;
    pe.executeQuery(q, result);
    REQUIRE(result.size() == 1);
    REQUIRE(contains(result, "TRUE"));
  }
  SECTION("Select Tuple") {
    Query q;
    QueryEntity p(QueryEntityType::Procedure, "p");
    QueryEntity c(QueryEntityType::Call, "c");
    QueryEntity w(QueryEntityType::While, "w");
    QueryEntity cp(QueryEntityType::Attrref, "c.procName",
                   QueryEntityType::Call);
    vector<QueryEntity> targets;
    vector<QueryEntity> sele;
    vector<Clause> clause;
    targets.push_back(p);
    targets.push_back(cp);
    targets.push_back(w);
    sele.push_back(p);
    sele.push_back(cp);
    sele.push_back(c);
    sele.push_back(w);
    q.setQuery(targets, sele, clause);
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
    Query q;
    QueryEntity w(QueryEntityType::While, "w");
    QueryEntity pl(QueryEntityType::Progline, "pl");
    QueryEntity firstPara(QueryEntityType::Attrref, "w.stmt#",
                          QueryEntityType::While);
    Clause with(ClauseType::With, vector<QueryEntity>{firstPara, pl});
    vector<QueryEntity> targets;
    vector<QueryEntity> sele;
    vector<Clause> clause;
    targets.push_back(pl);
    sele.push_back(w);
    sele.push_back(pl);
    clause.push_back(with);
    q.setQuery(targets, sele, clause);
    list<string> result;
    pe.executeQuery(q, result);
    REQUIRE(result.size() == 1);
    REQUIRE(contains(result, "7"));
  }
  SECTION("with attrRef = attrRef") {
    Query q;
    QueryEntity w(QueryEntityType::While, "w");
    QueryEntity s(QueryEntityType::Stmt, "s");
    QueryEntity firstPara(QueryEntityType::Attrref, "w.stmt#",
                          QueryEntityType::While);
    QueryEntity secondPara(QueryEntityType::Attrref, "s.stmt#",
                           QueryEntityType::Stmt);
    Clause with1(ClauseType::With, vector<QueryEntity>{firstPara, secondPara});
    vector<QueryEntity> targets;
    vector<QueryEntity> sele;
    vector<Clause> clause;
    targets.push_back(w);
    targets.push_back(s);
    sele.push_back(w);
    sele.push_back(secondPara);
    clause.push_back(with1);
    q.setQuery(targets, sele, clause);
    list<string> result;
    pe.executeQuery(q, result);
    REQUIRE(result.size() == 1);
    REQUIRE(contains(result, "7 7"));
  }
  SECTION("with attrref = ident") {
    Query q;
    QueryEntity r(QueryEntityType::Read, "r");
    QueryEntity name(QueryEntityType::Name, "x");
    QueryEntity firstPara(QueryEntityType::Attrref, "r.varName",
                          QueryEntityType::Read);
    Clause with(ClauseType::With, vector<QueryEntity>{firstPara, name});
    vector<QueryEntity> targets;
    vector<QueryEntity> sele;
    vector<Clause> clause;
    targets.push_back(r);
    sele.push_back(r);
    clause.push_back(with);
    q.setQuery(targets, sele, clause);
    list<string> result;
    pe.executeQuery(q, result);
    REQUIRE(result.size() == 1);
    REQUIRE(contains(result, "15"));
  }
  SECTION("with attrref = Integer") {
    Query q;
    QueryEntity c(QueryEntityType::Constant, "c");
    QueryEntity INT(QueryEntityType::Line, "1");
    QueryEntity firstPara(QueryEntityType::Attrref, "c.value",
                          QueryEntityType::Constant);
    Clause with(ClauseType::With, vector<QueryEntity>{firstPara, INT});
    vector<QueryEntity> targets;
    vector<QueryEntity> sele;
    vector<Clause> clause;
    targets.push_back(c);
    sele.push_back(c);
    clause.push_back(with);
    q.setQuery(targets, sele, clause);
    list<string> result;
    pe.executeQuery(q, result);
    REQUIRE(result.size() == 1);
    REQUIRE(contains(result, "1"));
  }
  SECTION("with synonym = Integer") {
    Query q;
    QueryEntity pl(QueryEntityType::Progline, "pl");
    QueryEntity INT(QueryEntityType::Line, "2");
    Clause with(ClauseType::With, vector<QueryEntity>{pl, INT});
    vector<QueryEntity> targets;
    vector<QueryEntity> sele;
    vector<Clause> clause;
    targets.push_back(pl);
    sele.push_back(pl);
    clause.push_back(with);
    q.setQuery(targets, sele, clause);
    list<string> result;
    pe.executeQuery(q, result);
    REQUIRE(result.size() == 1);
    REQUIRE(contains(result, "2"));
  }
  SECTION("with iden = iden") {
    Query q;
    QueryEntity iden1(QueryEntityType::Line, "1");
    QueryEntity iden2(QueryEntityType::Line, "1");
    QueryEntity while1(QueryEntityType::While, "w");
    Clause with(ClauseType::With, vector<QueryEntity>{iden1, iden2});
    vector<QueryEntity> targets;
    vector<QueryEntity> sele;
    vector<Clause> clause;
    targets.push_back(while1);
    sele.push_back(while1);
    clause.push_back(with);
    q.setQuery(targets, sele, clause);
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
    Query q;
    QueryEntity Int1(QueryEntityType::Line, "1");
    QueryEntity Int2(QueryEntityType::Line, "2");
    QueryEntity boolean(QueryEntityType::Boolean, "");
    Clause next(ClauseType::Next, vector<QueryEntity>{Int1, Int2});
    vector<QueryEntity> targets;
    vector<QueryEntity> sele;
    vector<Clause> clause;
    targets.push_back(boolean);
    clause.push_back(next);
    q.setQuery(targets, sele, clause);
    list<string> result;
    pe.executeQuery(q, result);
    REQUIRE(result.size() == 1);
    REQUIRE(contains(result, "TRUE"));
  }
  SECTION("(syn, int)") {
    Query q;
    QueryEntity w(QueryEntityType::While, "w");
    QueryEntity Int(QueryEntityType::Line, "8");
    Clause next(ClauseType::Next, vector<QueryEntity>{w, Int});
    vector<QueryEntity> targets;
    vector<QueryEntity> sele;
    vector<Clause> clause;
    targets.push_back(w);
    sele.push_back(w);
    clause.push_back(next);
    q.setQuery(targets, sele, clause);
    list<string> result;
    pe.executeQuery(q, result);
    REQUIRE(result.size() == 1);
    REQUIRE(contains(result, "7"));
  }
  SECTION("(syn, syn)") {
    Query q;
    QueryEntity w(QueryEntityType::While, "w");
    QueryEntity s(QueryEntityType::Stmt, "s");
    Clause next(ClauseType::Next, vector<QueryEntity>{w, s});
    vector<QueryEntity> targets;
    vector<QueryEntity> sele;
    vector<Clause> clause;
    targets.push_back(s);
    sele.push_back(w);
    sele.push_back(s);
    clause.push_back(next);
    q.setQuery(targets, sele, clause);
    list<string> result;
    pe.executeQuery(q, result);
    REQUIRE(result.size() == 1);
    REQUIRE(contains(result, "8"));
  }
  SECTION("(_, int)") {
    Query q;
    QueryEntity boolean(QueryEntityType::Boolean, "");
    QueryEntity unders(QueryEntityType::Underscore, "_");
    QueryEntity Int(QueryEntityType::Line, "13");
    Clause next(ClauseType::Next, vector<QueryEntity>{unders, Int});
    vector<QueryEntity> targets;
    vector<QueryEntity> sele;
    vector<Clause> clause;
    targets.push_back(boolean);
    clause.push_back(next);
    q.setQuery(targets, sele, clause);
    list<string> result;
    pe.executeQuery(q, result);
    REQUIRE(result.size() == 1);
    REQUIRE(contains(result, "FALSE"));
  }
}

// SCENARIO("test one Next* clause evaluate") {
//	PKB pkb = buildPKB();
//	PqlEvaluator pe(pkb);
//	SECTION("(int, int)") {
//		//Select Boolean such that NextT("1","10")
//		//True
//		Query q;
//		QueryEntity Int1(QueryEntityType::Line, "1");
//		QueryEntity Int2(QueryEntityType::Line, "10");
//		QueryEntity boolean(QueryEntityType::Boolean, "");
//		Clause next(ClauseType::NextT, vector<QueryEntity>{Int1, Int2});
//		vector<QueryEntity> targets;
//		vector<QueryEntity> sele;
//		vector<Clause> clause;
//		targets.push_back(boolean);
//		clause.push_back(next);
//		q.setQuery(targets, sele, clause);
//		list<string> result; pe.executeQuery(q, result);
//		REQUIRE(result.size() == 1);
//		REQUIRE(contains(result, "TRUE"));
//	}
//	SECTION("(syn, int)") {
//		//If ifs; Select ifs such that NextT(ifs,"10")
//		//4,8
//		Query q;
//		QueryEntity ifs(QueryEntityType::If, "ifs");
//		QueryEntity Int(QueryEntityType::Line, "10");
//		Clause next(ClauseType::NextT, vector<QueryEntity>{ifs, Int});
//		vector<QueryEntity> targets;
//		vector<QueryEntity> sele;
//		vector<Clause> clause;
//		targets.push_back(ifs);
//		sele.push_back(ifs);
//		clause.push_back(next);
//		q.setQuery(targets, sele, clause);
//		list<string> result; pe.executeQuery(q, result);
//		REQUIRE(result.size() == 2);
//		REQUIRE(contains(result, "4"));
//		REQUIRE(contains(result, "8"));
//	}
//	SECTION("(syn, syn)") {
//		//while w; stmt s; Select s such that NextT(w,s)
//		//7,8,9,10
//		Query q;
//		QueryEntity w(QueryEntityType::While, "w");
//		QueryEntity s(QueryEntityType::Stmt, "s");
//		Clause next(ClauseType::NextT, vector<QueryEntity>{w, s});
//		vector<QueryEntity> targets;
//		vector<QueryEntity> sele;
//		vector<Clause> clause;
//		targets.push_back(s);
//		sele.push_back(w);
//		sele.push_back(s);
//		clause.push_back(next);
//		q.setQuery(targets, sele, clause);
//		list<string> result; pe.executeQuery(q, result);
//		REQUIRE(result.size() == 4);
//		REQUIRE(contains(result, "7"));
//		REQUIRE(contains(result, "8"));
//		REQUIRE(contains(result, "9"));
//		REQUIRE(contains(result, "10"));
//	}
//	SECTION("(_, int)") {
//		//Select Boolean such that NextT(_,"13")
//		//FALSE
//		Query q;
//		QueryEntity boolean(QueryEntityType::Boolean, "");
//		QueryEntity unders(QueryEntityType::Underscore, "_");
//		QueryEntity Int(QueryEntityType::Line, "13");
//		Clause next(ClauseType::NextT, vector<QueryEntity>{unders,
// Int}); 		vector<QueryEntity> targets; 		vector<QueryEntity>
// sele; 		vector<Clause>
// clause; 		targets.push_back(boolean);
// clause.push_back(next); 		q.setQuery(targets, sele, clause); 		list<string>
//result; pe.executeQuery(q, result); 		REQUIRE(result.size() == 1);
//		REQUIRE(contains(result, "FALSE"));
//	}
//}

SCENARIO("test one Calls clause evaluate") {
  PKB pkb = buildPKB();
  PqlEvaluator pe(pkb);
  SECTION("(name, name)") {
    Query q;
    QueryEntity n1(QueryEntityType::Name, "main");
    QueryEntity n2(QueryEntityType::Name, "one");
    QueryEntity boolean(QueryEntityType::Boolean, "");
    Clause calls(ClauseType::Calls, vector<QueryEntity>{n1, n2});
    vector<QueryEntity> targets;
    vector<QueryEntity> sele;
    vector<Clause> clause;
    targets.push_back(boolean);
    clause.push_back(calls);
    q.setQuery(targets, sele, clause);
    list<string> result;
    pe.executeQuery(q, result);
    REQUIRE(result.size() == 1);
    REQUIRE(contains(result, "TRUE"));
  }
  SECTION("(syn, name)") {
    Query q;
    QueryEntity p(QueryEntityType::Procedure, "p");
    QueryEntity n(QueryEntityType::Name, "one");
    Clause calls(ClauseType::Calls, vector<QueryEntity>{p, n});
    vector<QueryEntity> targets;
    vector<QueryEntity> sele;
    vector<Clause> clause;
    targets.push_back(p);
    sele.push_back(p);
    clause.push_back(calls);
    q.setQuery(targets, sele, clause);
    list<string> result;
    pe.executeQuery(q, result);
    REQUIRE(result.size() == 1);
    REQUIRE(contains(result, "main"));
  }
  SECTION("(syn, syn)") {
    Query q;
    QueryEntity p1(QueryEntityType::Procedure, "p1");
    QueryEntity p2(QueryEntityType::Procedure, "p2");
    Clause calls(ClauseType::Calls, vector<QueryEntity>{p1, p2});
    vector<QueryEntity> targets;
    vector<QueryEntity> sele;
    vector<Clause> clause;
    targets.push_back(p2);
    sele.push_back(p1);
    sele.push_back(p2);
    clause.push_back(calls);
    q.setQuery(targets, sele, clause);
    list<string> result;
    pe.executeQuery(q, result);
    REQUIRE(result.size() == 3);
    REQUIRE(contains(result, "one"));
    REQUIRE(contains(result, "two"));
    REQUIRE(contains(result, "three"));
  }
  SECTION("(_, name)") {
    Query q;
    QueryEntity boolean(QueryEntityType::Boolean, "");
    QueryEntity unders(QueryEntityType::Underscore, "_");
    QueryEntity n(QueryEntityType::Name, "three");
    Clause calls(ClauseType::Calls, vector<QueryEntity>{unders, n});
    vector<QueryEntity> targets;
    vector<QueryEntity> sele;
    vector<Clause> clause;
    targets.push_back(boolean);
    clause.push_back(calls);
    q.setQuery(targets, sele, clause);
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
    Query q;
    QueryEntity boolean(QueryEntityType::Boolean, "");
    QueryEntity unders(QueryEntityType::Underscore, "_");
    QueryEntity n1(QueryEntityType::Name, "main");
    QueryEntity n2(QueryEntityType::Name, "one");
    Clause calls(ClauseType::Calls, vector<QueryEntity>{n1, n2});
    vector<QueryEntity> targets;
    vector<QueryEntity> sele;
    vector<Clause> clause;
    targets.push_back(boolean);
    clause.push_back(calls);
    q.setQuery(targets, sele, clause);
    list<string> result;
    pe.executeQuery(q, result);
    REQUIRE(result.size() == 1);
    REQUIRE(contains(result, "TRUE"));
  }
  SECTION("(name,synonym)") {
    Query q;
    QueryEntity boolean(QueryEntityType::Boolean, "");
    QueryEntity unders(QueryEntityType::Underscore, "_");
    QueryEntity n1(QueryEntityType::Name, "main");
    QueryEntity n2(QueryEntityType::Procedure, "p");
    Clause calls(ClauseType::Calls, vector<QueryEntity>{n1, n2});
    vector<QueryEntity> targets;
    vector<QueryEntity> sele;
    vector<Clause> clause;
    targets.push_back(n2);
    clause.push_back(calls);
    q.setQuery(targets, sele, clause);
    list<string> result;
    pe.executeQuery(q, result);
    REQUIRE(result.size() == 3);
    REQUIRE(contains(result, "one"));
    REQUIRE(contains(result, "two"));
    REQUIRE(contains(result, "three"));
  }
  SECTION("(name,_)") {
    Query q;
    QueryEntity boolean(QueryEntityType::Boolean, "");
    QueryEntity unders(QueryEntityType::Underscore, "_");
    QueryEntity n1(QueryEntityType::Name, "main");
    QueryEntity n2(QueryEntityType::Underscore, "_");
    Clause calls(ClauseType::Calls, vector<QueryEntity>{n1, n2});
    vector<QueryEntity> targets;
    vector<QueryEntity> sele;
    vector<Clause> clause;
    targets.push_back(boolean);
    clause.push_back(calls);
    q.setQuery(targets, sele, clause);
    list<string> result;
    pe.executeQuery(q, result);
    REQUIRE(result.size() == 1);
    REQUIRE(contains(result, "TRUE"));
  }
  SECTION("(synonym,synonym)") {
    Query q;
    QueryEntity boolean(QueryEntityType::Boolean, "");
    QueryEntity unders(QueryEntityType::Underscore, "_");
    QueryEntity n1(QueryEntityType::Procedure, "p1");
    QueryEntity n2(QueryEntityType::Procedure, "p2");
    Clause calls(ClauseType::Calls, vector<QueryEntity>{n1, n2});
    vector<QueryEntity> targets;
    vector<QueryEntity> sele;
    vector<Clause> clause;
    targets.push_back(n1);
    clause.push_back(calls);
    q.setQuery(targets, sele, clause);
    list<string> result;
    pe.executeQuery(q, result);
    REQUIRE(result.size() == 1);
    REQUIRE(contains(result, "main"));
  }
  SECTION("(synonym,name)") {
    Query q;
    QueryEntity boolean(QueryEntityType::Boolean, "");
    QueryEntity unders(QueryEntityType::Underscore, "_");
    QueryEntity n1(QueryEntityType::Name, "main");
    QueryEntity n2(QueryEntityType::Procedure, "p");
    Clause calls(ClauseType::Calls, vector<QueryEntity>{n2, n1});
    vector<QueryEntity> targets;
    vector<QueryEntity> sele;
    vector<Clause> clause;
    targets.push_back(boolean);
    clause.push_back(calls);
    q.setQuery(targets, sele, clause);
    list<string> result;
    pe.executeQuery(q, result);
    REQUIRE(result.size() == 1);
    REQUIRE(contains(result, "FALSE"));
  }
  SECTION("(_,_)") {
    Query q;
    QueryEntity boolean(QueryEntityType::Boolean, "");
    QueryEntity unders(QueryEntityType::Underscore, "_");
    QueryEntity n1(QueryEntityType::Underscore, "_");
    QueryEntity n2(QueryEntityType::Procedure, "p");
    Clause calls(ClauseType::Calls, vector<QueryEntity>{n1, n1});
    vector<QueryEntity> targets;
    vector<QueryEntity> sele;
    vector<Clause> clause;
    targets.push_back(n2);
    clause.push_back(calls);
    q.setQuery(targets, sele, clause);
    list<string> result;
    pe.executeQuery(q, result);
    REQUIRE(result.size() == 4);
    REQUIRE(contains(result, "main"));
    REQUIRE(contains(result, "one"));
    REQUIRE(contains(result, "two"));
    REQUIRE(contains(result, "three"));
  }
}
