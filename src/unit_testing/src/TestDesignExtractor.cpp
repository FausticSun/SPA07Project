#include "DesignExtractor.h"
#include "PKB.h"
#include "catch.hpp"
#include <sstream>

TEST_CASE("Validate call to non-existant proc") {
  std::unique_ptr<PKB> pkb{new PKB()};
  pkb->setProc("a", 1, 2);
  pkb->setProc("b", 2, 3);
  pkb->setProc("c", 3, 4);
  pkb->setCalls("a", "d");
  REQUIRE_THROWS(DesignExtractor::populateDesigns(pkb));
}

TEST_CASE("Validate cyclic calls") {
  std::unique_ptr<PKB> pkb{new PKB()};
  pkb->setProc("a", 1, 2);
  pkb->setProc("b", 2, 3);
  pkb->setProc("c", 3, 4);
  pkb->setCalls("a", "b");
  pkb->setCalls("b", "c");
  pkb->setCalls("c", "a");
  REQUIRE_THROWS(DesignExtractor::populateDesigns(pkb));
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
