#include "DesignExtractor.h"
#include "catch.hpp"

SCENARIO("Design Extractor extracts design abstractions from an AST into a PKB",
         "[DesignExtractor]") {
  GIVEN("An AST of an empty program") {
    std::unique_ptr<TNode> AST =
        std::unique_ptr<TNode>(new TNode(TNodeType::Program, ""));
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
    std::vector<std::unique_ptr<TNode>> childNodes;

    childNodes.emplace_back(new TNode(TNodeType::StatementList, ""));
    std::unique_ptr<TNode> proc1 = std::unique_ptr<TNode>(
        new TNode(TNodeType::Procedure, "Proc1", std::move(childNodes)));
    childNodes.clear();

    childNodes.emplace_back(new TNode(TNodeType::StatementList, ""));
    std::unique_ptr<TNode> proc2 = std::unique_ptr<TNode>(
        new TNode(TNodeType::Procedure, "Proc2", std::move(childNodes)));
    childNodes.clear();

    childNodes.emplace_back(new TNode(TNodeType::StatementList, ""));
    std::unique_ptr<TNode> proc3 = std::unique_ptr<TNode>(
        new TNode(TNodeType::Procedure, "Proc3", std::move(childNodes)));
    childNodes.clear();

    childNodes.push_back(std::move(proc1));
    childNodes.push_back(std::move(proc2));
    childNodes.push_back(std::move(proc3));
    std::unique_ptr<TNode> AST = std::unique_ptr<TNode>(
        new TNode(TNodeType::Program, std::move(childNodes)));

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
}
