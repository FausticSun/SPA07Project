#include "GeneralLexer.h"
#include "SIMPLEParser.h"
#include "catch.hpp"
#include <sstream>

TEST_CASE("Empty program") {
  std::string program = R"()";
  std::stringstream ss;
  ss << program;
  std::list<Token> tokens = Lexer::tokenize(ss);
  REQUIRE_THROWS(Parser::parseSIMPLE(tokens));
}

TEST_CASE("Single empty procedure") {
  std::string program = R"(
  procedure main {
  }
  )";
  std::stringstream ss;
  ss << program;
  std::list<Token> tokens = Lexer::tokenize(ss);
  REQUIRE_THROWS(Parser::parseSIMPLE(tokens));
}

TEST_CASE("Single non-empty procedure") {
  std::string program = R"(
  procedure main {
    read a;
  }
  )";
  std::stringstream ss;
  ss << program;
  std::list<Token> tokens = Lexer::tokenize(ss);
  auto pkb = Parser::parseSIMPLE(tokens);
  // Procedures
  auto procTable = pkb->getProcTable();
  REQUIRE(procTable.size() == 1);
  REQUIRE(procTable.contains({"main"}));
}

TEST_CASE("Multiple non-empty procedure") {
  std::string program = R"(
  procedure proc1 {
    read a;
  }
  procedure proc2 {
    read b;
  }
  procedure proc3 {
    read b;
  }
  )";
  std::stringstream ss;
  ss << program;
  std::list<Token> tokens = Lexer::tokenize(ss);
  auto pkb = Parser::parseSIMPLE(tokens);
  // Procedures
  auto procTable = pkb->getProcTable();
  REQUIRE(procTable.size() == 3);
  REQUIRE(procTable.contains({"proc1"}));
  REQUIRE(procTable.contains({"proc2"}));
  REQUIRE(procTable.contains({"proc3"}));
}

TEST_CASE("Statement extraction") {
  std::string program = R"(
  procedure proc1 {
    read a;
    print b;
    c = d;
    if (e > f) then {
      read g;  
    } else {
      read h;
    }
    while (i < j) {
      read k;
      call proc2;
    }
  }
  procedure proc2 {
    read l;
  }
  )";
  std::stringstream ss;
  ss << program;
  std::list<Token> tokens = Lexer::tokenize(ss);
  auto pkb = Parser::parseSIMPLE(tokens);
  // Statement
  auto readStmtTable = pkb->getStmtType(StatementType::Read);
  REQUIRE(readStmtTable.size() == 5);
  REQUIRE(readStmtTable.contains({"1"}));
  REQUIRE(readStmtTable.contains({"5"}));
  REQUIRE(readStmtTable.contains({"6"}));
  REQUIRE(readStmtTable.contains({"8"}));
  REQUIRE(readStmtTable.contains({"10"}));
  auto printStmtTable = pkb->getStmtType(StatementType::Print);
  REQUIRE(printStmtTable.size() == 1);
  REQUIRE(printStmtTable.contains({"2"}));
  auto assignStmtTable = pkb->getStmtType(StatementType::Assign);
  REQUIRE(assignStmtTable.size() == 1);
  REQUIRE(assignStmtTable.contains({"3"}));
  auto ifStmtTable = pkb->getStmtType(StatementType::If);
  REQUIRE(ifStmtTable.size() == 1);
  REQUIRE(ifStmtTable.contains({"4"}));
  auto whileStmtTable = pkb->getStmtType(StatementType::While);
  REQUIRE(whileStmtTable.size() == 1);
  REQUIRE(whileStmtTable.contains({"7"}));
  auto callStmtTable = pkb->getStmtType(StatementType::Call);
  REQUIRE(callStmtTable.size() == 1);
  REQUIRE(callStmtTable.contains({"9"}));
}

TEST_CASE("Variables extraction") {
  std::string program = R"(
  procedure proc1 {
    read a;
    print b;
    c = d;
    if (e > f) then {
      read g;  
    } else {
      read h;
    }
    while (i < j) {
      read k;
    }
  }
  procedure proc2 {
    read l;
  }
  )";
  std::stringstream ss;
  ss << program;
  std::list<Token> tokens = Lexer::tokenize(ss);
  auto pkb = Parser::parseSIMPLE(tokens);
  // Variables
  auto varTable = pkb->getVarTable();
  REQUIRE(varTable.size() == 12);
  REQUIRE(varTable.contains({"a"}));
  REQUIRE(varTable.contains({"b"}));
  REQUIRE(varTable.contains({"c"}));
  REQUIRE(varTable.contains({"d"}));
  REQUIRE(varTable.contains({"e"}));
  REQUIRE(varTable.contains({"f"}));
  REQUIRE(varTable.contains({"g"}));
  REQUIRE(varTable.contains({"h"}));
  REQUIRE(varTable.contains({"i"}));
  REQUIRE(varTable.contains({"j"}));
  REQUIRE(varTable.contains({"k"}));
  REQUIRE(varTable.contains({"l"}));
}

TEST_CASE("Read statement") {
  std::string program = R"(
  procedure main {
    read a;
  }
  )";
  std::stringstream ss;
  ss << program;
  std::list<Token> tokens = Lexer::tokenize(ss);
  auto pkb = Parser::parseSIMPLE(tokens);
  // Modifies(s, v)
  auto modifiesSTable = pkb->getModifiesS();
  REQUIRE(modifiesSTable.size() == 1);
  REQUIRE(modifiesSTable.contains({"1", "a"}));
  // Modifies(p, v)
  auto modifiesPTable = pkb->getModifiesP();
  REQUIRE(modifiesPTable.size() == 1);
  REQUIRE(modifiesPTable.contains({"main", "a"}));
}

TEST_CASE("Print statement") {
  std::string program = R"(
  procedure main {
    print a;
  }
  )";
  std::stringstream ss;
  ss << program;
  std::list<Token> tokens = Lexer::tokenize(ss);
  auto pkb = Parser::parseSIMPLE(tokens);
  // Uses(s, v)
  auto usesSTable = pkb->getUsesS();
  REQUIRE(usesSTable.size() == 1);
  REQUIRE(usesSTable.contains({"1", "a"}));
  // Uses(p, v)
  auto usesPTable = pkb->getUsesP();
  REQUIRE(usesPTable.size() == 1);
  REQUIRE(usesPTable.contains({"main", "a"}));
}

TEST_CASE("Assign statement") {
  std::string program = R"(
  procedure main {
    a = b + c;
  }
  )";
  std::stringstream ss;
  ss << program;
  std::list<Token> tokens = Lexer::tokenize(ss);
  auto pkb = Parser::parseSIMPLE(tokens);
  // Uses(s, v)
  auto usesSTable = pkb->getUsesS();
  REQUIRE(usesSTable.size() == 2);
  REQUIRE(usesSTable.contains({"1", "b"}));
  REQUIRE(usesSTable.contains({"1", "c"}));
  // Uses(p, v)
  auto usesPTable = pkb->getUsesP();
  REQUIRE(usesPTable.size() == 2);
  REQUIRE(usesPTable.contains({"main", "b"}));
  REQUIRE(usesPTable.contains({"main", "c"}));
  // Modifies(s, v)
  auto modifiesSTable = pkb->getModifiesS();
  REQUIRE(modifiesSTable.size() == 1);
  REQUIRE(modifiesSTable.contains({"1", "a"}));
  // Modifies(p, v)
  auto modifiesPTable = pkb->getModifiesP();
  REQUIRE(modifiesPTable.size() == 1);
  REQUIRE(modifiesPTable.contains({"main", "a"}));
  // Assign pattern
  auto assignTable = pkb->getAssignMatches("b c + ", true);
  REQUIRE(assignTable.size() == 1);
  REQUIRE(assignTable.contains({"1", "a"}));
}

TEST_CASE("While statement") {
  std::string program = R"(
  procedure main {
    while (a > b) {
      print c;
    }
  }
  )";
  std::stringstream ss;
  ss << program;
  std::list<Token> tokens = Lexer::tokenize(ss);
  auto pkb = Parser::parseSIMPLE(tokens);
  // Uses(s, v)
  auto usesSTable = pkb->getUsesS();
  REQUIRE(usesSTable.size() == 3);
  REQUIRE(usesSTable.contains({"1", "a"}));
  REQUIRE(usesSTable.contains({"1", "b"}));
  REQUIRE(usesSTable.contains({"2", "c"}));
  // While pattern
  auto whileCondTable = pkb->getWhileMatches();
  REQUIRE(whileCondTable.size() == 2);
  REQUIRE(whileCondTable.contains({"1", "a"}));
  REQUIRE(whileCondTable.contains({"1", "b"}));
}

TEST_CASE("If statement") {
  std::string program = R"(
  procedure main {
    if (a > b) then {
      print c;
    } else {
      print d;
    }
  }
  )";
  std::stringstream ss;
  ss << program;
  std::list<Token> tokens = Lexer::tokenize(ss);
  auto pkb = Parser::parseSIMPLE(tokens);
  // Uses(s, v)
  auto usesSTable = pkb->getUsesS();
  REQUIRE(usesSTable.size() == 4);
  REQUIRE(usesSTable.contains({"1", "a"}));
  REQUIRE(usesSTable.contains({"1", "b"}));
  REQUIRE(usesSTable.contains({"2", "c"}));
  REQUIRE(usesSTable.contains({"3", "d"}));
  // If pattern
  auto ifCondTable = pkb->getIfMatches();
  REQUIRE(ifCondTable.size() == 2);
  REQUIRE(ifCondTable.contains({"1", "a"}));
  REQUIRE(ifCondTable.contains({"1", "b"}));
}

TEST_CASE("Assign operators") {
  std::string program = R"(
  procedure main {
    a = b * c + d / e - f % g;
  }
  )";
  std::stringstream ss;
  ss << program;
  std::list<Token> tokens = Lexer::tokenize(ss);
  auto pkb = Parser::parseSIMPLE(tokens);
  // Uses(s, v)
  auto usesSTable = pkb->getUsesS();
  REQUIRE(usesSTable.size() == 6);
  REQUIRE(usesSTable.contains({"1", "b"}));
  REQUIRE(usesSTable.contains({"1", "c"}));
  REQUIRE(usesSTable.contains({"1", "d"}));
  REQUIRE(usesSTable.contains({"1", "e"}));
  REQUIRE(usesSTable.contains({"1", "f"}));
  REQUIRE(usesSTable.contains({"1", "g"}));
  // Modifies(s, v)
  auto modifiesSTable = pkb->getModifiesS();
  REQUIRE(modifiesSTable.size() == 1);
  REQUIRE(modifiesSTable.contains({"1", "a"}));
  // Assign pattern
  auto assignTable = pkb->getAssignMatches("b c * d e / + f g % - ", true);
  REQUIRE(assignTable.size() == 1);
  REQUIRE(assignTable.contains({"1", "a"}));
}

TEST_CASE("Follows relations") {
  std::string program = R"(
  procedure main {
    read a;
    read a;
    while (a > b) {
      read a;
      read a;
    }
    if (a > b) then {
      read a;
      read a;
    } else {
      read a;
      read a;
    }
    read a;
    read a;
  }
  )";
  std::stringstream ss;
  ss << program;
  std::list<Token> tokens = Lexer::tokenize(ss);
  auto pkb = Parser::parseSIMPLE(tokens);
  // Follows(s1, s2)
  auto followsTable = pkb->getFollows();
  REQUIRE(followsTable.size() == 8);
  // Within proc
  REQUIRE(followsTable.contains({"1", "2"}));
  REQUIRE(followsTable.contains({"2", "3"}));
  REQUIRE(followsTable.contains({"3", "6"}));
  REQUIRE(followsTable.contains({"6", "11"}));
  REQUIRE(followsTable.contains({"11", "12"}));
  // Within while
  REQUIRE(followsTable.contains({"4", "5"}));
  // Within if
  REQUIRE(followsTable.contains({"7", "8"}));
  REQUIRE(followsTable.contains({"9", "10"}));
}

TEST_CASE("Parent relations") {
  std::string program = R"(
  procedure main {
    while (a > b) {
      if (a > b) then {
        while (a > b) {
          read a;
        }
      } else {
        while (a > b) {
          read a;
        }
      }      
    }
  }
  )";
  std::stringstream ss;
  ss << program;
  std::list<Token> tokens = Lexer::tokenize(ss);
  auto pkb = Parser::parseSIMPLE(tokens);
  // Follows(s1, s2)
  auto parentTable = pkb->getParent();
  REQUIRE(parentTable.size() == 5);
  REQUIRE(parentTable.contains({"1", "2"}));
  REQUIRE(parentTable.contains({"2", "3"}));
  REQUIRE(parentTable.contains({"3", "4"}));
  REQUIRE(parentTable.contains({"2", "5"}));
  REQUIRE(parentTable.contains({"5", "6"}));
}

TEST_CASE("Next relations") {
  std::string program = R"(
  procedure main {
    read a;
    while (a > b) {
      read a;
      if (a > b) then {
        read a;
      } else {
        read a;
      }
    }
    read a;
  }
  )";
  std::stringstream ss;
  ss << program;
  std::list<Token> tokens = Lexer::tokenize(ss);
  auto pkb = Parser::parseSIMPLE(tokens);
  // Follows(s1, s2)
  auto nextTable = pkb->getNext();
  REQUIRE(nextTable.size() == 8);
  REQUIRE(nextTable.contains({"1", "2"}));
  REQUIRE(nextTable.contains({"2", "3"}));
  REQUIRE(nextTable.contains({"2", "7"}));
  REQUIRE(nextTable.contains({"3", "4"}));
  REQUIRE(nextTable.contains({"4", "5"}));
  REQUIRE(nextTable.contains({"4", "6"}));
  REQUIRE(nextTable.contains({"5", "2"}));
  REQUIRE(nextTable.contains({"6", "2"}));
}
