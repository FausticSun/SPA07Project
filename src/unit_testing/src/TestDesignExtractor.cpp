#include "DesignExtractor.h"
#include "PKB.h"
#include "catch.hpp"
#include <sstream>

using Catch::Matchers::UnorderedEquals;

TEST_CASE("Validate call to non-existant proc") {
  std::unique_ptr<PKB> pkb{new PKB()};
  pkb->setProc("A", 1, 2);
  pkb->setProc("B", 2, 3);
  pkb->setProc("C", 3, 4);
  pkb->setCalls("A", "D");
  REQUIRE_THROWS_WITH(DesignExtractor::populateDesigns(pkb),
                      "PKB contains call to non-existent procedure");
}

TEST_CASE("Validate cyclic calls") {
  std::unique_ptr<PKB> pkb{new PKB()};
  pkb->setProc("A", 1, 2);
  pkb->setProc("B", 2, 3);
  pkb->setProc("C", 3, 4);
  pkb->setCalls("A", "B");
  pkb->setCalls("B", "C");
  pkb->setCalls("C", "A");
  REQUIRE_THROWS_WITH(DesignExtractor::populateDesigns(pkb),
                      "PKB contains cyclical calls");
}

TEST_CASE("Populate FollowsT") {
  std::unique_ptr<PKB> pkb{new PKB()};
  pkb->setFollows(1, 2);
  pkb->setFollows(2, 3);
  pkb->setFollows(3, 4);
  DesignExtractor::populateDesigns(pkb);
  auto followsTTable = pkb->getFollowsT();
  REQUIRE(followsTTable.size() == 6);
  REQUIRE(followsTTable.contains({"1", "2"}));
  REQUIRE(followsTTable.contains({"1", "3"}));
  REQUIRE(followsTTable.contains({"1", "4"}));
  REQUIRE(followsTTable.contains({"2", "3"}));
  REQUIRE(followsTTable.contains({"2", "4"}));
  REQUIRE(followsTTable.contains({"3", "4"}));
}

TEST_CASE("Populate ParentT") {
  std::unique_ptr<PKB> pkb{new PKB()};
  pkb->setParent(1, 2);
  pkb->setParent(2, 3);
  pkb->setParent(3, 4);
  DesignExtractor::populateDesigns(pkb);
  auto parentTTable = pkb->getParentT();
  REQUIRE(parentTTable.size() == 6);
  REQUIRE(parentTTable.contains({"1", "2"}));
  REQUIRE(parentTTable.contains({"1", "3"}));
  REQUIRE(parentTTable.contains({"1", "4"}));
  REQUIRE(parentTTable.contains({"2", "3"}));
  REQUIRE(parentTTable.contains({"2", "4"}));
  REQUIRE(parentTTable.contains({"3", "4"}));
}

TEST_CASE("Populate CallsT") {
  std::unique_ptr<PKB> pkb{new PKB()};
  pkb->setProc("A", 1, 2);
  pkb->setProc("B", 2, 3);
  pkb->setProc("C", 3, 4);
  pkb->setProc("D", 4, 5);
  pkb->setCalls("A", "B");
  pkb->setCalls("B", "C");
  pkb->setCalls("C", "D");
  DesignExtractor::populateDesigns(pkb);
  auto callsTTable = pkb->getCallsT();
  REQUIRE(callsTTable.size() == 6);
  REQUIRE(callsTTable.contains({"A", "B"}));
  REQUIRE(callsTTable.contains({"A", "C"}));
  REQUIRE(callsTTable.contains({"A", "D"}));
  REQUIRE(callsTTable.contains({"B", "C"}));
  REQUIRE(callsTTable.contains({"B", "D"}));
  REQUIRE(callsTTable.contains({"C", "D"}));
}

TEST_CASE("Populate UsesS") {
  std::unique_ptr<PKB> pkb{new PKB()};
  pkb->setUses(1, "a");
  pkb->setUses(2, "b");
  pkb->setUses(3, "c");
  pkb->setUses(4, "d");
  pkb->setParent(1, 2);
  pkb->setParent(1, 3);
  pkb->setParent(3, 4);
  pkb->setStmtType(1, StatementType::While);
  pkb->setStmtType(3, StatementType::While);
  DesignExtractor::populateDesigns(pkb);
  auto usesSTable = pkb->getUsesS();
  REQUIRE(usesSTable.size() == 8);
  REQUIRE(usesSTable.contains({"1", "a"}));
  REQUIRE(usesSTable.contains({"1", "b"}));
  REQUIRE(usesSTable.contains({"1", "c"}));
  REQUIRE(usesSTable.contains({"1", "d"}));
  REQUIRE(usesSTable.contains({"2", "b"}));
  REQUIRE(usesSTable.contains({"3", "c"}));
  REQUIRE(usesSTable.contains({"3", "d"}));
  REQUIRE(usesSTable.contains({"4", "d"}));
}

TEST_CASE("Populate ModifiesS") {
  std::unique_ptr<PKB> pkb{new PKB()};
  pkb->setModifies(2, "a");
  pkb->setModifies(4, "b");
  pkb->setParent(1, 2);
  pkb->setParent(1, 3);
  pkb->setParent(3, 4);
  pkb->setStmtType(1, StatementType::While);
  pkb->setStmtType(3, StatementType::While);
  DesignExtractor::populateDesigns(pkb);
  auto modifiesSTable = pkb->getModifiesS();
  REQUIRE(modifiesSTable.size() == 5);
  REQUIRE(modifiesSTable.contains({"1", "a"}));
  REQUIRE(modifiesSTable.contains({"1", "b"}));
  REQUIRE(modifiesSTable.contains({"2", "a"}));
  REQUIRE(modifiesSTable.contains({"3", "b"}));
  REQUIRE(modifiesSTable.contains({"4", "b"}));
}

TEST_CASE("Populate Use and Modifies for Call statements") {
  std::unique_ptr<PKB> pkb{new PKB()};
  pkb->setProc("A", 1, 2);
  pkb->setProc("B", 2, 4);
  pkb->setCalls("A", "B");
  pkb->setCallProcName(1, "B");
  pkb->setUses("B", "a");
  pkb->setUses("B", "b");
  pkb->setUses(2, "a");
  pkb->setUses(3, "b");
  pkb->setModifies("B", "c");
  pkb->setModifies("B", "d");
  pkb->setModifies(2, "c");
  pkb->setModifies(3, "d");
  DesignExtractor::populateDesigns(pkb);
  auto usesPTable = pkb->getUsesP();
  auto usesSTable = pkb->getUsesS();
  auto modifiesPTable = pkb->getModifiesP();
  auto modifiesSTable = pkb->getModifiesS();
  REQUIRE(usesPTable.size() == 4);
  REQUIRE(usesSTable.size() == 4);
  REQUIRE(modifiesPTable.size() == 4);
  REQUIRE(modifiesSTable.size() == 4);
  REQUIRE(usesPTable.contains({"A", "a"}));
  REQUIRE(usesPTable.contains({"A", "b"}));
  REQUIRE(usesSTable.contains({"1", "a"}));
  REQUIRE(usesSTable.contains({"1", "b"}));
  REQUIRE(usesSTable.contains({"2", "a"}));
  REQUIRE(usesSTable.contains({"3", "b"}));
  REQUIRE(modifiesPTable.contains({"A", "c"}));
  REQUIRE(modifiesPTable.contains({"A", "d"}));
  REQUIRE(modifiesSTable.contains({"1", "c"}));
  REQUIRE(modifiesSTable.contains({"1", "d"}));
  REQUIRE(modifiesSTable.contains({"2", "c"}));
  REQUIRE(modifiesSTable.contains({"3", "d"}));
}

TEST_CASE("Populate CFG") {
  SECTION("CFG with no if/while statements") {
    std::unique_ptr<PKB> pkb{new PKB()};
    pkb->setProc("A", 1, 5);
    pkb->setNext(1, 2);
    pkb->setNext(2, 3);
    pkb->setNext(3, 4);
    DesignExtractor::populateDesigns(pkb);
    auto CFG = pkb->getCFG("A");
    auto forwardCFG = CFG.getForwardCompressedGraph();
    auto reverseCFG = CFG.getReverseCompressedGraph();
    REQUIRE(forwardCFG.size() == 1);
    REQUIRE(reverseCFG.size() == 1);
  }

  SECTION("CFG with one if and one while statement") {
    std::unique_ptr<PKB> pkb{new PKB()};
    pkb->setProc("A", 1, 11);
    pkb->setNext(1, 2);
    pkb->setNext(2, 3);
    pkb->setNext(3, 1);
    pkb->setNext(1, 4);
    pkb->setNext(4, 5);
    pkb->setNext(5, 6);
    pkb->setNext(4, 7);
    pkb->setNext(7, 8);
    pkb->setNext(6, 9);
    pkb->setNext(8, 9);
    pkb->setNext(9, 10);
    pkb->setStmtType(1, StatementType::While);
    pkb->setStmtType(4, StatementType::If);
    DesignExtractor::populateDesigns(pkb);
    auto CFG = pkb->getCFG("A");
    auto forwardCFG = CFG.getForwardCompressedGraph();
    auto reverseCFG = CFG.getReverseCompressedGraph();
    REQUIRE(forwardCFG.size() == 6);
    REQUIRE(reverseCFG.size() == 6);
    REQUIRE_THAT(forwardCFG[0], UnorderedEquals(std::vector<int>{1, 2}));
    REQUIRE_THAT(forwardCFG[1], UnorderedEquals(std::vector<int>{0}));
    REQUIRE_THAT(forwardCFG[2], UnorderedEquals(std::vector<int>{3, 4}));
    REQUIRE_THAT(forwardCFG[3], UnorderedEquals(std::vector<int>{5}));
    REQUIRE_THAT(forwardCFG[4], UnorderedEquals(std::vector<int>{5}));
    REQUIRE_THAT(forwardCFG[5], UnorderedEquals(std::vector<int>{}));
    REQUIRE_THAT(reverseCFG[0], UnorderedEquals(std::vector<int>{1}));
    REQUIRE_THAT(reverseCFG[1], UnorderedEquals(std::vector<int>{0}));
    REQUIRE_THAT(reverseCFG[2], UnorderedEquals(std::vector<int>{1}));
    REQUIRE_THAT(reverseCFG[3], UnorderedEquals(std::vector<int>{2}));
    REQUIRE_THAT(reverseCFG[4], UnorderedEquals(std::vector<int>{2}));
    REQUIRE_THAT(reverseCFG[5], UnorderedEquals(std::vector<int>{3, 4}));
  }

  SECTION("CFG with two if statements nested") {
    std::unique_ptr<PKB> pkb{new PKB()};
    pkb->setProc("A", 1, 15);
    pkb->setNext(1, 2);
    pkb->setNext(2, 3);
    pkb->setNext(3, 4);
    pkb->setNext(3, 5);
    pkb->setNext(5, 6);
    pkb->setNext(6, 7);
    pkb->setNext(7, 12);
    pkb->setNext(4, 8);
    pkb->setNext(8, 9);
    pkb->setNext(9, 12);
    pkb->setNext(4, 10);
    pkb->setNext(10, 11);
    pkb->setNext(11, 12);
    pkb->setNext(12, 13);
    pkb->setNext(13, 14);
    pkb->setStmtType(3, StatementType::If);
    pkb->setStmtType(4, StatementType::If);
    DesignExtractor::populateDesigns(pkb);
    auto CFG = pkb->getCFG("A");
    auto forwardCFG = CFG.getForwardCompressedGraph();
    auto reverseCFG = CFG.getReverseCompressedGraph();
    REQUIRE(forwardCFG.size() == 7);
    REQUIRE(reverseCFG.size() == 7);
    REQUIRE_THAT(forwardCFG[0], UnorderedEquals(std::vector<int>{1}));
    REQUIRE_THAT(forwardCFG[1], UnorderedEquals(std::vector<int>{2, 3}));
    REQUIRE_THAT(forwardCFG[2], UnorderedEquals(std::vector<int>{5, 6}));
    REQUIRE_THAT(forwardCFG[3], UnorderedEquals(std::vector<int>{4}));
    REQUIRE_THAT(forwardCFG[4], UnorderedEquals(std::vector<int>{}));
    REQUIRE_THAT(forwardCFG[5], UnorderedEquals(std::vector<int>{4}));
    REQUIRE_THAT(forwardCFG[6], UnorderedEquals(std::vector<int>{4}));
    REQUIRE_THAT(reverseCFG[0], UnorderedEquals(std::vector<int>{}));
    REQUIRE_THAT(reverseCFG[1], UnorderedEquals(std::vector<int>{0}));
    REQUIRE_THAT(reverseCFG[2], UnorderedEquals(std::vector<int>{1}));
    REQUIRE_THAT(reverseCFG[3], UnorderedEquals(std::vector<int>{1}));
    REQUIRE_THAT(reverseCFG[4], UnorderedEquals(std::vector<int>{3, 5, 6}));
    REQUIRE_THAT(reverseCFG[5], UnorderedEquals(std::vector<int>{2}));
    REQUIRE_THAT(reverseCFG[6], UnorderedEquals(std::vector<int>{2}));
  }

  SECTION("CFG with two while statements nested") {
    std::unique_ptr<PKB> pkb{new PKB()};
    pkb->setProc("A", 1, 6);
    pkb->setNext(1, 2);
    pkb->setNext(1, 4);
    pkb->setNext(2, 1);
    pkb->setNext(2, 3);
    pkb->setNext(3, 2);
    pkb->setNext(4, 5);
    pkb->setStmtType(1, StatementType::While);
    pkb->setStmtType(2, StatementType::While);
    DesignExtractor::populateDesigns(pkb);
    auto CFG = pkb->getCFG("A");
    auto forwardCFG = CFG.getForwardCompressedGraph();
    auto reverseCFG = CFG.getReverseCompressedGraph();
    REQUIRE(forwardCFG.size() == 4);
    REQUIRE(reverseCFG.size() == 4);
    REQUIRE_THAT(forwardCFG[0], UnorderedEquals(std::vector<int>{1, 2}));
    REQUIRE_THAT(forwardCFG[1], UnorderedEquals(std::vector<int>{0, 3}));
    REQUIRE_THAT(forwardCFG[2], UnorderedEquals(std::vector<int>{}));
    REQUIRE_THAT(forwardCFG[3], UnorderedEquals(std::vector<int>{1}));
    REQUIRE_THAT(reverseCFG[0], UnorderedEquals(std::vector<int>{1}));
    REQUIRE_THAT(reverseCFG[1], UnorderedEquals(std::vector<int>{0, 3}));
    REQUIRE_THAT(reverseCFG[2], UnorderedEquals(std::vector<int>{0}));
    REQUIRE_THAT(reverseCFG[3], UnorderedEquals(std::vector<int>{2}));
  }

  SECTION("CFG with while nested in if statement") {
    std::unique_ptr<PKB> pkb{new PKB()};
    pkb->setProc("A", 1, 8);
    pkb->setNext(1, 2);
    pkb->setNext(1, 3);
    pkb->setNext(2, 5);
    pkb->setNext(3, 4);
    pkb->setNext(4, 3);
    pkb->setNext(3, 5);
    pkb->setNext(5, 6);
    pkb->setStmtType(1, StatementType::If);
    pkb->setStmtType(3, StatementType::While);
    DesignExtractor::populateDesigns(pkb);
    auto CFG = pkb->getCFG("A");
    auto forwardCFG = CFG.getForwardCompressedGraph();
    auto reverseCFG = CFG.getReverseCompressedGraph();
    std::vector<int> empty;
    REQUIRE(forwardCFG.size() == 5);
    REQUIRE(reverseCFG.size() == 5);
    REQUIRE_THAT(forwardCFG[0], UnorderedEquals(std::vector<int>{1, 2}));
    REQUIRE_THAT(forwardCFG[1], UnorderedEquals(std::vector<int>{4}));
    REQUIRE_THAT(forwardCFG[2], UnorderedEquals(std::vector<int>{3, 4}));
    REQUIRE_THAT(forwardCFG[3], UnorderedEquals(std::vector<int>{2}));
    REQUIRE_THAT(forwardCFG[4], UnorderedEquals(std::vector<int>{}));
    REQUIRE_THAT(reverseCFG[0], UnorderedEquals(std::vector<int>{}));
    REQUIRE_THAT(reverseCFG[1], UnorderedEquals(std::vector<int>{0}));
    REQUIRE_THAT(reverseCFG[2], UnorderedEquals(std::vector<int>{0, 3}));
    REQUIRE_THAT(reverseCFG[3], UnorderedEquals(std::vector<int>{2}));
    REQUIRE_THAT(reverseCFG[4], UnorderedEquals(std::vector<int>{1, 2}));
  }
  SECTION("CFG with if nested in while statement") {
    std::unique_ptr<PKB> pkb{new PKB()};
    pkb->setProc("A", 1, 11);
    pkb->setNext(1, 2);
    pkb->setNext(2, 3);
    pkb->setNext(3, 4);
    pkb->setNext(3, 9);
    pkb->setNext(9, 10);
    pkb->setNext(4, 5);
    pkb->setNext(5, 6);
    pkb->setNext(6, 3);
    pkb->setNext(4, 7);
    pkb->setNext(7, 8);
    pkb->setNext(8, 3);
    pkb->setStmtType(3, StatementType::While);
    pkb->setStmtType(4, StatementType::If);
    DesignExtractor::populateDesigns(pkb);
    auto CFG = pkb->getCFG("A");
    auto forwardCFG = CFG.getForwardCompressedGraph();
    auto reverseCFG = CFG.getReverseCompressedGraph();
    std::vector<int> empty;
    REQUIRE(forwardCFG.size() == 6);
    REQUIRE(reverseCFG.size() == 6);
    REQUIRE_THAT(forwardCFG[0], UnorderedEquals(std::vector<int>{1}));
    REQUIRE_THAT(forwardCFG[1], UnorderedEquals(std::vector<int>{2, 3}));
    REQUIRE_THAT(forwardCFG[2], UnorderedEquals(std::vector<int>{4, 5}));
    REQUIRE_THAT(forwardCFG[3], UnorderedEquals(std::vector<int>{}));
    REQUIRE_THAT(forwardCFG[4], UnorderedEquals(std::vector<int>{3}));
    REQUIRE_THAT(forwardCFG[5], UnorderedEquals(std::vector<int>{3}));
    REQUIRE_THAT(reverseCFG[0], UnorderedEquals(std::vector<int>{}));
    REQUIRE_THAT(reverseCFG[1], UnorderedEquals(std::vector<int>{0, 4, 5}));
    REQUIRE_THAT(reverseCFG[2], UnorderedEquals(std::vector<int>{3}));
    REQUIRE_THAT(reverseCFG[3], UnorderedEquals(std::vector<int>{1}));
    REQUIRE_THAT(reverseCFG[4], UnorderedEquals(std::vector<int>{2}));
    REQUIRE_THAT(reverseCFG[5], UnorderedEquals(std::vector<int>{2}));
  }

  SECTION("Multiple CFGs with multiple procedures") {
    std::unique_ptr<PKB> pkb{new PKB()};
    pkb->setProc("A", 1, 4);
    pkb->setProc("B", 4, 7);
    pkb->setNext(1, 2);
    pkb->setNext(2, 3);
    pkb->setNext(4, 5);
    pkb->setNext(5, 6);
    DesignExtractor::populateDesigns(pkb);
    auto CFGA = pkb->getCFG("A");
    auto CFGB = pkb->getCFG("B");
    auto forwardCFGA = CFGA.getForwardCompressedGraph();
    auto reverseCFGA = CFGA.getReverseCompressedGraph();
    auto forwardCFGB = CFGB.getForwardCompressedGraph();
    auto reverseCFGB = CFGB.getReverseCompressedGraph();
    REQUIRE(forwardCFGA.size() == 1);
    REQUIRE(reverseCFGA.size() == 1);
    REQUIRE(forwardCFGB.size() == 1);
    REQUIRE(reverseCFGB.size() == 1);
  }
}
