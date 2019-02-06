#include "TNode.h"

#include "catch.hpp"

SCENARIO("Construction of TNode") {

	TNode* tNode = new TNode(TNodeType::Procedure, "child", 1);

	SECTION("Type of TNode") {
		REQUIRE(tNode->getType() == TNodeType::Procedure);
	}

	SECTION("StatementNumber of TNode") {
		REQUIRE(tNode->getStatementNumber() == 1);
	}

	SECTION("Name of TNode") {
		REQUIRE(tNode->getName() == "child");
	}

	SECTION("TNode has no children during construction") {
		REQUIRE(tNode->getChildren().size() == 0);
	}

}

SCENARIO("Children of TNode") {

	TNode* childTNode = new TNode(TNodeType::Procedure, "child", 2);
	TNode* parentTNode = new TNode(TNodeType::Procedure, "parent", 1);

	SECTION("Adding one child to TNode") {
		parentTNode->setChildren({ childTNode });
		REQUIRE(parentTNode->getChildren().front() == childTNode);
	}

}
