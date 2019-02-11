#include "DesignExtractor.h"
#include "catch.hpp"

SCENARIO("Design Extractor extracts design abstractions from an AST into a PKB",
         "[DesignExtractor]") {
  GIVEN("An AST of an empty program") {
    TNode *AST = new TNode(TNodeType::Program);
    DesignExtractor de(AST);
    WHEN("The PKB is retrieved") {
      PKB pkb = de.getPKB();
      THEN("The PKB is empty") {
        REQUIRE(pkb.getProcTable().size() == 0);
        REQUIRE(pkb.getVarTable().size() == 0);
      }
    }
  }
  GIVEN("An AST of a program with 3 empty procedures") {
    TNode *AST = new TNode(TNodeType::Program);
    AST->children.push_back(new TNode(TNodeType::Procedure, "Proc1"));
    AST->children.push_back(new TNode(TNodeType::Procedure, "Proc2"));
    AST->children.push_back(new TNode(TNodeType::Procedure, "Proc3"));

    DesignExtractor de(AST);
    WHEN("The PKB is retrieved") {
      PKB pkb = de.getPKB();
      THEN("The PKB contains the 3 procedures") {
        auto procTable = pkb.getProcTable();
        REQUIRE(procTable.size() == 3);
        REQUIRE(procTable.find("Proc1") != procTable.end());
        REQUIRE(procTable.find("Proc2") != procTable.end());
        REQUIRE(procTable.find("Proc3") != procTable.end());
      }
      THEN("The PKB does not contain variables") {
        REQUIRE(pkb.getVarTable().size() == 0);
      }
    }
  }
  GIVEN("An AST of a program with a procedure with 3 variables") {
    TNode *var1 = new TNode(TNodeType::Variable, "x");
    TNode *proc = new TNode(TNodeType::Procedure, "main");
    TNode *AST = new TNode(TNodeType::Program);
  }
}
