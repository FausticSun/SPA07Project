#include "DesignExtractor.h"
#include "GeneralLexer.h"
#include "SIMPLEParser.h"
#include "catch.hpp"

#include <list>
#include <sstream>

using Lexer::Token;
using Lexer::TokenType;

TEST_CASE("A very simple program") {
  std::string program = R"(
  procedure A {
    x = 1;
    y = 2;
  }
  )";
  std::stringstream ss;
  ss << program;
  std::list<Token> tokens = Lexer::tokenize(ss);
  auto pkb = Parser::parseSIMPLE(tokens);
  DesignExtractor::populateDesigns(pkb);

  auto procTable = pkb->getProcTable();
  auto varTable = pkb->getVarTable();
  auto constTable = pkb->getConstTable();
  auto followsTable = pkb->getFollows();
  auto followsTTable = pkb->getFollowsT();
  auto modifiesPTable = pkb->getModifiesP();
  auto modifiesSTable = pkb->getModifiesS();
  auto nextTable = pkb->getNext();

  REQUIRE(procTable.contains({"A"}));
  REQUIRE(varTable.contains({"x"}));
  REQUIRE(varTable.contains({"y"}));
  REQUIRE(constTable.contains({"1"}));
  REQUIRE(constTable.contains({"2"}));
  REQUIRE(followsTable.contains({"1", "2"}));
  REQUIRE(followsTTable.contains({"1", "2"}));
  REQUIRE(modifiesPTable.contains({"A", "x"}));
  REQUIRE(modifiesPTable.contains({"A", "y"}));
  REQUIRE(modifiesSTable.contains({"1", "x"}));
  REQUIRE(modifiesSTable.contains({"2", "y"}));
  REQUIRE(nextTable.contains({"1", "2"}));
}

TEST_CASE("Program with one while stmt") {
  std::string program = R"(
  procedure A {
    while ((x == 0) && (y <= 22)) {
      x = y % z + 55 * e - k;
    }
  }
  )";
  std::stringstream ss;
  ss << program;
  std::list<Token> tokens = Lexer::tokenize(ss);
  auto pkb = Parser::parseSIMPLE(tokens);
  DesignExtractor::populateDesigns(pkb);

  auto procTable = pkb->getProcTable();
  auto varTable = pkb->getVarTable();
  auto constTable = pkb->getConstTable();
  auto parentTable = pkb->getParent();
  auto parentTTable = pkb->getParentT();
  auto modifiesPTable = pkb->getModifiesP();
  auto modifiesSTable = pkb->getModifiesS();
  auto usesPTable = pkb->getUsesP();
  auto usesSTable = pkb->getUsesS();
  auto nextTable = pkb->getNext();

  REQUIRE(procTable.contains({"A"}));
  REQUIRE(varTable.contains({"x"}));
  REQUIRE(varTable.contains({"y"}));
  REQUIRE(varTable.contains({"z"}));
  REQUIRE(varTable.contains({"e"}));
  REQUIRE(varTable.contains({"k"}));
  REQUIRE(constTable.contains({"0"}));
  REQUIRE(constTable.contains({"22"}));
  REQUIRE(constTable.contains({"55"}));
  REQUIRE(parentTable.contains({"1", "2"}));
  REQUIRE(parentTTable.contains({"1", "2"}));
  REQUIRE(modifiesPTable.contains({"A", "x"}));
  REQUIRE(modifiesSTable.contains({"2", "x"}));
  REQUIRE(usesPTable.contains({"A", "x"}));
  REQUIRE(usesPTable.contains({"A", "y"}));
  REQUIRE(usesPTable.contains({"A", "z"}));
  REQUIRE(usesPTable.contains({"A", "e"}));
  REQUIRE(usesPTable.contains({"A", "k"}));
  REQUIRE(usesSTable.contains({"1", "x"}));
  REQUIRE(usesSTable.contains({"1", "y"}));
  REQUIRE(usesSTable.contains({"2", "y"}));
  REQUIRE(usesSTable.contains({"2", "z"}));
  REQUIRE(usesSTable.contains({"2", "e"}));
  REQUIRE(usesSTable.contains({"2", "k"}));
  REQUIRE(nextTable.contains({"1", "2"}));
}

TEST_CASE("Program with one if stmt") {
  std::string program = R"(
  procedure A {
    if (k + 5 / z < (50 - 3)) then {
      read y;
    } else {
      print q;
    }
  }
  )";
  std::stringstream ss;
  ss << program;
  std::list<Token> tokens = Lexer::tokenize(ss);
  auto pkb = Parser::parseSIMPLE(tokens);
  DesignExtractor::populateDesigns(pkb);

  auto procTable = pkb->getProcTable();
  auto varTable = pkb->getVarTable();
  auto constTable = pkb->getConstTable();
  auto parentTable = pkb->getParent();
  auto parentTTable = pkb->getParentT();
  auto modifiesPTable = pkb->getModifiesP();
  auto modifiesSTable = pkb->getModifiesS();
  auto usesPTable = pkb->getUsesP();
  auto usesSTable = pkb->getUsesS();
  auto nextTable = pkb->getNext();

  REQUIRE(procTable.contains({"A"}));
  REQUIRE(varTable.contains({"y"}));
  REQUIRE(varTable.contains({"z"}));
  REQUIRE(varTable.contains({"q"}));
  REQUIRE(varTable.contains({"k"}));
  REQUIRE(constTable.contains({"3"}));
  REQUIRE(constTable.contains({"5"}));
  REQUIRE(constTable.contains({"50"}));
  REQUIRE(parentTable.contains({"1", "2"}));
  REQUIRE(parentTable.contains({"1", "3"}));
  REQUIRE(parentTTable.contains({"1", "2"}));
  REQUIRE(parentTTable.contains({"1", "3"}));
  REQUIRE(modifiesPTable.contains({"A", "y"}));
  REQUIRE(modifiesSTable.contains({"2", "y"}));
  REQUIRE(usesPTable.contains({"A", "q"}));
  REQUIRE(usesSTable.contains({"3", "q"}));
  REQUIRE(nextTable.contains({"1", "2"}));
  REQUIRE(nextTable.contains({"1", "3"}));
}

TEST_CASE("Program with while nested in while") {
  std::string program = R"(
  procedure A {
    while (x > 0) {
      while (y != x) {
        read x;
      }
    }
  }
  )";
  std::stringstream ss;
  ss << program;
  std::list<Token> tokens = Lexer::tokenize(ss);
  auto pkb = Parser::parseSIMPLE(tokens);
  DesignExtractor::populateDesigns(pkb);

  auto procTable = pkb->getProcTable();
  auto varTable = pkb->getVarTable();
  auto constTable = pkb->getConstTable();
  auto parentTable = pkb->getParent();
  auto parentTTable = pkb->getParentT();
  auto modifiesPTable = pkb->getModifiesP();
  auto modifiesSTable = pkb->getModifiesS();
  auto usesPTable = pkb->getUsesP();
  auto usesSTable = pkb->getUsesS();
  auto nextTable = pkb->getNext();

  REQUIRE(procTable.contains({"A"}));
  REQUIRE(varTable.contains({"x"}));
  REQUIRE(varTable.contains({"y"}));
  REQUIRE(constTable.contains({"0"}));
  REQUIRE(parentTable.contains({"1", "2"}));
  REQUIRE(parentTable.contains({"2", "3"}));
  REQUIRE(parentTTable.contains({"1", "2"}));
  REQUIRE(parentTTable.contains({"2", "3"}));
  REQUIRE(parentTTable.contains({"1", "3"}));
  REQUIRE(modifiesPTable.contains({"A", "x"}));
  REQUIRE(modifiesSTable.contains({"3", "x"}));
  REQUIRE(usesPTable.contains({"A", "x"}));
  REQUIRE(usesPTable.contains({"A", "y"}));
  REQUIRE(usesSTable.contains({"1", "x"}));
  REQUIRE(usesSTable.contains({"2", "y"}));
  REQUIRE(usesSTable.contains({"2", "x"}));
  REQUIRE(nextTable.contains({"1", "2"}));
  REQUIRE(nextTable.contains({"2", "3"}));
  REQUIRE(nextTable.contains({"3", "2"}));
  REQUIRE(nextTable.contains({"2", "1"}));
}

TEST_CASE("Program with if nested in if") {
  std::string program = R"(
  procedure A {
    if (x == 0) then {
      if (y == 0) then {
        x = 2 * k;
      } else {
        read p;
      }
    } else {
      print q;
    }
  }
  )";
  std::stringstream ss;
  ss << program;
  std::list<Token> tokens = Lexer::tokenize(ss);
  auto pkb = Parser::parseSIMPLE(tokens);
  DesignExtractor::populateDesigns(pkb);

  auto procTable = pkb->getProcTable();
  auto varTable = pkb->getVarTable();
  auto constTable = pkb->getConstTable();
  auto parentTable = pkb->getParent();
  auto parentTTable = pkb->getParentT();
  auto modifiesPTable = pkb->getModifiesP();
  auto modifiesSTable = pkb->getModifiesS();
  auto usesPTable = pkb->getUsesP();
  auto usesSTable = pkb->getUsesS();
  auto nextTable = pkb->getNext();

  REQUIRE(procTable.contains({"A"}));
  REQUIRE(varTable.contains({"x"}));
  REQUIRE(varTable.contains({"y"}));
  REQUIRE(varTable.contains({"k"}));
  REQUIRE(varTable.contains({"p"}));
  REQUIRE(varTable.contains({"q"}));
  REQUIRE(constTable.contains({"0"}));
  REQUIRE(constTable.contains({"2"}));
  REQUIRE(parentTable.contains({"1", "2"}));
  REQUIRE(parentTable.contains({"2", "3"}));
  REQUIRE(parentTable.contains({"2", "4"}));
  REQUIRE(parentTable.contains({"1", "5"}));
  REQUIRE(parentTTable.contains({"1", "2"}));
  REQUIRE(parentTTable.contains({"2", "3"}));
  REQUIRE(parentTTable.contains({"1", "3"}));
  REQUIRE(parentTTable.contains({"2", "4"}));
  REQUIRE(parentTTable.contains({"1", "4"}));
  REQUIRE(parentTTable.contains({"1", "5"}));
  REQUIRE(modifiesPTable.contains({"A", "x"}));
  REQUIRE(modifiesPTable.contains({"A", "p"}));
  REQUIRE(modifiesSTable.contains({"3", "x"}));
  REQUIRE(modifiesSTable.contains({"4", "p"}));
  REQUIRE(usesPTable.contains({"A", "x"}));
  REQUIRE(usesPTable.contains({"A", "y"}));
  REQUIRE(usesPTable.contains({"A", "q"}));
  REQUIRE(usesSTable.contains({"1", "x"}));
  REQUIRE(usesSTable.contains({"2", "y"}));
  REQUIRE(usesSTable.contains({"5", "q"}));
  REQUIRE(nextTable.contains({"1", "2"}));
  REQUIRE(nextTable.contains({"2", "3"}));
  REQUIRE(nextTable.contains({"2", "4"}));
  REQUIRE(nextTable.contains({"1", "5"}));
}

TEST_CASE("Program with while nested in if") {
  std::string program = R"(
  procedure A {
    if (x != 0) then {
      while (m == k) {
        print x;
      }
    } else {
       print y;
    }
  }
  )";
  std::stringstream ss;
  ss << program;
  std::list<Token> tokens = Lexer::tokenize(ss);
  auto pkb = Parser::parseSIMPLE(tokens);
  DesignExtractor::populateDesigns(pkb);

  auto procTable = pkb->getProcTable();
  auto varTable = pkb->getVarTable();
  auto constTable = pkb->getConstTable();
  auto parentTable = pkb->getParent();
  auto parentTTable = pkb->getParentT();
  auto usesPTable = pkb->getUsesP();
  auto usesSTable = pkb->getUsesS();
  auto nextTable = pkb->getNext();

  REQUIRE(procTable.contains({"A"}));
  REQUIRE(varTable.contains({"x"}));
  REQUIRE(varTable.contains({"y"}));
  REQUIRE(varTable.contains({"k"}));
  REQUIRE(varTable.contains({"m"}));
  REQUIRE(constTable.contains({"0"}));
  REQUIRE(parentTable.contains({"1", "2"}));
  REQUIRE(parentTable.contains({"2", "3"}));
  REQUIRE(parentTable.contains({"1", "4"}));
  REQUIRE(parentTTable.contains({"1", "2"}));
  REQUIRE(parentTTable.contains({"2", "3"}));
  REQUIRE(parentTTable.contains({"1", "3"}));
  REQUIRE(parentTTable.contains({"1", "4"}));
  REQUIRE(usesPTable.contains({"A", "x"}));
  REQUIRE(usesPTable.contains({"A", "y"}));
  REQUIRE(usesPTable.contains({"A", "m"}));
  REQUIRE(usesPTable.contains({"A", "k"}));
  REQUIRE(usesSTable.contains({"1", "x"}));
  REQUIRE(usesSTable.contains({"2", "m"}));
  REQUIRE(usesSTable.contains({"2", "k"}));
  REQUIRE(usesSTable.contains({"3", "x"}));
  REQUIRE(usesSTable.contains({"4", "y"}));
  REQUIRE(nextTable.contains({"1", "2"}));
  REQUIRE(nextTable.contains({"2", "3"}));
  REQUIRE(nextTable.contains({"3", "2"}));
  REQUIRE(nextTable.contains({"1", "4"}));
}

TEST_CASE("Program with if nested in while") {
  std::string program = R"(
  procedure A {
    while (x > 0) {
      if (y == k) then {
        read m;
      } else {
        read v;
      }
    }
  }
  )";
  std::stringstream ss;
  ss << program;
  std::list<Token> tokens = Lexer::tokenize(ss);
  auto pkb = Parser::parseSIMPLE(tokens);
  DesignExtractor::populateDesigns(pkb);

  auto procTable = pkb->getProcTable();
  auto varTable = pkb->getVarTable();
  auto constTable = pkb->getConstTable();
  auto parentTable = pkb->getParent();
  auto parentTTable = pkb->getParentT();
  auto modifiesPTable = pkb->getModifiesP();
  auto modifiesSTable = pkb->getModifiesS();
  auto usesPTable = pkb->getUsesP();
  auto usesSTable = pkb->getUsesS();
  auto nextTable = pkb->getNext();

  REQUIRE(procTable.contains({"A"}));
  REQUIRE(varTable.contains({"x"}));
  REQUIRE(varTable.contains({"y"}));
  REQUIRE(varTable.contains({"k"}));
  REQUIRE(varTable.contains({"m"}));
  REQUIRE(varTable.contains({"v"}));
  REQUIRE(constTable.contains({"0"}));
  REQUIRE(parentTable.contains({"1", "2"}));
  REQUIRE(parentTable.contains({"2", "3"}));
  REQUIRE(parentTable.contains({"2", "4"}));
  REQUIRE(parentTTable.contains({"1", "2"}));
  REQUIRE(parentTTable.contains({"2", "3"}));
  REQUIRE(parentTTable.contains({"1", "3"}));
  REQUIRE(parentTTable.contains({"2", "4"}));
  REQUIRE(parentTTable.contains({"1", "4"}));
  REQUIRE(modifiesPTable.contains({"A", "m"}));
  REQUIRE(modifiesPTable.contains({"A", "v"}));
  REQUIRE(modifiesSTable.contains({"3", "m"}));
  REQUIRE(modifiesSTable.contains({"4", "v"}));
  REQUIRE(usesPTable.contains({"A", "x"}));
  REQUIRE(usesPTable.contains({"A", "y"}));
  REQUIRE(usesPTable.contains({"A", "k"}));
  REQUIRE(usesSTable.contains({"1", "x"}));
  REQUIRE(usesSTable.contains({"2", "y"}));
  REQUIRE(usesSTable.contains({"2", "k"}));
  REQUIRE(nextTable.contains({"1", "2"}));
  REQUIRE(nextTable.contains({"2", "3"}));
  REQUIRE(nextTable.contains({"2", "4"}));
  REQUIRE(nextTable.contains({"3", "1"}));
  REQUIRE(nextTable.contains({"4", "1"}));
}

TEST_CASE("Program with multiple procedures with call stmts") {
  std::string program = R"(
  procedure A {
    call B;
    call C;
  }

  procedure B {
    call D;
    print x;
    y = 124;
  }

  procedure C {
    call B;
    while (x == 12 / 2) {
      read v;
    }
  }
  
  procedure D {
    if ((v == s) && (x == 2)) then {
      f = 12409 * sao;
    } else {
      while (k == 0) {
        while (f == 1) {
          print z;
        }
      }
    }
    o = 0;
  }
  )";
  std::stringstream ss;
  ss << program;
  std::list<Token> tokens = Lexer::tokenize(ss);
  auto pkb = Parser::parseSIMPLE(tokens);
  DesignExtractor::populateDesigns(pkb);

  auto procTable = pkb->getProcTable();
  auto varTable = pkb->getVarTable();
  auto constTable = pkb->getConstTable();
  auto followsTable = pkb->getFollows();
  auto followsTTable = pkb->getFollowsT();
  auto parentTable = pkb->getParent();
  auto parentTTable = pkb->getParentT();
  auto modifiesPTable = pkb->getModifiesP();
  auto modifiesSTable = pkb->getModifiesS();
  auto usesPTable = pkb->getUsesP();
  auto usesSTable = pkb->getUsesS();
  auto callsTable = pkb->getCalls();
  auto callsTTable = pkb->getCallsT();
  auto nextTable = pkb->getNext();

  REQUIRE(procTable.contains({"A"}));
  REQUIRE(procTable.contains({"B"}));
  REQUIRE(procTable.contains({"C"}));
  REQUIRE(procTable.contains({"D"}));
  REQUIRE(varTable.contains({"x"}));
  REQUIRE(varTable.contains({"y"}));
  REQUIRE(varTable.contains({"v"}));
  REQUIRE(varTable.contains({"s"}));
  REQUIRE(varTable.contains({"f"}));
  REQUIRE(varTable.contains({"k"}));
  REQUIRE(varTable.contains({"z"}));
  REQUIRE(varTable.contains({"o"}));
  REQUIRE(constTable.contains({"124"}));
  REQUIRE(constTable.contains({"12"}));
  REQUIRE(constTable.contains({"2"}));
  REQUIRE(constTable.contains({"12409"}));
  REQUIRE(constTable.contains({"0"}));
  REQUIRE(constTable.contains({"1"}));

  REQUIRE(followsTable.contains({"1", "2"}));
  REQUIRE(followsTable.contains({"3", "4"}));
  REQUIRE(followsTable.contains({"4", "5"}));
  REQUIRE(followsTable.contains({"6", "7"}));
  REQUIRE(followsTable.contains({"9", "14"}));
  REQUIRE(followsTTable.contains({"1", "2"}));
  REQUIRE(followsTTable.contains({"3", "4"}));
  REQUIRE(followsTTable.contains({"4", "5"}));
  REQUIRE(followsTTable.contains({"3", "5"}));
  REQUIRE(followsTTable.contains({"6", "7"}));
  REQUIRE(followsTTable.contains({"9", "14"}));

  REQUIRE(parentTable.contains({"7", "8"}));
  REQUIRE(parentTable.contains({"9", "10"}));
  REQUIRE(parentTable.contains({"9", "11"}));
  REQUIRE(parentTable.contains({"11", "12"}));
  REQUIRE(parentTable.contains({"12", "13"}));
  REQUIRE(parentTTable.contains({"7", "8"}));
  REQUIRE(parentTTable.contains({"9", "10"}));
  REQUIRE(parentTTable.contains({"9", "11"}));
  REQUIRE(parentTTable.contains({"11", "12"}));
  REQUIRE(parentTTable.contains({"12", "13"}));
  REQUIRE(parentTTable.contains({"11", "13"}));
  REQUIRE(parentTTable.contains({"9", "12"}));
  REQUIRE(parentTTable.contains({"9", "13"}));

  REQUIRE(modifiesPTable.contains({"D", "f"}));
  REQUIRE(modifiesPTable.contains({"D", "o"}));
  REQUIRE(modifiesPTable.contains({"B", "f"}));
  REQUIRE(modifiesPTable.contains({"B", "o"}));
  REQUIRE(modifiesPTable.contains({"B", "y"}));
  REQUIRE(modifiesPTable.contains({"C", "f"}));
  REQUIRE(modifiesPTable.contains({"C", "o"}));
  REQUIRE(modifiesPTable.contains({"C", "y"}));
  REQUIRE(modifiesPTable.contains({"C", "v"}));
  REQUIRE(modifiesPTable.contains({"A", "f"}));
  REQUIRE(modifiesPTable.contains({"A", "o"}));
  REQUIRE(modifiesPTable.contains({"A", "y"}));
  REQUIRE(modifiesPTable.contains({"A", "v"}));

  REQUIRE(modifiesSTable.contains({"10", "f"}));
  REQUIRE(modifiesSTable.contains({"14", "o"}));
  REQUIRE(modifiesSTable.contains({"3", "f"}));
  REQUIRE(modifiesSTable.contains({"3", "o"}));
  REQUIRE(modifiesSTable.contains({"5", "y"}));
  REQUIRE(modifiesSTable.contains({"6", "f"}));
  REQUIRE(modifiesSTable.contains({"6", "o"}));
  REQUIRE(modifiesSTable.contains({"6", "y"}));
  REQUIRE(modifiesSTable.contains({"8", "v"}));
  REQUIRE(modifiesSTable.contains({"1", "f"}));
  REQUIRE(modifiesSTable.contains({"1", "o"}));
  REQUIRE(modifiesSTable.contains({"1", "y"}));
  REQUIRE(modifiesSTable.contains({"2", "f"}));
  REQUIRE(modifiesSTable.contains({"2", "o"}));
  REQUIRE(modifiesSTable.contains({"2", "y"}));
  REQUIRE(modifiesSTable.contains({"2", "v"}));

  REQUIRE(usesPTable.contains({"D", "v"}));
  REQUIRE(usesPTable.contains({"D", "s"}));
  REQUIRE(usesPTable.contains({"D", "x"}));
  REQUIRE(usesPTable.contains({"D", "sao"}));
  REQUIRE(usesPTable.contains({"D", "k"}));
  REQUIRE(usesPTable.contains({"D", "f"}));
  REQUIRE(usesPTable.contains({"D", "z"}));
  REQUIRE(usesPTable.contains({"B", "v"}));
  REQUIRE(usesPTable.contains({"B", "s"}));
  REQUIRE(usesPTable.contains({"B", "x"}));
  REQUIRE(usesPTable.contains({"B", "sao"}));
  REQUIRE(usesPTable.contains({"B", "k"}));
  REQUIRE(usesPTable.contains({"B", "f"}));
  REQUIRE(usesPTable.contains({"B", "z"}));
  REQUIRE(usesPTable.contains({"C", "v"}));
  REQUIRE(usesPTable.contains({"C", "s"}));
  REQUIRE(usesPTable.contains({"C", "x"}));
  REQUIRE(usesPTable.contains({"C", "sao"}));
  REQUIRE(usesPTable.contains({"C", "k"}));
  REQUIRE(usesPTable.contains({"C", "f"}));
  REQUIRE(usesPTable.contains({"C", "z"}));
  REQUIRE(usesPTable.contains({"A", "v"}));
  REQUIRE(usesPTable.contains({"A", "s"}));
  REQUIRE(usesPTable.contains({"A", "x"}));
  REQUIRE(usesPTable.contains({"A", "sao"}));
  REQUIRE(usesPTable.contains({"A", "k"}));
  REQUIRE(usesPTable.contains({"A", "f"}));
  REQUIRE(usesPTable.contains({"A", "z"}));

  REQUIRE(usesSTable.contains({"9", "v"}));
  REQUIRE(usesSTable.contains({"9", "s"}));
  REQUIRE(usesSTable.contains({"9", "x"}));
  REQUIRE(usesSTable.contains({"10", "sao"}));
  REQUIRE(usesSTable.contains({"11", "k"}));
  REQUIRE(usesSTable.contains({"12", "f"}));
  REQUIRE(usesSTable.contains({"13", "z"}));
  REQUIRE(usesSTable.contains({"3", "v"}));
  REQUIRE(usesSTable.contains({"3", "s"}));
  REQUIRE(usesSTable.contains({"3", "x"}));
  REQUIRE(usesSTable.contains({"3", "sao"}));
  REQUIRE(usesSTable.contains({"3", "k"}));
  REQUIRE(usesSTable.contains({"3", "f"}));
  REQUIRE(usesSTable.contains({"3", "z"}));
  REQUIRE(usesSTable.contains({"4", "x"}));
  REQUIRE(usesSTable.contains({"6", "v"}));
  REQUIRE(usesSTable.contains({"6", "s"}));
  REQUIRE(usesSTable.contains({"6", "x"}));
  REQUIRE(usesSTable.contains({"6", "sao"}));
  REQUIRE(usesSTable.contains({"6", "k"}));
  REQUIRE(usesSTable.contains({"6", "f"}));
  REQUIRE(usesSTable.contains({"6", "z"}));
  REQUIRE(usesSTable.contains({"6", "x"}));
  REQUIRE(usesSTable.contains({"7", "x"}));
  REQUIRE(usesSTable.contains({"1", "v"}));
  REQUIRE(usesSTable.contains({"1", "s"}));
  REQUIRE(usesSTable.contains({"1", "x"}));
  REQUIRE(usesSTable.contains({"1", "sao"}));
  REQUIRE(usesSTable.contains({"1", "k"}));
  REQUIRE(usesSTable.contains({"1", "f"}));
  REQUIRE(usesSTable.contains({"1", "z"}));
  REQUIRE(usesSTable.contains({"1", "x"}));
  REQUIRE(usesSTable.contains({"2", "v"}));
  REQUIRE(usesSTable.contains({"2", "s"}));
  REQUIRE(usesSTable.contains({"2", "x"}));
  REQUIRE(usesSTable.contains({"2", "sao"}));
  REQUIRE(usesSTable.contains({"2", "k"}));
  REQUIRE(usesSTable.contains({"2", "f"}));
  REQUIRE(usesSTable.contains({"2", "z"}));
  REQUIRE(usesSTable.contains({"2", "x"}));

  REQUIRE(callsTable.contains({"A", "B"}));
  REQUIRE(callsTable.contains({"A", "C"}));
  REQUIRE(callsTable.contains({"B", "D"}));
  REQUIRE(callsTable.contains({"C", "B"}));

  REQUIRE(callsTTable.contains({"A", "B"}));
  REQUIRE(callsTTable.contains({"A", "C"}));
  REQUIRE(callsTTable.contains({"B", "D"}));
  REQUIRE(callsTTable.contains({"C", "B"}));
  REQUIRE(callsTTable.contains({"A", "D"}));
  REQUIRE(callsTTable.contains({"C", "D"}));

  REQUIRE(nextTable.contains({"1", "2"}));
  REQUIRE(nextTable.contains({"3", "4"}));
  REQUIRE(nextTable.contains({"4", "5"}));
  REQUIRE(nextTable.contains({"6", "7"}));
  REQUIRE(nextTable.contains({"7", "8"}));
  REQUIRE(nextTable.contains({"8", "7"}));
  REQUIRE(nextTable.contains({"9", "10"}));
  REQUIRE(nextTable.contains({"9", "11"}));
  REQUIRE(nextTable.contains({"11", "12"}));
  REQUIRE(nextTable.contains({"12", "13"}));
  REQUIRE(nextTable.contains({"13", "12"}));
  REQUIRE(nextTable.contains({"12", "11"}));
  REQUIRE(nextTable.contains({"11", "14"}));
  REQUIRE(nextTable.contains({"10", "14"}));
}
