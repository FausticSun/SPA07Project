#include "PKB.h"
#include "DesignExtractor.h"
#include "catch.hpp"

using Catch::Matchers::UnorderedEquals;

TEST_CASE("No nesting in one procedure") {
	std::unique_ptr<PKB> pkb{ new PKB() };
	pkb->setProc("A", 1, 5);
	pkb->setNext(1, 2);
	pkb->setNext(2, 3);
	pkb->setNext(3, 4);
	DesignExtractor::populateDesigns(pkb);
	Table result = pkb->getNextT();

	//forward relations
	REQUIRE(result.size() == 6);
	REQUIRE(result.contains({ "1", "2" }));
	REQUIRE(result.contains({ "1", "3" }));
	REQUIRE(result.contains({ "1", "4" }));
	REQUIRE(result.contains({ "2", "3" }));
	REQUIRE(result.contains({ "2", "4" }));
	REQUIRE(result.contains({ "3", "4" }));
	REQUIRE(pkb->getNextT(1, 2) == true);
	REQUIRE(pkb->getNextT(1, 3) == true);
	REQUIRE(pkb->getNextT(1, 4) == true);
	REQUIRE(pkb->getNextT(2, 3) == true);
	REQUIRE(pkb->getNextT(2, 4) == true);
	REQUIRE(pkb->getNextT(3, 4) == true);

	//reverse relations
	result = pkb->getNextT(1, false);
	REQUIRE(result.size() == 0);

	result = pkb->getNextT(2, false);
	REQUIRE(result.size() == 1);
	REQUIRE(result.contains({ "1" }));

	result = pkb->getNextT(3, false);
	REQUIRE(result.size() == 2);
	REQUIRE(result.contains({ "1" }));
	REQUIRE(result.contains({ "2" }));

	result = pkb->getNextT(4, false);
	REQUIRE(result.size() == 3);
	REQUIRE(result.contains({ "1" }));
	REQUIRE(result.contains({ "2" }));
	REQUIRE(result.contains({ "3" }));

}


TEST_CASE("One if statement in one procedure") {
	std::unique_ptr<PKB> pkb{ new PKB() };
	pkb->setProc("A", 1, 9);
	pkb->setNext(1, 2);
	pkb->setNext(2, 3);
	pkb->setNext(3, 4);
	pkb->setNext(2, 5);
	pkb->setNext(5, 6);
	pkb->setNext(4, 7);
	pkb->setNext(6, 7);
	pkb->setNext(7, 8);
	pkb->setStmtType(2, StatementType::If);
	DesignExtractor::populateDesigns(pkb);
	Table result = pkb->getNextT();

	//forward relations
	REQUIRE(result.size() == 24);
	REQUIRE(result.contains({ "1", "2" }));
	REQUIRE(result.contains({ "1", "3" }));
	REQUIRE(result.contains({ "1", "4" }));
	REQUIRE(result.contains({ "1", "5" }));
	REQUIRE(result.contains({ "1", "6" }));
	REQUIRE(result.contains({ "1", "7" }));
	REQUIRE(result.contains({ "1", "8" }));
	REQUIRE(pkb->getNextT(1, 2) == true);
	REQUIRE(pkb->getNextT(1, 3) == true);
	REQUIRE(pkb->getNextT(1, 4) == true);
	REQUIRE(pkb->getNextT(1, 5) == true);
	REQUIRE(pkb->getNextT(1, 6) == true);
	REQUIRE(pkb->getNextT(1, 7) == true);
	REQUIRE(pkb->getNextT(1, 8) == true);

	REQUIRE(result.contains({ "2", "3" }));
	REQUIRE(result.contains({ "2", "4" }));
	REQUIRE(result.contains({ "2", "5" }));
	REQUIRE(result.contains({ "2", "6" }));
	REQUIRE(result.contains({ "2", "7" }));
	REQUIRE(result.contains({ "2", "8" }));
	REQUIRE(pkb->getNextT(2, 3) == true);
	REQUIRE(pkb->getNextT(2, 4) == true);
	REQUIRE(pkb->getNextT(2, 5) == true);
	REQUIRE(pkb->getNextT(2, 6) == true);
	REQUIRE(pkb->getNextT(2, 7) == true);
	REQUIRE(pkb->getNextT(2, 8) == true);


	REQUIRE(result.contains({ "3", "4" }));
	REQUIRE(result.contains({ "3", "7" }));
	REQUIRE(result.contains({ "3", "8" }));
	REQUIRE(pkb->getNextT(3, 4) == true);
	REQUIRE(pkb->getNextT(3, 7) == true);
	REQUIRE(pkb->getNextT(3, 8) == true);

	REQUIRE(result.contains({ "4", "7" }));
	REQUIRE(result.contains({ "4", "8" }));
	REQUIRE(pkb->getNextT(4, 7) == true);
	REQUIRE(pkb->getNextT(4, 8) == true);


	REQUIRE(result.contains({ "5", "6" }));
	REQUIRE(result.contains({ "5", "7" }));
	REQUIRE(result.contains({ "5", "8" }));
	REQUIRE(pkb->getNextT(5, 6) == true);
	REQUIRE(pkb->getNextT(5, 7) == true);
	REQUIRE(pkb->getNextT(5, 8) == true);


	REQUIRE(result.contains({ "6", "7" }));
	REQUIRE(result.contains({ "6", "8" }));
	REQUIRE(pkb->getNextT(6, 7) == true);
	REQUIRE(pkb->getNextT(6, 8) == true);

	REQUIRE(result.contains({ "7", "8" }));
	REQUIRE(pkb->getNextT(7, 8) == true);

	//reverse relations
	result = pkb->getNextT(1, false);
	REQUIRE(result.size() == 0);

	result = pkb->getNextT(2, false);
	REQUIRE(result.size() == 1);
	REQUIRE(result.contains({ "1" }));

	result = pkb->getNextT(3, false);
	REQUIRE(result.size() == 2);
	REQUIRE(result.contains({ "1" }));
	REQUIRE(result.contains({ "2" }));

	result = pkb->getNextT(4, false);
	REQUIRE(result.size() == 3);
	REQUIRE(result.contains({ "1" }));
	REQUIRE(result.contains({ "2" }));
	REQUIRE(result.contains({ "3" }));

	result = pkb->getNextT(5, false);
	REQUIRE(result.size() == 2);
	REQUIRE(result.contains({ "1" }));
	REQUIRE(result.contains({ "2" }));

	result = pkb->getNextT(6, false);
	REQUIRE(result.size() == 3);
	REQUIRE(result.contains({ "1" }));
	REQUIRE(result.contains({ "2" }));
	REQUIRE(result.contains({ "5" }));

	result = pkb->getNextT(7, false);
	REQUIRE(result.size() == 6);
	REQUIRE(result.contains({ "1" }));
	REQUIRE(result.contains({ "2" }));
	REQUIRE(result.contains({ "3" }));
	REQUIRE(result.contains({ "4" }));
	REQUIRE(result.contains({ "5" }));
	REQUIRE(result.contains({ "6" }));

	result = pkb->getNextT(8, false);
	REQUIRE(result.size() == 7);
	REQUIRE(result.contains({ "1" }));
	REQUIRE(result.contains({ "2" }));
	REQUIRE(result.contains({ "3" }));
	REQUIRE(result.contains({ "4" }));
	REQUIRE(result.contains({ "5" }));
	REQUIRE(result.contains({ "6" }));
	REQUIRE(result.contains({ "7" }));

}



TEST_CASE("One while loop in one procedure") {
	std::unique_ptr<PKB> pkb{ new PKB() };
	pkb->setProc("A", 1, 7);
	pkb->setNext(1, 2);
	pkb->setNext(2, 3);
	pkb->setNext(3, 4);
	pkb->setNext(4, 2);
	pkb->setNext(2, 5);
	pkb->setNext(5, 6);
	pkb->setStmtType(2, StatementType::While);
	DesignExtractor::populateDesigns(pkb);
	Table result = pkb->getNextT();

	//forward relations
	REQUIRE(result.size() == 21);

	REQUIRE(result.contains({ "1", "2" }));
	REQUIRE(result.contains({ "1", "3" }));
	REQUIRE(result.contains({ "1", "4" }));
	REQUIRE(result.contains({ "1", "5" }));
	REQUIRE(result.contains({ "1", "6" }));
	REQUIRE(pkb->getNextT(1, 2) == true);
	REQUIRE(pkb->getNextT(1, 3) == true);
	REQUIRE(pkb->getNextT(1, 4) == true);
	REQUIRE(pkb->getNextT(1, 5) == true);
	REQUIRE(pkb->getNextT(1, 6) == true);

	REQUIRE(result.contains({ "2", "2" }));
	REQUIRE(result.contains({ "2", "3" }));
	REQUIRE(result.contains({ "2", "4" }));
	REQUIRE(result.contains({ "2", "5" }));
	REQUIRE(result.contains({ "2", "6" }));
	REQUIRE(pkb->getNextT(2, 2) == true);
	REQUIRE(pkb->getNextT(2, 3) == true);
	REQUIRE(pkb->getNextT(2, 4) == true);
	REQUIRE(pkb->getNextT(2, 5) == true);
	REQUIRE(pkb->getNextT(2, 6) == true);

	REQUIRE(result.contains({ "3", "2" }));
	REQUIRE(result.contains({ "3", "3" }));
	REQUIRE(result.contains({ "3", "4" }));
	REQUIRE(result.contains({ "3", "5" }));
	REQUIRE(result.contains({ "3", "6" }));
	REQUIRE(pkb->getNextT(3, 2) == true);
	REQUIRE(pkb->getNextT(3, 3) == true);
	REQUIRE(pkb->getNextT(3, 4) == true);
	REQUIRE(pkb->getNextT(3, 5) == true);
	REQUIRE(pkb->getNextT(3, 6) == true);

	REQUIRE(result.contains({ "4", "2" }));
	REQUIRE(result.contains({ "4", "3" }));
	REQUIRE(result.contains({ "4", "4" }));
	REQUIRE(result.contains({ "4", "5" }));
	REQUIRE(result.contains({ "4", "6" }));
	REQUIRE(pkb->getNextT(4, 2) == true);
	REQUIRE(pkb->getNextT(4, 3) == true);
	REQUIRE(pkb->getNextT(4, 4) == true);
	REQUIRE(pkb->getNextT(4, 5) == true);
	REQUIRE(pkb->getNextT(4, 6) == true);

	REQUIRE(result.contains({ "5", "6" }));
	REQUIRE(pkb->getNextT(5, 6) == true);

	//reverse reltions
	result = pkb->getNextT(1, false);
	REQUIRE(result.size() == 0);

	result = pkb->getNextT(2, false);
	REQUIRE(result.size() == 4);
	REQUIRE(result.contains({ "1" }));
	REQUIRE(result.contains({ "3" }));
	REQUIRE(result.contains({ "3" }));
	REQUIRE(result.contains({ "4" }));

	result = pkb->getNextT(3, false);
	REQUIRE(result.size() == 4);
	REQUIRE(result.contains({ "1" }));
	REQUIRE(result.contains({ "3" }));
	REQUIRE(result.contains({ "3" }));
	REQUIRE(result.contains({ "4" }));

	result = pkb->getNextT(4, false);
	REQUIRE(result.size() == 4);
	REQUIRE(result.contains({ "1" }));
	REQUIRE(result.contains({ "3" }));
	REQUIRE(result.contains({ "3" }));
	REQUIRE(result.contains({ "4" }));

	result = pkb->getNextT(5, false);
	REQUIRE(result.size() == 4);
	REQUIRE(result.contains({ "1" }));
	REQUIRE(result.contains({ "3" }));
	REQUIRE(result.contains({ "3" }));
	REQUIRE(result.contains({ "4" }));

	result = pkb->getNextT(6, false);
	REQUIRE(result.size() == 5);
	REQUIRE(result.contains({ "1" }));
	REQUIRE(result.contains({ "3" }));
	REQUIRE(result.contains({ "3" }));
	REQUIRE(result.contains({ "4" }));
	REQUIRE(result.contains({ "5" }));

}


TEST_CASE("One if and one while loop unnested in one procedure") {
	std::unique_ptr<PKB> pkb{ new PKB() };
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
	Table result = pkb->getNextT();

	//forward relations
	REQUIRE(result.size() == 47);

	REQUIRE(result.contains({ "1", "1" }));
	REQUIRE(result.contains({ "1", "2" }));
	REQUIRE(result.contains({ "1", "3" }));
	REQUIRE(result.contains({ "1", "4" }));
	REQUIRE(result.contains({ "1", "5" }));
	REQUIRE(result.contains({ "1", "6" }));
	REQUIRE(result.contains({ "1", "7" }));
	REQUIRE(result.contains({ "1", "8" }));
	REQUIRE(result.contains({ "1", "9" }));
	REQUIRE(result.contains({ "1", "10" }));
	REQUIRE(pkb->getNextT(1, 1) == true);
	REQUIRE(pkb->getNextT(1, 2) == true);
	REQUIRE(pkb->getNextT(1, 3) == true);
	REQUIRE(pkb->getNextT(1, 4) == true);
	REQUIRE(pkb->getNextT(1, 5) == true);
	REQUIRE(pkb->getNextT(1, 6) == true);
	REQUIRE(pkb->getNextT(1, 7) == true);
	REQUIRE(pkb->getNextT(1, 8) == true);
	REQUIRE(pkb->getNextT(1, 9) == true);
	REQUIRE(pkb->getNextT(1, 10) == true);

	REQUIRE(result.contains({ "2", "1" }));
	REQUIRE(result.contains({ "2", "2" }));
	REQUIRE(result.contains({ "2", "3" }));
	REQUIRE(result.contains({ "2", "4" }));
	REQUIRE(result.contains({ "2", "5" }));
	REQUIRE(result.contains({ "2", "6" }));
	REQUIRE(result.contains({ "2", "7" }));
	REQUIRE(result.contains({ "2", "8" }));
	REQUIRE(result.contains({ "2", "9" }));
	REQUIRE(result.contains({ "2", "10" }));
	REQUIRE(pkb->getNextT(2, 1) == true);
	REQUIRE(pkb->getNextT(2, 2) == true);
	REQUIRE(pkb->getNextT(2, 3) == true);
	REQUIRE(pkb->getNextT(2, 4) == true);
	REQUIRE(pkb->getNextT(2, 5) == true);
	REQUIRE(pkb->getNextT(2, 6) == true);
	REQUIRE(pkb->getNextT(2, 7) == true);
	REQUIRE(pkb->getNextT(2, 8) == true);
	REQUIRE(pkb->getNextT(2, 9) == true);
	REQUIRE(pkb->getNextT(2, 10) == true);

	REQUIRE(result.contains({ "3", "1" }));
	REQUIRE(result.contains({ "3", "2" }));
	REQUIRE(result.contains({ "3", "3" }));
	REQUIRE(result.contains({ "3", "4" }));
	REQUIRE(result.contains({ "3", "5" }));
	REQUIRE(result.contains({ "3", "6" }));
	REQUIRE(result.contains({ "3", "7" }));
	REQUIRE(result.contains({ "3", "8" }));
	REQUIRE(result.contains({ "3", "9" }));
	REQUIRE(result.contains({ "3", "10" }));
	REQUIRE(pkb->getNextT(3, 1) == true);
	REQUIRE(pkb->getNextT(3, 2) == true);
	REQUIRE(pkb->getNextT(3, 3) == true);
	REQUIRE(pkb->getNextT(3, 4) == true);
	REQUIRE(pkb->getNextT(3, 5) == true);
	REQUIRE(pkb->getNextT(3, 6) == true);
	REQUIRE(pkb->getNextT(3, 7) == true);
	REQUIRE(pkb->getNextT(3, 8) == true);
	REQUIRE(pkb->getNextT(3, 9) == true);
	REQUIRE(pkb->getNextT(3, 10) == true);

	REQUIRE(result.contains({ "4", "5" }));
	REQUIRE(result.contains({ "4", "6" }));
	REQUIRE(result.contains({ "4", "7" }));
	REQUIRE(result.contains({ "4", "8" }));
	REQUIRE(result.contains({ "4", "9" }));
	REQUIRE(result.contains({ "4", "10" }));
	REQUIRE(pkb->getNextT(4, 5) == true);
	REQUIRE(pkb->getNextT(4, 6) == true);
	REQUIRE(pkb->getNextT(4, 7) == true);
	REQUIRE(pkb->getNextT(4, 8) == true);
	REQUIRE(pkb->getNextT(4, 9) == true);
	REQUIRE(pkb->getNextT(4, 10) == true);

	REQUIRE(result.contains({ "5", "6" }));
	REQUIRE(result.contains({ "5", "9" }));
	REQUIRE(result.contains({ "5", "10" }));
	REQUIRE(pkb->getNextT(5, 6) == true);
	REQUIRE(pkb->getNextT(5, 9) == true);
	REQUIRE(pkb->getNextT(5, 10) == true);

	REQUIRE(result.contains({ "6", "9" }));
	REQUIRE(result.contains({ "6", "10" }));
	REQUIRE(pkb->getNextT(6, 9) == true);
	REQUIRE(pkb->getNextT(6, 10) == true);

	REQUIRE(result.contains({ "7", "8" }));
	REQUIRE(result.contains({ "7", "9" }));
	REQUIRE(result.contains({ "7", "10" }));
	REQUIRE(pkb->getNextT(7, 8) == true);
	REQUIRE(pkb->getNextT(7, 9) == true);
	REQUIRE(pkb->getNextT(7, 10) == true);

	REQUIRE(result.contains({ "8", "9" }));
	REQUIRE(result.contains({ "8", "10" }));
	REQUIRE(pkb->getNextT(8, 9) == true);
	REQUIRE(pkb->getNextT(8, 10) == true);

	REQUIRE(result.contains({ "9", "10" }));
	REQUIRE(pkb->getNextT(9, 10) == true);

	//reverse relations
	result = pkb->getNextT(1, false);
	REQUIRE(result.size() == 3);
	REQUIRE(result.contains({ "1" }));
	REQUIRE(result.contains({ "2" }));
	REQUIRE(result.contains({ "3" }));

	result = pkb->getNextT(2, false);
	REQUIRE(result.size() == 3);
	REQUIRE(result.contains({ "1" }));
	REQUIRE(result.contains({ "2" }));
	REQUIRE(result.contains({ "3" }));

	result = pkb->getNextT(3, false);
	REQUIRE(result.size() == 3);
	REQUIRE(result.contains({ "1" }));
	REQUIRE(result.contains({ "2" }));
	REQUIRE(result.contains({ "3" }));

	result = pkb->getNextT(4, false);
	REQUIRE(result.size() == 3);
	REQUIRE(result.contains({ "1" }));
	REQUIRE(result.contains({ "2" }));
	REQUIRE(result.contains({ "3" }));

	result = pkb->getNextT(5, false);
	REQUIRE(result.size() == 4);
	REQUIRE(result.contains({ "1" }));
	REQUIRE(result.contains({ "2" }));
	REQUIRE(result.contains({ "3" }));
	REQUIRE(result.contains({ "4" }));

	result = pkb->getNextT(6, false);
	REQUIRE(result.size() == 5);
	REQUIRE(result.contains({ "1" }));
	REQUIRE(result.contains({ "2" }));
	REQUIRE(result.contains({ "3" }));
	REQUIRE(result.contains({ "4" }));
	REQUIRE(result.contains({ "5" }));

	result = pkb->getNextT(7, false);
	REQUIRE(result.size() == 4);
	REQUIRE(result.contains({ "1" }));
	REQUIRE(result.contains({ "2" }));
	REQUIRE(result.contains({ "3" }));
	REQUIRE(result.contains({ "4" }));

	result = pkb->getNextT(8, false);
	REQUIRE(result.size() == 5);
	REQUIRE(result.contains({ "1" }));
	REQUIRE(result.contains({ "2" }));
	REQUIRE(result.contains({ "3" }));
	REQUIRE(result.contains({ "4" }));
	REQUIRE(result.contains({ "7" }));

	result = pkb->getNextT(9, false);
	REQUIRE(result.size() == 8);
	REQUIRE(result.contains({ "1" }));
	REQUIRE(result.contains({ "2" }));
	REQUIRE(result.contains({ "3" }));
	REQUIRE(result.contains({ "4" }));
	REQUIRE(result.contains({ "5" }));
	REQUIRE(result.contains({ "6" }));
	REQUIRE(result.contains({ "7" }));
	REQUIRE(result.contains({ "8" }));

	result = pkb->getNextT(10, false);
	REQUIRE(result.size() == 9);
	REQUIRE(result.contains({ "1" }));
	REQUIRE(result.contains({ "2" }));
	REQUIRE(result.contains({ "3" }));
	REQUIRE(result.contains({ "4" }));
	REQUIRE(result.contains({ "5" }));
	REQUIRE(result.contains({ "6" }));
	REQUIRE(result.contains({ "7" }));
	REQUIRE(result.contains({ "8" }));
	REQUIRE(result.contains({ "9" }));

}


TEST_CASE("Two if statements nested in one procedure") {
	std::unique_ptr<PKB> pkb{ new PKB() };
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
	Table result = pkb->getNextT();

	//forward relations
	REQUIRE(result.size() == 72);

	REQUIRE(result.contains({ "1", "2" }));
	REQUIRE(result.contains({ "1", "3" }));
	REQUIRE(result.contains({ "1", "4" }));
	REQUIRE(result.contains({ "1", "5" }));
	REQUIRE(result.contains({ "1", "6" }));
	REQUIRE(result.contains({ "1", "7" }));
	REQUIRE(result.contains({ "1", "8" }));
	REQUIRE(result.contains({ "1", "9" }));
	REQUIRE(result.contains({ "1", "10" }));
	REQUIRE(result.contains({ "1", "11" }));
	REQUIRE(result.contains({ "1", "12" }));
	REQUIRE(result.contains({ "1", "13" }));
	REQUIRE(result.contains({ "1", "14" }));
	REQUIRE(pkb->getNextT(1, 2) == true);
	REQUIRE(pkb->getNextT(1, 3) == true);
	REQUIRE(pkb->getNextT(1, 4) == true);
	REQUIRE(pkb->getNextT(1, 5) == true);
	REQUIRE(pkb->getNextT(1, 6) == true);
	REQUIRE(pkb->getNextT(1, 7) == true);
	REQUIRE(pkb->getNextT(1, 8) == true);
	REQUIRE(pkb->getNextT(1, 9) == true);
	REQUIRE(pkb->getNextT(1, 10) == true);
	REQUIRE(pkb->getNextT(1, 11) == true);
	REQUIRE(pkb->getNextT(1, 12)== true);
	REQUIRE(pkb->getNextT(1, 13) == true);
	REQUIRE(pkb->getNextT(1, 14) == true);

	REQUIRE(result.contains({ "2", "3" }));
	REQUIRE(result.contains({ "2", "4" }));
	REQUIRE(result.contains({ "2", "5" }));
	REQUIRE(result.contains({ "2", "6" }));
	REQUIRE(result.contains({ "2", "7" }));
	REQUIRE(result.contains({ "2", "8" }));
	REQUIRE(result.contains({ "2", "9" }));
	REQUIRE(result.contains({ "2", "10" }));
	REQUIRE(result.contains({ "2", "11" }));
	REQUIRE(result.contains({ "2", "12" }));
	REQUIRE(result.contains({ "2", "13" }));
	REQUIRE(result.contains({ "2", "14" }));
	REQUIRE(pkb->getNextT(2, 3) == true);
	REQUIRE(pkb->getNextT(2, 4) == true);
	REQUIRE(pkb->getNextT(2, 5) == true);
	REQUIRE(pkb->getNextT(2, 6) == true);
	REQUIRE(pkb->getNextT(2, 7) == true);
	REQUIRE(pkb->getNextT(2, 8) == true);
	REQUIRE(pkb->getNextT(2, 9) == true);
	REQUIRE(pkb->getNextT(2, 10) == true);
	REQUIRE(pkb->getNextT(2, 11) == true);
	REQUIRE(pkb->getNextT(2, 12) == true);
	REQUIRE(pkb->getNextT(2, 13) == true);
	REQUIRE(pkb->getNextT(2, 14) == true);

	REQUIRE(result.contains({ "3", "4" }));
	REQUIRE(result.contains({ "3", "5" }));
	REQUIRE(result.contains({ "3", "6" }));
	REQUIRE(result.contains({ "3", "7" }));
	REQUIRE(result.contains({ "3", "8" }));
	REQUIRE(result.contains({ "3", "9" }));
	REQUIRE(result.contains({ "3", "10" }));
	REQUIRE(result.contains({ "3", "11" }));
	REQUIRE(result.contains({ "3", "12" }));
	REQUIRE(result.contains({ "3", "13" }));
	REQUIRE(result.contains({ "3", "14" }));
	REQUIRE(pkb->getNextT(3, 4) == true);
	REQUIRE(pkb->getNextT(3, 5) == true);
	REQUIRE(pkb->getNextT(3, 6) == true);
	REQUIRE(pkb->getNextT(3, 7) == true);
	REQUIRE(pkb->getNextT(3, 8) == true);
	REQUIRE(pkb->getNextT(3, 9) == true);
	REQUIRE(pkb->getNextT(3, 10) == true);
	REQUIRE(pkb->getNextT(3, 11) == true);
	REQUIRE(pkb->getNextT(3, 12) == true);
	REQUIRE(pkb->getNextT(3, 13) == true);
	REQUIRE(pkb->getNextT(3, 14) == true);

	REQUIRE(result.contains({ "4", "8" }));
	REQUIRE(result.contains({ "4", "9" }));
	REQUIRE(result.contains({ "4", "10" }));
	REQUIRE(result.contains({ "4", "11" }));
	REQUIRE(result.contains({ "4", "12" }));
	REQUIRE(result.contains({ "4", "13" }));
	REQUIRE(result.contains({ "4", "14" }));
	REQUIRE(pkb->getNextT(4, 8) == true);
	REQUIRE(pkb->getNextT(4, 9) == true);
	REQUIRE(pkb->getNextT(4, 10) == true);
	REQUIRE(pkb->getNextT(4, 11) == true);
	REQUIRE(pkb->getNextT(4, 12) == true);
	REQUIRE(pkb->getNextT(4, 13) == true);
	REQUIRE(pkb->getNextT(4, 14) == true);

	REQUIRE(result.contains({ "5", "6" }));
	REQUIRE(result.contains({ "5", "7" }));
	REQUIRE(result.contains({ "5", "12" }));
	REQUIRE(result.contains({ "5", "13" }));
	REQUIRE(result.contains({ "5", "14" }));
	REQUIRE(pkb->getNextT(5, 6) == true);
	REQUIRE(pkb->getNextT(5, 7) == true);
	REQUIRE(pkb->getNextT(5, 12) == true);
	REQUIRE(pkb->getNextT(5, 13) == true);
	REQUIRE(pkb->getNextT(5, 14) == true);

	REQUIRE(result.contains({ "6", "7" }));
	REQUIRE(result.contains({ "6", "12" }));
	REQUIRE(result.contains({ "6", "13" }));
	REQUIRE(result.contains({ "6", "14" }));
	REQUIRE(pkb->getNextT(6, 7) == true);
	REQUIRE(pkb->getNextT(6, 12) == true);
	REQUIRE(pkb->getNextT(6, 13) == true);
	REQUIRE(pkb->getNextT(6, 14) == true);

	REQUIRE(result.contains({ "7", "12" }));
	REQUIRE(result.contains({ "7", "13" }));
	REQUIRE(result.contains({ "7", "14" }));
	REQUIRE(pkb->getNextT(7, 12) == true);
	REQUIRE(pkb->getNextT(7, 13) == true);
	REQUIRE(pkb->getNextT(7, 14) == true);

	REQUIRE(result.contains({ "8", "9" }));
	REQUIRE(result.contains({ "8", "12" }));
	REQUIRE(result.contains({ "8", "13" }));
	REQUIRE(result.contains({ "8", "14" }));
	REQUIRE(pkb->getNextT(8, 9) == true);
	REQUIRE(pkb->getNextT(8, 12) == true);
	REQUIRE(pkb->getNextT(8, 13) == true);
	REQUIRE(pkb->getNextT(8, 14) == true);

	REQUIRE(result.contains({ "9", "12" }));
	REQUIRE(result.contains({ "9", "13" }));
	REQUIRE(result.contains({ "9", "14" }));
	REQUIRE(pkb->getNextT(9, 12) == true);
	REQUIRE(pkb->getNextT(9, 13) == true);
	REQUIRE(pkb->getNextT(9, 14) == true);

	REQUIRE(result.contains({ "10", "11" }));
	REQUIRE(result.contains({ "10", "12" }));
	REQUIRE(result.contains({ "10", "13" }));
	REQUIRE(result.contains({ "10", "14" }));
	REQUIRE(pkb->getNextT(10, 11) == true);
	REQUIRE(pkb->getNextT(10, 12) == true);
	REQUIRE(pkb->getNextT(10, 13) == true);
	REQUIRE(pkb->getNextT(10, 14) == true);

	REQUIRE(result.contains({ "11", "12" }));
	REQUIRE(result.contains({ "11", "13" }));
	REQUIRE(result.contains({ "11", "14" }));
	REQUIRE(pkb->getNextT(11, 12) == true);
	REQUIRE(pkb->getNextT(11, 13) == true);
	REQUIRE(pkb->getNextT(11, 14) == true);

	REQUIRE(result.contains({ "12", "13" }));
	REQUIRE(result.contains({ "12", "14" }));
	REQUIRE(pkb->getNextT(12, 13) == true);
	REQUIRE(pkb->getNextT(12, 14) == true);

	REQUIRE(result.contains({ "13", "14" }));
	REQUIRE(pkb->getNextT(13, 14) == true);

	//reverse relations
	result = pkb->getNextT(1, false);
	REQUIRE(result.size() == 0);

	result = pkb->getNextT(2, false);
	REQUIRE(result.size() == 1);
	REQUIRE(result.contains({ "1" }));

	result = pkb->getNextT(3, false);
	REQUIRE(result.size() == 2);
	REQUIRE(result.contains({ "1" }));
	REQUIRE(result.contains({ "2" }));

	result = pkb->getNextT(4, false);
	REQUIRE(result.size() == 3);
	REQUIRE(result.contains({ "1" }));
	REQUIRE(result.contains({ "2" }));
	REQUIRE(result.contains({ "3" }));

	result = pkb->getNextT(5, false);
	REQUIRE(result.size() == 3);
	REQUIRE(result.contains({ "1" }));
	REQUIRE(result.contains({ "2" }));
	REQUIRE(result.contains({ "3" }));

	result = pkb->getNextT(6, false);
	REQUIRE(result.size() == 4);
	REQUIRE(result.contains({ "1" }));
	REQUIRE(result.contains({ "2" }));
	REQUIRE(result.contains({ "3" }));
	REQUIRE(result.contains({ "5" }));

	result = pkb->getNextT(7, false);
	REQUIRE(result.size() == 5);
	REQUIRE(result.contains({ "1" }));
	REQUIRE(result.contains({ "2" }));
	REQUIRE(result.contains({ "3" }));
	REQUIRE(result.contains({ "5" }));
	REQUIRE(result.contains({ "6" }));

	result = pkb->getNextT(8, false);
	REQUIRE(result.size() == 4);
	REQUIRE(result.contains({ "1" }));
	REQUIRE(result.contains({ "2" }));
	REQUIRE(result.contains({ "3" }));
	REQUIRE(result.contains({ "4" }));

	result = pkb->getNextT(9, false);
	REQUIRE(result.size() == 5);
	REQUIRE(result.contains({ "1" }));
	REQUIRE(result.contains({ "2" }));
	REQUIRE(result.contains({ "3" }));
	REQUIRE(result.contains({ "4" }));
	REQUIRE(result.contains({ "8" }));

	result = pkb->getNextT(10, false);
	REQUIRE(result.size() == 4);
	REQUIRE(result.contains({ "1" }));
	REQUIRE(result.contains({ "2" }));
	REQUIRE(result.contains({ "3" }));
	REQUIRE(result.contains({ "4" }));

	result = pkb->getNextT(11, false);
	REQUIRE(result.size() == 5);
	REQUIRE(result.contains({ "1" }));
	REQUIRE(result.contains({ "2" }));
	REQUIRE(result.contains({ "3" }));
	REQUIRE(result.contains({ "4" }));
	REQUIRE(result.contains({ "10" }));

	result = pkb->getNextT(12, false);
	REQUIRE(result.size() == 11);
	REQUIRE(result.contains({ "1" }));
	REQUIRE(result.contains({ "2" }));
	REQUIRE(result.contains({ "3" }));
	REQUIRE(result.contains({ "4" }));
	REQUIRE(result.contains({ "5" }));
	REQUIRE(result.contains({ "6" }));
	REQUIRE(result.contains({ "7" }));
	REQUIRE(result.contains({ "8" }));
	REQUIRE(result.contains({ "9" }));
	REQUIRE(result.contains({ "10" }));
	REQUIRE(result.contains({ "11" }));

	result = pkb->getNextT(13, false);
	REQUIRE(result.size() == 12);
	REQUIRE(result.contains({ "1" }));
	REQUIRE(result.contains({ "2" }));
	REQUIRE(result.contains({ "3" }));
	REQUIRE(result.contains({ "4" }));
	REQUIRE(result.contains({ "5" }));
	REQUIRE(result.contains({ "6" }));
	REQUIRE(result.contains({ "7" }));
	REQUIRE(result.contains({ "8" }));
	REQUIRE(result.contains({ "9" }));
	REQUIRE(result.contains({ "10" }));
	REQUIRE(result.contains({ "11" }));
	REQUIRE(result.contains({ "12" }));

	result = pkb->getNextT(14, false);
	REQUIRE(result.size() == 13);
	REQUIRE(result.contains({ "1" }));
	REQUIRE(result.contains({ "2" }));
	REQUIRE(result.contains({ "3" }));
	REQUIRE(result.contains({ "4" }));
	REQUIRE(result.contains({ "5" }));
	REQUIRE(result.contains({ "6" }));
	REQUIRE(result.contains({ "7" }));
	REQUIRE(result.contains({ "8" }));
	REQUIRE(result.contains({ "9" }));
	REQUIRE(result.contains({ "10" }));
	REQUIRE(result.contains({ "11" }));
	REQUIRE(result.contains({ "12" }));
	REQUIRE(result.contains({ "13" }));

}


TEST_CASE("Two while loops nested in one procedure") {
	std::unique_ptr<PKB> pkb{ new PKB() };
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
	Table result = pkb->getNextT();

	//forward relations
	REQUIRE(result.size() == 16);

	REQUIRE(result.contains({ "1", "1" }));
	REQUIRE(result.contains({ "1", "2" }));
	REQUIRE(result.contains({ "1", "3" }));
	REQUIRE(result.contains({ "1", "4" }));
	REQUIRE(result.contains({ "1", "5" }));
	REQUIRE(pkb->getNextT(1, 1) == true);
	REQUIRE(pkb->getNextT(1, 2) == true);
	REQUIRE(pkb->getNextT(1, 3) == true);
	REQUIRE(pkb->getNextT(1, 4) == true);
	REQUIRE(pkb->getNextT(1, 5) == true);

	REQUIRE(result.contains({ "2", "1" }));
	REQUIRE(result.contains({ "2", "2" }));
	REQUIRE(result.contains({ "2", "3" }));
	REQUIRE(result.contains({ "2", "4" }));
	REQUIRE(result.contains({ "2", "5" }));
	REQUIRE(pkb->getNextT(2, 1) == true);
	REQUIRE(pkb->getNextT(2, 2) == true);
	REQUIRE(pkb->getNextT(2, 3) == true);
	REQUIRE(pkb->getNextT(2, 4) == true);
	REQUIRE(pkb->getNextT(2, 5) == true);

	REQUIRE(result.contains({ "3", "1" }));
	REQUIRE(result.contains({ "3", "2" }));
	REQUIRE(result.contains({ "3", "3" }));
	REQUIRE(result.contains({ "3", "4" }));
	REQUIRE(result.contains({ "3", "5" }));
	REQUIRE(pkb->getNextT(3, 1) == true);
	REQUIRE(pkb->getNextT(3, 2) == true);
	REQUIRE(pkb->getNextT(3, 3) == true);
	REQUIRE(pkb->getNextT(3, 4) == true);
	REQUIRE(pkb->getNextT(3, 5) == true);

	REQUIRE(result.contains({ "4", "5" }));
	REQUIRE(pkb->getNextT(4, 5) == true);

	//reverse relations
	result = pkb->getNextT(1, false);
	REQUIRE(result.size() == 3);
	REQUIRE(result.contains({ "1" }));
	REQUIRE(result.contains({ "2" }));
	REQUIRE(result.contains({ "3" }));

	result = pkb->getNextT(2, false);
	REQUIRE(result.size() == 3);
	REQUIRE(result.contains({ "1" }));
	REQUIRE(result.contains({ "2" }));
	REQUIRE(result.contains({ "3" }));

	result = pkb->getNextT(3, false);
	REQUIRE(result.size() == 3);
	REQUIRE(result.contains({ "1" }));
	REQUIRE(result.contains({ "2" }));
	REQUIRE(result.contains({ "3" }));

	result = pkb->getNextT(4, false);
	REQUIRE(result.size() == 3);
	REQUIRE(result.contains({ "1" }));
	REQUIRE(result.contains({ "2" }));
	REQUIRE(result.contains({ "3" }));

	result = pkb->getNextT(5, false);
	REQUIRE(result.size() == 4);
	REQUIRE(result.contains({ "1" }));
	REQUIRE(result.contains({ "2" }));
	REQUIRE(result.contains({ "3" }));
	REQUIRE(result.contains({ "4" }));

}

TEST_CASE("One while loop nested in one if statement in one procedure") {
	std::unique_ptr<PKB> pkb{ new PKB() };
	pkb->setProc("A", 1, 7);
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
	Table result = pkb->getNextT();

	//forward relations
	REQUIRE(result.size() == 16);

	REQUIRE(result.contains({ "1", "2" }));
	REQUIRE(result.contains({ "1", "3" }));
	REQUIRE(result.contains({ "1", "4" }));
	REQUIRE(result.contains({ "1", "5" }));
	REQUIRE(result.contains({ "1", "6" }));
	REQUIRE(pkb->getNextT(1, 2) == true);
	REQUIRE(pkb->getNextT(1, 3) == true);
	REQUIRE(pkb->getNextT(1, 4) == true);
	REQUIRE(pkb->getNextT(1, 5) == true);
	REQUIRE(pkb->getNextT(1, 6) == true);

	REQUIRE(result.contains({ "2", "5" }));
	REQUIRE(result.contains({ "2", "6" }));
	REQUIRE(pkb->getNextT(2, 5) == true);
	REQUIRE(pkb->getNextT(2, 6) == true);

	REQUIRE(result.contains({ "3", "3" }));
	REQUIRE(result.contains({ "3", "4" }));
	REQUIRE(result.contains({ "3", "5" }));
	REQUIRE(result.contains({ "3", "6" }));
	REQUIRE(pkb->getNextT(3, 3) == true);
	REQUIRE(pkb->getNextT(3, 4) == true);
	REQUIRE(pkb->getNextT(3, 5) == true);
	REQUIRE(pkb->getNextT(3, 6) == true);

	REQUIRE(result.contains({ "4", "3" }));
	REQUIRE(result.contains({ "4", "4" }));
	REQUIRE(result.contains({ "4", "5" }));
	REQUIRE(result.contains({ "4", "6" }));
	REQUIRE(pkb->getNextT(4, 3) == true);
	REQUIRE(pkb->getNextT(4, 4) == true);
	REQUIRE(pkb->getNextT(4, 5) == true);
	REQUIRE(pkb->getNextT(4, 6) == true);

	REQUIRE(result.contains({ "5", "6" }));
	REQUIRE(pkb->getNextT(5, 6) == true);

	//reverse relations
	result = pkb->getNextT(1, false);
	REQUIRE(result.size() == 0);

	result = pkb->getNextT(2, false);
	REQUIRE(result.size() == 1);
	REQUIRE(result.contains({ "1" }));

	result = pkb->getNextT(3, false);
	REQUIRE(result.size() == 3);
	REQUIRE(result.contains({ "1" }));
	REQUIRE(result.contains({ "3" }));
	REQUIRE(result.contains({ "4" }));

	result = pkb->getNextT(4, false);
	REQUIRE(result.size() == 3);
	REQUIRE(result.contains({ "1" }));
	REQUIRE(result.contains({ "3" }));
	REQUIRE(result.contains({ "4" }));

	result = pkb->getNextT(5, false);
	REQUIRE(result.size() == 4);
	REQUIRE(result.contains({ "1" }));
	REQUIRE(result.contains({ "2" }));
	REQUIRE(result.contains({ "3" }));
	REQUIRE(result.contains({ "4" }));

	result = pkb->getNextT(6, false);
	REQUIRE(result.size() == 5);
	REQUIRE(result.contains({ "1" }));
	REQUIRE(result.contains({ "2" }));
	REQUIRE(result.contains({ "3" }));
	REQUIRE(result.contains({ "4" }));
	REQUIRE(result.contains({ "5" }));

}

TEST_CASE("One if statement nested in one while loop in one procedure") {
	std::unique_ptr<PKB> pkb{ new PKB() };
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
	Table result = pkb->getNextT();

	//forward relations
	REQUIRE(result.size() == 66);

	REQUIRE(result.contains({ "1", "2" }));
	REQUIRE(result.contains({ "1", "3" }));
	REQUIRE(result.contains({ "1", "4" }));
	REQUIRE(result.contains({ "1", "5" }));
	REQUIRE(result.contains({ "1", "6" }));
	REQUIRE(result.contains({ "1", "7" }));
	REQUIRE(result.contains({ "1", "8" }));
	REQUIRE(result.contains({ "1", "9" }));
	REQUIRE(result.contains({ "1", "10" }));
	REQUIRE(pkb->getNextT(1, 2)== true);
	REQUIRE(pkb->getNextT(1, 3) == true);
	REQUIRE(pkb->getNextT(1, 4) == true);
	REQUIRE(pkb->getNextT(1, 5) == true);
	REQUIRE(pkb->getNextT(1, 6) == true);
	REQUIRE(pkb->getNextT(1, 7) == true);
	REQUIRE(pkb->getNextT(1, 8) == true);
	REQUIRE(pkb->getNextT(1, 9) == true);
	REQUIRE(pkb->getNextT(1, 10) == true);

	REQUIRE(result.contains({ "2", "3" }));
	REQUIRE(result.contains({ "2", "4" }));
	REQUIRE(result.contains({ "2", "5" }));
	REQUIRE(result.contains({ "2", "6" }));
	REQUIRE(result.contains({ "2", "7" }));
	REQUIRE(result.contains({ "2", "8" }));
	REQUIRE(result.contains({ "2", "9" }));
	REQUIRE(result.contains({ "2", "10" }));
	REQUIRE(pkb->getNextT(2, 3) == true);
	REQUIRE(pkb->getNextT(2, 4) == true);
	REQUIRE(pkb->getNextT(2, 5) == true);
	REQUIRE(pkb->getNextT(2, 6) == true);
	REQUIRE(pkb->getNextT(2, 7) == true);
	REQUIRE(pkb->getNextT(2, 8) == true);
	REQUIRE(pkb->getNextT(2, 9) == true);
	REQUIRE(pkb->getNextT(2, 10) == true);

	REQUIRE(result.contains({ "3", "3" }));
	REQUIRE(result.contains({ "3", "4" }));
	REQUIRE(result.contains({ "3", "5" }));
	REQUIRE(result.contains({ "3", "6" }));
	REQUIRE(result.contains({ "3", "7" }));
	REQUIRE(result.contains({ "3", "8" }));
	REQUIRE(result.contains({ "3", "9" }));
	REQUIRE(result.contains({ "3", "10" }));
	REQUIRE(pkb->getNextT(3, 3) == true);
	REQUIRE(pkb->getNextT(3, 4) == true);
	REQUIRE(pkb->getNextT(3, 5) == true);
	REQUIRE(pkb->getNextT(3, 6) == true);
	REQUIRE(pkb->getNextT(3, 7) == true);
	REQUIRE(pkb->getNextT(3, 8) == true);
	REQUIRE(pkb->getNextT(3, 9) == true);
	REQUIRE(pkb->getNextT(3, 10) == true);

	REQUIRE(result.contains({ "4", "3" }));
	REQUIRE(result.contains({ "4", "4" }));
	REQUIRE(result.contains({ "4", "5" }));
	REQUIRE(result.contains({ "4", "6" }));
	REQUIRE(result.contains({ "4", "7" }));
	REQUIRE(result.contains({ "4", "8" }));
	REQUIRE(result.contains({ "4", "9" }));
	REQUIRE(result.contains({ "4", "10" }));
	REQUIRE(pkb->getNextT(4, 3) == true);
	REQUIRE(pkb->getNextT(4, 4) == true);
	REQUIRE(pkb->getNextT(4, 5) == true);
	REQUIRE(pkb->getNextT(4, 6) == true);
	REQUIRE(pkb->getNextT(4, 7) == true);
	REQUIRE(pkb->getNextT(4, 8) == true);
	REQUIRE(pkb->getNextT(4, 9) == true);
	REQUIRE(pkb->getNextT(4, 10) == true);

	REQUIRE(result.contains({ "5", "3" }));
	REQUIRE(result.contains({ "5", "4" }));
	REQUIRE(result.contains({ "5", "5" }));
	REQUIRE(result.contains({ "5", "6" }));
	REQUIRE(result.contains({ "5", "7" }));
	REQUIRE(result.contains({ "5", "8" }));
	REQUIRE(result.contains({ "5", "9" }));
	REQUIRE(result.contains({ "5", "10" }));
	REQUIRE(pkb->getNextT(5, 3) == true);
	REQUIRE(pkb->getNextT(5, 4) == true);
	REQUIRE(pkb->getNextT(5, 5) == true);
	REQUIRE(pkb->getNextT(5, 6) == true);
	REQUIRE(pkb->getNextT(5, 7) == true);
	REQUIRE(pkb->getNextT(5, 8) == true);
	REQUIRE(pkb->getNextT(5, 9) == true);
	REQUIRE(pkb->getNextT(5, 10) == true);

	REQUIRE(result.contains({ "6", "3" }));
	REQUIRE(result.contains({ "6", "4" }));
	REQUIRE(result.contains({ "6", "5" }));
	REQUIRE(result.contains({ "6", "6" }));
	REQUIRE(result.contains({ "6", "7" }));
	REQUIRE(result.contains({ "6", "8" }));
	REQUIRE(result.contains({ "6", "9" }));
	REQUIRE(result.contains({ "6", "10" }));
	REQUIRE(pkb->getNextT(6, 3) == true);
	REQUIRE(pkb->getNextT(6, 4) == true);
	REQUIRE(pkb->getNextT(6, 5) == true);
	REQUIRE(pkb->getNextT(6, 6) == true);
	REQUIRE(pkb->getNextT(6, 7) == true);
	REQUIRE(pkb->getNextT(6, 8) == true);
	REQUIRE(pkb->getNextT(6, 9) == true);
	REQUIRE(pkb->getNextT(6, 10) == true);

	REQUIRE(result.contains({ "7", "3" }));
	REQUIRE(result.contains({ "7", "4" }));
	REQUIRE(result.contains({ "7", "5" }));
	REQUIRE(result.contains({ "7", "6" }));
	REQUIRE(result.contains({ "7", "7" }));
	REQUIRE(result.contains({ "7", "8" }));
	REQUIRE(result.contains({ "7", "9" }));
	REQUIRE(result.contains({ "7", "10" }));
	REQUIRE(pkb->getNextT(7, 3) == true);
	REQUIRE(pkb->getNextT(7, 4) == true);
	REQUIRE(pkb->getNextT(7, 5) == true);
	REQUIRE(pkb->getNextT(7, 6) == true);
	REQUIRE(pkb->getNextT(7, 7) == true);
	REQUIRE(pkb->getNextT(7, 8) == true);
	REQUIRE(pkb->getNextT(7, 9) == true);
	REQUIRE(pkb->getNextT(7, 10) == true);

	REQUIRE(result.contains({ "8", "3" }));
	REQUIRE(result.contains({ "8", "4" }));
	REQUIRE(result.contains({ "8", "5" }));
	REQUIRE(result.contains({ "8", "6" }));
	REQUIRE(result.contains({ "8", "7" }));
	REQUIRE(result.contains({ "8", "8" }));
	REQUIRE(result.contains({ "8", "9" }));
	REQUIRE(result.contains({ "8", "10" }));
	REQUIRE(pkb->getNextT(8, 3) == true);
	REQUIRE(pkb->getNextT(8, 4) == true);
	REQUIRE(pkb->getNextT(8, 5) == true);
	REQUIRE(pkb->getNextT(8, 6) == true);
	REQUIRE(pkb->getNextT(8, 7) == true);
	REQUIRE(pkb->getNextT(8, 8) == true);
	REQUIRE(pkb->getNextT(8, 9) == true);
	REQUIRE(pkb->getNextT(8, 10) == true);

	REQUIRE(result.contains({ "9", "10" }));
	REQUIRE(pkb->getNextT(9, 10) == true);

	//reverse relations
	result = pkb->getNextT(1, false);
	REQUIRE(result.size() == 0);

	result = pkb->getNextT(2, false);
	REQUIRE(result.size() == 1);
	REQUIRE(result.contains({ "1" }));

	result = pkb->getNextT(3, false);
	REQUIRE(result.size() == 8);
	REQUIRE(result.contains({ "1" }));
	REQUIRE(result.contains({ "2" }));
	REQUIRE(result.contains({ "3" }));
	REQUIRE(result.contains({ "4" }));
	REQUIRE(result.contains({ "5" }));
	REQUIRE(result.contains({ "6" }));
	REQUIRE(result.contains({ "7" }));
	REQUIRE(result.contains({ "8" }));

	result = pkb->getNextT(4, false);
	REQUIRE(result.size() == 8);
	REQUIRE(result.contains({ "1" }));
	REQUIRE(result.contains({ "2" }));
	REQUIRE(result.contains({ "3" }));
	REQUIRE(result.contains({ "4" }));
	REQUIRE(result.contains({ "5" }));
	REQUIRE(result.contains({ "6" }));
	REQUIRE(result.contains({ "7" }));
	REQUIRE(result.contains({ "8" }));

	result = pkb->getNextT(5, false);
	REQUIRE(result.size() == 8);
	REQUIRE(result.contains({ "1" }));
	REQUIRE(result.contains({ "2" }));
	REQUIRE(result.contains({ "3" }));
	REQUIRE(result.contains({ "4" }));
	REQUIRE(result.contains({ "5" }));
	REQUIRE(result.contains({ "6" }));
	REQUIRE(result.contains({ "7" }));
	REQUIRE(result.contains({ "8" }));

	result = pkb->getNextT(6, false);
	REQUIRE(result.size() == 8);
	REQUIRE(result.contains({ "1" }));
	REQUIRE(result.contains({ "2" }));
	REQUIRE(result.contains({ "3" }));
	REQUIRE(result.contains({ "4" }));
	REQUIRE(result.contains({ "5" }));
	REQUIRE(result.contains({ "6" }));
	REQUIRE(result.contains({ "7" }));
	REQUIRE(result.contains({ "8" }));

	result = pkb->getNextT(7, false);
	REQUIRE(result.size() == 8);
	REQUIRE(result.contains({ "1" }));
	REQUIRE(result.contains({ "2" }));
	REQUIRE(result.contains({ "3" }));
	REQUIRE(result.contains({ "4" }));
	REQUIRE(result.contains({ "5" }));
	REQUIRE(result.contains({ "6" }));
	REQUIRE(result.contains({ "7" }));
	REQUIRE(result.contains({ "8" }));

	result = pkb->getNextT(8, false);
	REQUIRE(result.size() == 8);
	REQUIRE(result.contains({ "1" }));
	REQUIRE(result.contains({ "2" }));
	REQUIRE(result.contains({ "3" }));
	REQUIRE(result.contains({ "4" }));
	REQUIRE(result.contains({ "5" }));
	REQUIRE(result.contains({ "6" }));
	REQUIRE(result.contains({ "7" }));
	REQUIRE(result.contains({ "8" }));

	result = pkb->getNextT(9, false);
	REQUIRE(result.size() == 8);
	REQUIRE(result.contains({ "1" }));
	REQUIRE(result.contains({ "2" }));
	REQUIRE(result.contains({ "3" }));
	REQUIRE(result.contains({ "4" }));
	REQUIRE(result.contains({ "5" }));
	REQUIRE(result.contains({ "6" }));
	REQUIRE(result.contains({ "7" }));
	REQUIRE(result.contains({ "8" }));

	result = pkb->getNextT(10, false);
	REQUIRE(result.size() == 9);
	REQUIRE(result.contains({ "1" }));
	REQUIRE(result.contains({ "2" }));
	REQUIRE(result.contains({ "3" }));
	REQUIRE(result.contains({ "4" }));
	REQUIRE(result.contains({ "5" }));
	REQUIRE(result.contains({ "6" }));
	REQUIRE(result.contains({ "7" }));
	REQUIRE(result.contains({ "8" }));
	REQUIRE(result.contains({ "9" }));

}

TEST_CASE("No nesting in program with two procedures") {
	std::unique_ptr<PKB> pkb{ new PKB() };
	pkb->setProc("A", 1, 4);
	pkb->setProc("B", 4, 8);
	pkb->setNext(1, 2);
	pkb->setNext(2, 3);
	pkb->setNext(4, 5);
	pkb->setNext(5, 6);
	pkb->setNext(6, 7);
	DesignExtractor::populateDesigns(pkb);
	Table result = pkb->getNextT();

	//forward relations
	REQUIRE(result.size() == 9);

	//procedure A
	REQUIRE(result.contains({ "1", "2" }));
	REQUIRE(result.contains({ "1", "3" }));
	REQUIRE(pkb->getNextT(1, 2) == true);
	REQUIRE(pkb->getNextT(1, 3) == true);

	REQUIRE(result.contains({ "2", "3" }));
	REQUIRE(pkb->getNextT(2, 3) == true);

	//check no nextT between procedures
	REQUIRE(pkb->getNextT(3, 4) == false);

	//procedure B
	REQUIRE(result.contains({ "4", "5" }));
	REQUIRE(result.contains({ "4", "6" }));
	REQUIRE(result.contains({ "4", "7" }));
	REQUIRE(pkb->getNextT(4, 5) == true);
	REQUIRE(pkb->getNextT(4, 6) == true);
	REQUIRE(pkb->getNextT(4, 7) == true);

	REQUIRE(result.contains({ "5", "6" }));
	REQUIRE(result.contains({ "5", "7" }));
	REQUIRE(pkb->getNextT(5, 6) == true);
	REQUIRE(pkb->getNextT(5, 7) == true);

	REQUIRE(result.contains({ "6", "7" }));
	REQUIRE(pkb->getNextT(6, 7) == true);

	//reverse relations
	result = pkb->getNextT(1, false);
	REQUIRE(result.size() == 0);
	
	result = pkb->getNextT(2, false);
	REQUIRE(result.size() == 1);
	REQUIRE(result.contains({ "1" }));

	result = pkb->getNextT(3, false);
	REQUIRE(result.size() == 2);
	REQUIRE(result.contains({ "1" }));
	REQUIRE(result.contains({ "2" }));

	result = pkb->getNextT(4, false);
	REQUIRE(result.size() == 0);

	result = pkb->getNextT(5, false);
	REQUIRE(result.size() == 1);
	REQUIRE(result.contains({ "4" }));

	result = pkb->getNextT(6, false);
	REQUIRE(result.size() == 2);
	REQUIRE(result.contains({ "4" }));
	REQUIRE(result.contains({ "5" }));

	result = pkb->getNextT(7, false);
	REQUIRE(result.size() == 3);
	REQUIRE(result.contains({ "4" }));
	REQUIRE(result.contains({ "5" }));
	REQUIRE(result.contains({ "6" }));
}

TEST_CASE("One complex nesting in one procedure in program with two procedures") {
	std::unique_ptr<PKB> pkb{ new PKB() };
	pkb->setProc("A", 1, 4);
	pkb->setProc("B", 4, 23);
	pkb->setNext(1, 2);
	pkb->setNext(2, 3);

	pkb->setNext(4, 5);
	pkb->setNext(5, 6);
	pkb->setNext(5, 7);
	pkb->setNext(6, 8);
	pkb->setNext(7, 8);
	pkb->setNext(8, 9);
	pkb->setNext(9, 8);
	pkb->setNext(8, 10);
	pkb->setNext(10, 11);
	pkb->setNext(10, 13);
	pkb->setNext(11, 12);
	pkb->setNext(12, 11);
	pkb->setNext(13, 14);
	pkb->setNext(11, 14);
	pkb->setNext(14, 15);
	pkb->setNext(14, 16);
	pkb->setNext(16, 17);
	pkb->setNext(17, 16);
	pkb->setNext(15, 18);
	pkb->setNext(16, 18);
	pkb->setNext(18, 19);
	pkb->setNext(19, 20);
	pkb->setNext(19, 21);
	pkb->setNext(20, 18);
	pkb->setNext(21, 18);
	pkb->setNext(18, 22);

	pkb->setStmtType(5, StatementType::If);
	pkb->setStmtType(8, StatementType::While);
	pkb->setStmtType(10, StatementType::If);
	pkb->setStmtType(11, StatementType::While);
	pkb->setStmtType(14, StatementType::If);
	pkb->setStmtType(16, StatementType::While);
	pkb->setStmtType(18, StatementType::While);
	pkb->setStmtType(19, StatementType::If);
	DesignExtractor::populateDesigns(pkb);
	Table result = pkb->getNextT();

	//forward relations
	REQUIRE(result.size() == 188);

	//procedure A
	REQUIRE(result.contains({ "1", "2" }));
	REQUIRE(result.contains({ "1", "3" }));
	REQUIRE(pkb->getNextT(1, 2) == true);
	REQUIRE(pkb->getNextT(1, 3) == true);

	REQUIRE(result.contains({ "2", "3" }));
	REQUIRE(pkb->getNextT(2, 3) == true);

	//check that NextT does not cross procedures
	REQUIRE(pkb->getNextT(3, 4) == false);

	//procedure B
	REQUIRE(result.contains({ "4", "5" }));
	REQUIRE(result.contains({ "4", "6" }));
	REQUIRE(result.contains({ "4", "7" }));
	REQUIRE(result.contains({ "4", "8" }));
	REQUIRE(result.contains({ "4", "9" }));
	REQUIRE(result.contains({ "4", "10" }));
	REQUIRE(result.contains({ "4", "11" }));
	REQUIRE(result.contains({ "4", "12" }));
	REQUIRE(result.contains({ "4", "13" }));
	REQUIRE(result.contains({ "4", "14" }));
	REQUIRE(result.contains({ "4", "15" }));
	REQUIRE(result.contains({ "4", "16" }));
	REQUIRE(result.contains({ "4", "17" }));
	REQUIRE(result.contains({ "4", "18" }));
	REQUIRE(result.contains({ "4", "19" }));
	REQUIRE(result.contains({ "4", "20" }));
	REQUIRE(result.contains({ "4", "21" }));
	REQUIRE(result.contains({ "4", "22" }));
	REQUIRE(pkb->getNextT(4, 5) == true);
	REQUIRE(pkb->getNextT(4, 6) == true);
	REQUIRE(pkb->getNextT(4, 7) == true);
	REQUIRE(pkb->getNextT(4, 8) == true);
	REQUIRE(pkb->getNextT(4, 9) == true);
	REQUIRE(pkb->getNextT(4, 10) == true);
	REQUIRE(pkb->getNextT(4, 11) == true);
	REQUIRE(pkb->getNextT(4, 12) == true);
	REQUIRE(pkb->getNextT(4, 13) == true);
	REQUIRE(pkb->getNextT(4, 14) == true);
	REQUIRE(pkb->getNextT(4, 15) == true);
	REQUIRE(pkb->getNextT(4, 16) == true);
	REQUIRE(pkb->getNextT(4, 17) == true);
	REQUIRE(pkb->getNextT(4, 18) == true);
	REQUIRE(pkb->getNextT(4, 19) == true);
	REQUIRE(pkb->getNextT(4, 20) == true);
	REQUIRE(pkb->getNextT(4, 21) == true);
	REQUIRE(pkb->getNextT(4, 22) == true);

	REQUIRE(result.contains({ "5", "6" }));
	REQUIRE(result.contains({ "5", "7" }));
	REQUIRE(result.contains({ "5", "8" }));
	REQUIRE(result.contains({ "5", "9" }));
	REQUIRE(result.contains({ "5", "10" }));
	REQUIRE(result.contains({ "5", "11" }));
	REQUIRE(result.contains({ "5", "12" }));
	REQUIRE(result.contains({ "5", "13" }));
	REQUIRE(result.contains({ "5", "14" }));
	REQUIRE(result.contains({ "5", "15" }));
	REQUIRE(result.contains({ "5", "16" }));
	REQUIRE(result.contains({ "5", "17" }));
	REQUIRE(result.contains({ "5", "18" }));
	REQUIRE(result.contains({ "5", "19" }));
	REQUIRE(result.contains({ "5", "20" }));
	REQUIRE(result.contains({ "5", "21" }));
	REQUIRE(result.contains({ "5", "22" }));
	REQUIRE(pkb->getNextT(5, 6) == true);
	REQUIRE(pkb->getNextT(5, 7) == true);
	REQUIRE(pkb->getNextT(5, 8) == true);
	REQUIRE(pkb->getNextT(5, 9) == true);
	REQUIRE(pkb->getNextT(5, 10) == true);
	REQUIRE(pkb->getNextT(5, 11) == true);
	REQUIRE(pkb->getNextT(5, 12) == true);
	REQUIRE(pkb->getNextT(5, 13) == true);
	REQUIRE(pkb->getNextT(5, 14) == true);
	REQUIRE(pkb->getNextT(5, 15) == true);
	REQUIRE(pkb->getNextT(5, 16) == true);
	REQUIRE(pkb->getNextT(5, 17) == true);
	REQUIRE(pkb->getNextT(5, 18) == true);
	REQUIRE(pkb->getNextT(5, 19) == true);
	REQUIRE(pkb->getNextT(5, 20) == true);
	REQUIRE(pkb->getNextT(5, 21) == true);
	REQUIRE(pkb->getNextT(5, 22) == true);

	REQUIRE(result.contains({ "6", "8" }));
	REQUIRE(result.contains({ "6", "9" }));
	REQUIRE(result.contains({ "6", "10" }));
	REQUIRE(result.contains({ "6", "11" }));
	REQUIRE(result.contains({ "6", "12" }));
	REQUIRE(result.contains({ "6", "13" }));
	REQUIRE(result.contains({ "6", "14" }));
	REQUIRE(result.contains({ "6", "15" }));
	REQUIRE(result.contains({ "6", "16" }));
	REQUIRE(result.contains({ "6", "17" }));
	REQUIRE(result.contains({ "6", "18" }));
	REQUIRE(result.contains({ "6", "19" }));
	REQUIRE(result.contains({ "6", "20" }));
	REQUIRE(result.contains({ "6", "21" }));
	REQUIRE(result.contains({ "6", "22" }));
	REQUIRE(pkb->getNextT(6, 8) == true);
	REQUIRE(pkb->getNextT(6, 9) == true);
	REQUIRE(pkb->getNextT(6, 10) == true);
	REQUIRE(pkb->getNextT(6, 11) == true);
	REQUIRE(pkb->getNextT(6, 12) == true);
	REQUIRE(pkb->getNextT(6, 13) == true);
	REQUIRE(pkb->getNextT(6, 14) == true);
	REQUIRE(pkb->getNextT(6, 15) == true);
	REQUIRE(pkb->getNextT(6, 16) == true);
	REQUIRE(pkb->getNextT(6, 17) == true);
	REQUIRE(pkb->getNextT(6, 18) == true);
	REQUIRE(pkb->getNextT(6, 19) == true);
	REQUIRE(pkb->getNextT(6, 10) == true);
	REQUIRE(pkb->getNextT(6, 21) == true);
	REQUIRE(pkb->getNextT(6, 22) == true);

	REQUIRE(result.contains({ "7", "8" }));
	REQUIRE(result.contains({ "7", "9" }));
	REQUIRE(result.contains({ "7", "10" }));
	REQUIRE(result.contains({ "7", "11" }));
	REQUIRE(result.contains({ "7", "12" }));
	REQUIRE(result.contains({ "7", "13" }));
	REQUIRE(result.contains({ "7", "14" }));
	REQUIRE(result.contains({ "7", "15" }));
	REQUIRE(result.contains({ "7", "16" }));
	REQUIRE(result.contains({ "7", "17" }));
	REQUIRE(result.contains({ "7", "18" }));
	REQUIRE(result.contains({ "7", "19" }));
	REQUIRE(result.contains({ "7", "20" }));
	REQUIRE(result.contains({ "7", "21" }));
	REQUIRE(result.contains({ "7", "22" }));
	REQUIRE(pkb->getNextT(7, 8) == true);
	REQUIRE(pkb->getNextT(7, 9) == true);
	REQUIRE(pkb->getNextT(7, 10) == true);
	REQUIRE(pkb->getNextT(7, 11) == true);
	REQUIRE(pkb->getNextT(7, 12) == true);
	REQUIRE(pkb->getNextT(7, 13) == true);
	REQUIRE(pkb->getNextT(7, 14) == true);
	REQUIRE(pkb->getNextT(7, 15) == true);
	REQUIRE(pkb->getNextT(7, 16) == true);
	REQUIRE(pkb->getNextT(7, 17) == true);
	REQUIRE(pkb->getNextT(7, 18) == true);
	REQUIRE(pkb->getNextT(7, 19) == true);
	REQUIRE(pkb->getNextT(7, 20) == true);
	REQUIRE(pkb->getNextT(7, 21) == true);
	REQUIRE(pkb->getNextT(7, 22) == true);

	REQUIRE(result.contains({ "8", "8" }));
	REQUIRE(result.contains({ "8", "9" }));
	REQUIRE(result.contains({ "8", "10" }));
	REQUIRE(result.contains({ "8", "11" }));
	REQUIRE(result.contains({ "8", "12" }));
	REQUIRE(result.contains({ "8", "13" }));
	REQUIRE(result.contains({ "8", "14" }));
	REQUIRE(result.contains({ "8", "15" }));
	REQUIRE(result.contains({ "8", "16" }));
	REQUIRE(result.contains({ "8", "17" }));
	REQUIRE(result.contains({ "8", "18" }));
	REQUIRE(result.contains({ "8", "19" }));
	REQUIRE(result.contains({ "8", "20" }));
	REQUIRE(result.contains({ "8", "21" }));
	REQUIRE(result.contains({ "8", "22" }));
	REQUIRE(pkb->getNextT(8, 8) == true);
	REQUIRE(pkb->getNextT(8, 9) == true);
	REQUIRE(pkb->getNextT(8, 10) == true);
	REQUIRE(pkb->getNextT(8, 11) == true);
	REQUIRE(pkb->getNextT(8, 12) == true);
	REQUIRE(pkb->getNextT(8, 13) == true);
	REQUIRE(pkb->getNextT(8, 14) == true);
	REQUIRE(pkb->getNextT(8, 15) == true);
	REQUIRE(pkb->getNextT(8, 16) == true);
	REQUIRE(pkb->getNextT(8, 17) == true);
	REQUIRE(pkb->getNextT(8, 18) == true);
	REQUIRE(pkb->getNextT(8, 19) == true);
	REQUIRE(pkb->getNextT(8, 20) == true);
	REQUIRE(pkb->getNextT(8, 21) == true);
	REQUIRE(pkb->getNextT(8, 22) == true);

	REQUIRE(result.contains({ "9", "8" }));
	REQUIRE(result.contains({ "9", "9" }));
	REQUIRE(result.contains({ "9", "10" }));
	REQUIRE(result.contains({ "9", "11" }));
	REQUIRE(result.contains({ "9", "12" }));
	REQUIRE(result.contains({ "9", "13" }));
	REQUIRE(result.contains({ "9", "14" }));
	REQUIRE(result.contains({ "9", "15" }));
	REQUIRE(result.contains({ "9", "16" }));
	REQUIRE(result.contains({ "9", "17" }));
	REQUIRE(result.contains({ "9", "18" }));
	REQUIRE(result.contains({ "9", "19" }));
	REQUIRE(result.contains({ "9", "20" }));
	REQUIRE(result.contains({ "9", "21" }));
	REQUIRE(result.contains({ "9", "22" }));
	REQUIRE(pkb->getNextT(9, 8) == true);
	REQUIRE(pkb->getNextT(9, 9) == true);
	REQUIRE(pkb->getNextT(9, 10) == true);
	REQUIRE(pkb->getNextT(9, 11) == true);
	REQUIRE(pkb->getNextT(9, 12) == true);
	REQUIRE(pkb->getNextT(9, 13) == true);
	REQUIRE(pkb->getNextT(9, 14) == true);
	REQUIRE(pkb->getNextT(9, 15) == true);
	REQUIRE(pkb->getNextT(9, 16) == true);
	REQUIRE(pkb->getNextT(9, 17) == true);
	REQUIRE(pkb->getNextT(9, 18) == true);
	REQUIRE(pkb->getNextT(9, 19) == true);
	REQUIRE(pkb->getNextT(9, 20) == true);
	REQUIRE(pkb->getNextT(9, 21) == true);
	REQUIRE(pkb->getNextT(9, 22) == true);

	REQUIRE(result.contains({ "10", "11" }));
	REQUIRE(result.contains({ "10", "12" }));
	REQUIRE(result.contains({ "10", "13" }));
	REQUIRE(result.contains({ "10", "14" }));
	REQUIRE(result.contains({ "10", "15" }));
	REQUIRE(result.contains({ "10", "16" }));
	REQUIRE(result.contains({ "10", "17" }));
	REQUIRE(result.contains({ "10", "18" }));
	REQUIRE(result.contains({ "10", "19" }));
	REQUIRE(result.contains({ "10", "20" }));
	REQUIRE(result.contains({ "10", "21" }));
	REQUIRE(result.contains({ "10", "22" }));
	REQUIRE(pkb->getNextT(10, 11) == true);
	REQUIRE(pkb->getNextT(10, 12) == true);
	REQUIRE(pkb->getNextT(10, 13) == true);
	REQUIRE(pkb->getNextT(10, 14) == true);
	REQUIRE(pkb->getNextT(10, 15) == true);
	REQUIRE(pkb->getNextT(10, 16) == true);
	REQUIRE(pkb->getNextT(10, 17) == true);
	REQUIRE(pkb->getNextT(10, 18) == true);
	REQUIRE(pkb->getNextT(10, 19) == true);
	REQUIRE(pkb->getNextT(10, 20) == true);
	REQUIRE(pkb->getNextT(10, 21) == true);
	REQUIRE(pkb->getNextT(10, 22) == true);

	REQUIRE(result.contains({ "11", "11" }));
	REQUIRE(result.contains({ "11", "12" }));
	REQUIRE(result.contains({ "11", "14" }));
	REQUIRE(result.contains({ "11", "15" }));
	REQUIRE(result.contains({ "11", "16" }));
	REQUIRE(result.contains({ "11", "17" }));
	REQUIRE(result.contains({ "11", "18" }));
	REQUIRE(result.contains({ "11", "19" }));
	REQUIRE(result.contains({ "11", "20" }));
	REQUIRE(result.contains({ "11", "21" }));
	REQUIRE(result.contains({ "11", "22" }));
	REQUIRE(pkb->getNextT(11, 11) == true);
	REQUIRE(pkb->getNextT(11, 12) == true);
	REQUIRE(pkb->getNextT(11, 14) == true);
	REQUIRE(pkb->getNextT(11, 15) == true);
	REQUIRE(pkb->getNextT(11, 16) == true);
	REQUIRE(pkb->getNextT(11, 17) == true);
	REQUIRE(pkb->getNextT(11, 18) == true);
	REQUIRE(pkb->getNextT(11, 19) == true);
	REQUIRE(pkb->getNextT(11, 20) == true);
	REQUIRE(pkb->getNextT(11, 21) == true);
	REQUIRE(pkb->getNextT(11, 22) == true);

	REQUIRE(result.contains({ "12", "11" }));
	REQUIRE(result.contains({ "12", "12" }));
	REQUIRE(result.contains({ "12", "14" }));
	REQUIRE(result.contains({ "12", "15" }));
	REQUIRE(result.contains({ "12", "16" }));
	REQUIRE(result.contains({ "12", "17" }));
	REQUIRE(result.contains({ "12", "18" }));
	REQUIRE(result.contains({ "12", "19" }));
	REQUIRE(result.contains({ "12", "20" }));
	REQUIRE(result.contains({ "12", "21" }));
	REQUIRE(result.contains({ "12", "22" }));
	REQUIRE(pkb->getNextT(12, 11) == true);
	REQUIRE(pkb->getNextT(12, 12) == true);
	REQUIRE(pkb->getNextT(12, 14) == true);
	REQUIRE(pkb->getNextT(12, 15) == true);
	REQUIRE(pkb->getNextT(12, 16) == true);
	REQUIRE(pkb->getNextT(12, 17) == true);
	REQUIRE(pkb->getNextT(12, 18) == true);
	REQUIRE(pkb->getNextT(12, 19) == true);
	REQUIRE(pkb->getNextT(12, 20) == true);
	REQUIRE(pkb->getNextT(12, 21) == true);
	REQUIRE(pkb->getNextT(12, 22) == true);

	REQUIRE(result.contains({ "13", "14" }));
	REQUIRE(result.contains({ "13", "15" }));
	REQUIRE(result.contains({ "13", "16" }));
	REQUIRE(result.contains({ "13", "17" }));
	REQUIRE(result.contains({ "13", "18" }));
	REQUIRE(result.contains({ "13", "19" }));
	REQUIRE(result.contains({ "13", "20" }));
	REQUIRE(result.contains({ "13", "21" }));
	REQUIRE(result.contains({ "13", "22" }));
	REQUIRE(pkb->getNextT(13, 14) == true);
	REQUIRE(pkb->getNextT(13, 15) == true);
	REQUIRE(pkb->getNextT(13, 16) == true);
	REQUIRE(pkb->getNextT(13, 17) == true);
	REQUIRE(pkb->getNextT(13, 18) == true);
	REQUIRE(pkb->getNextT(13, 19) == true);
	REQUIRE(pkb->getNextT(13, 20) == true);
	REQUIRE(pkb->getNextT(13, 21) == true);
	REQUIRE(pkb->getNextT(13, 22) == true);

	REQUIRE(result.contains({ "14", "15" }));
	REQUIRE(result.contains({ "14", "16" }));
	REQUIRE(result.contains({ "14", "17" }));
	REQUIRE(result.contains({ "14", "18" }));
	REQUIRE(result.contains({ "14", "19" }));
	REQUIRE(result.contains({ "14", "20" }));
	REQUIRE(result.contains({ "14", "21" }));
	REQUIRE(result.contains({ "14", "22" }));
	REQUIRE(pkb->getNextT(14, 15) == true);
	REQUIRE(pkb->getNextT(14, 16) == true);
	REQUIRE(pkb->getNextT(14, 17) == true);
	REQUIRE(pkb->getNextT(14, 18) == true);
	REQUIRE(pkb->getNextT(14, 19) == true);
	REQUIRE(pkb->getNextT(14, 20) == true);
	REQUIRE(pkb->getNextT(14, 21) == true);
	REQUIRE(pkb->getNextT(14, 22) == true);

	REQUIRE(result.contains({ "15", "18" }));
	REQUIRE(result.contains({ "15", "19" }));
	REQUIRE(result.contains({ "15", "20" }));
	REQUIRE(result.contains({ "15", "21" }));
	REQUIRE(result.contains({ "15", "22" }));
	REQUIRE(pkb->getNextT(15, 18) == true);
	REQUIRE(pkb->getNextT(15, 19) == true);
	REQUIRE(pkb->getNextT(15, 20) == true);
	REQUIRE(pkb->getNextT(15, 21) == true);
	REQUIRE(pkb->getNextT(15, 22) == true);

	REQUIRE(result.contains({ "16", "16" }));
	REQUIRE(result.contains({ "16", "17" }));
	REQUIRE(result.contains({ "16", "18" }));
	REQUIRE(result.contains({ "16", "19" }));
	REQUIRE(result.contains({ "16", "20" }));
	REQUIRE(result.contains({ "16", "21" }));
	REQUIRE(result.contains({ "16", "22" }));
	REQUIRE(pkb->getNextT(16, 16) == true);
	REQUIRE(pkb->getNextT(16, 17) == true);
	REQUIRE(pkb->getNextT(16, 18) == true);
	REQUIRE(pkb->getNextT(16, 19) == true);
	REQUIRE(pkb->getNextT(16, 20) == true);
	REQUIRE(pkb->getNextT(16, 21) == true);
	REQUIRE(pkb->getNextT(16, 22) == true);

	REQUIRE(result.contains({ "17", "16" }));
	REQUIRE(result.contains({ "17", "17" }));
	REQUIRE(result.contains({ "17", "18" }));
	REQUIRE(result.contains({ "17", "19" }));
	REQUIRE(result.contains({ "17", "20" }));
	REQUIRE(result.contains({ "17", "21" }));
	REQUIRE(result.contains({ "17", "22" }));
	REQUIRE(pkb->getNextT(17, 16) == true);
	REQUIRE(pkb->getNextT(17, 17) == true);
	REQUIRE(pkb->getNextT(17, 18) == true);
	REQUIRE(pkb->getNextT(17, 19) == true);
	REQUIRE(pkb->getNextT(17, 20) == true);
	REQUIRE(pkb->getNextT(17, 21) == true);
	REQUIRE(pkb->getNextT(17, 22) == true);

	REQUIRE(result.contains({ "18", "18" }));
	REQUIRE(result.contains({ "18", "19" }));
	REQUIRE(result.contains({ "18", "20" }));
	REQUIRE(result.contains({ "18", "21" }));
	REQUIRE(result.contains({ "18", "22" }));
	REQUIRE(pkb->getNextT(18, 18) == true);
	REQUIRE(pkb->getNextT(18, 19) == true);
	REQUIRE(pkb->getNextT(18, 20) == true);
	REQUIRE(pkb->getNextT(18, 21) == true);
	REQUIRE(pkb->getNextT(18, 22) == true);

	REQUIRE(result.contains({ "19", "18" }));
	REQUIRE(result.contains({ "19", "19" }));
	REQUIRE(result.contains({ "19", "20" }));
	REQUIRE(result.contains({ "19", "21" }));
	REQUIRE(result.contains({ "19", "22" }));
	REQUIRE(pkb->getNextT(19, 18) == true);
	REQUIRE(pkb->getNextT(19, 19) == true);
	REQUIRE(pkb->getNextT(19, 20) == true);
	REQUIRE(pkb->getNextT(19, 21) == true);
	REQUIRE(pkb->getNextT(19, 22) == true);

	REQUIRE(result.contains({ "20", "18" }));
	REQUIRE(result.contains({ "20", "19" }));
	REQUIRE(result.contains({ "20", "20" }));
	REQUIRE(result.contains({ "20", "21" }));
	REQUIRE(result.contains({ "20", "22" }));
	REQUIRE(pkb->getNextT(20, 18) == true);
	REQUIRE(pkb->getNextT(20, 19) == true);
	REQUIRE(pkb->getNextT(20, 20) == true);
	REQUIRE(pkb->getNextT(20, 21) == true);
	REQUIRE(pkb->getNextT(20, 22) == true);

	REQUIRE(result.contains({ "21", "18" }));
	REQUIRE(result.contains({ "21", "19" }));
	REQUIRE(result.contains({ "21", "20" }));
	REQUIRE(result.contains({ "21", "21" }));
	REQUIRE(result.contains({ "21", "22" }));
	REQUIRE(pkb->getNextT(21, 18) == true);
	REQUIRE(pkb->getNextT(21, 19) == true);
	REQUIRE(pkb->getNextT(21, 20) == true);
	REQUIRE(pkb->getNextT(21, 21) == true);
	REQUIRE(pkb->getNextT(21, 22) == true);

	//reverse relations
	result = pkb->getNextT(1, false);
	REQUIRE(result.size() == 0);

	result = pkb->getNextT(2, false);
	REQUIRE(result.size() == 1);
	REQUIRE(result.contains({ "1" }));

	result = pkb->getNextT(3, false);
	REQUIRE(result.size() == 2);
	REQUIRE(result.contains({ "1" }));
	REQUIRE(result.contains({ "2" }));

	result = pkb->getNextT(4, false);
	REQUIRE(result.size() == 0);
	
	result = pkb->getNextT(5, false);
	REQUIRE(result.size() == 1);
	REQUIRE(result.contains({ "4" }));

	result = pkb->getNextT(6, false);
	REQUIRE(result.size() == 2);
	REQUIRE(result.contains({ "4" }));
	REQUIRE(result.contains({ "5" }));

	result = pkb->getNextT(7, false);
	REQUIRE(result.size() == 2);
	REQUIRE(result.contains({ "4" }));
	REQUIRE(result.contains({ "5" }));

	result = pkb->getNextT(8, false);
	REQUIRE(result.size() == 6);
	REQUIRE(result.contains({ "4" }));
	REQUIRE(result.contains({ "5" }));
	REQUIRE(result.contains({ "6" }));
	REQUIRE(result.contains({ "7" }));
	REQUIRE(result.contains({ "8" }));
	REQUIRE(result.contains({ "9" }));

	result = pkb->getNextT(9, false);
	REQUIRE(result.size() == 6);
	REQUIRE(result.contains({ "4" }));
	REQUIRE(result.contains({ "5" }));
	REQUIRE(result.contains({ "6" }));
	REQUIRE(result.contains({ "7" }));
	REQUIRE(result.contains({ "8" }));
	REQUIRE(result.contains({ "9" }));

	result = pkb->getNextT(10, false);
	REQUIRE(result.size() == 6);
	REQUIRE(result.contains({ "4" }));
	REQUIRE(result.contains({ "5" }));
	REQUIRE(result.contains({ "6" }));
	REQUIRE(result.contains({ "7" }));
	REQUIRE(result.contains({ "8" }));
	REQUIRE(result.contains({ "9" }));

	result = pkb->getNextT(11, false);
	REQUIRE(result.size() == 9);
	REQUIRE(result.contains({ "4" }));
	REQUIRE(result.contains({ "5" }));
	REQUIRE(result.contains({ "6" }));
	REQUIRE(result.contains({ "7" }));
	REQUIRE(result.contains({ "8" }));
	REQUIRE(result.contains({ "9" }));
	REQUIRE(result.contains({ "10" }));
	REQUIRE(result.contains({ "11" }));
	REQUIRE(result.contains({ "12" }));

	result = pkb->getNextT(12, false);
	REQUIRE(result.size() == 9);
	REQUIRE(result.contains({ "4" }));
	REQUIRE(result.contains({ "5" }));
	REQUIRE(result.contains({ "6" }));
	REQUIRE(result.contains({ "7" }));
	REQUIRE(result.contains({ "8" }));
	REQUIRE(result.contains({ "9" }));
	REQUIRE(result.contains({ "10" }));
	REQUIRE(result.contains({ "11" }));
	REQUIRE(result.contains({ "12" }));

	result = pkb->getNextT(13, false);
	REQUIRE(result.size() == 7);
	REQUIRE(result.contains({ "4" }));
	REQUIRE(result.contains({ "5" }));
	REQUIRE(result.contains({ "6" }));
	REQUIRE(result.contains({ "7" }));
	REQUIRE(result.contains({ "8" }));
	REQUIRE(result.contains({ "9" }));
	REQUIRE(result.contains({ "10" }));

	result = pkb->getNextT(14, false);
	REQUIRE(result.size() == 10);
	REQUIRE(result.contains({ "4" }));
	REQUIRE(result.contains({ "5" }));
	REQUIRE(result.contains({ "6" }));
	REQUIRE(result.contains({ "7" }));
	REQUIRE(result.contains({ "8" }));
	REQUIRE(result.contains({ "9" }));
	REQUIRE(result.contains({ "10" }));
	REQUIRE(result.contains({ "11" }));
	REQUIRE(result.contains({ "12" }));
	REQUIRE(result.contains({ "13" }));

	result = pkb->getNextT(15, false);
	REQUIRE(result.size() == 11);
	REQUIRE(result.contains({ "4" }));
	REQUIRE(result.contains({ "5" }));
	REQUIRE(result.contains({ "6" }));
	REQUIRE(result.contains({ "7" }));
	REQUIRE(result.contains({ "8" }));
	REQUIRE(result.contains({ "9" }));
	REQUIRE(result.contains({ "10" }));
	REQUIRE(result.contains({ "11" }));
	REQUIRE(result.contains({ "12" }));
	REQUIRE(result.contains({ "13" }));
	REQUIRE(result.contains({ "14" }));

	result = pkb->getNextT(16, false);
	REQUIRE(result.size() == 13);
	REQUIRE(result.contains({ "4" }));
	REQUIRE(result.contains({ "5" }));
	REQUIRE(result.contains({ "6" }));
	REQUIRE(result.contains({ "7" }));
	REQUIRE(result.contains({ "8" }));
	REQUIRE(result.contains({ "9" }));
	REQUIRE(result.contains({ "10" }));
	REQUIRE(result.contains({ "11" }));
	REQUIRE(result.contains({ "12" }));
	REQUIRE(result.contains({ "13" }));
	REQUIRE(result.contains({ "14" }));
	REQUIRE(result.contains({ "16" }));
	REQUIRE(result.contains({ "17" }));

	result = pkb->getNextT(17, false);
	REQUIRE(result.size() == 13);
	REQUIRE(result.contains({ "4" }));
	REQUIRE(result.contains({ "5" }));
	REQUIRE(result.contains({ "6" }));
	REQUIRE(result.contains({ "7" }));
	REQUIRE(result.contains({ "8" }));
	REQUIRE(result.contains({ "9" }));
	REQUIRE(result.contains({ "10" }));
	REQUIRE(result.contains({ "11" }));
	REQUIRE(result.contains({ "12" }));
	REQUIRE(result.contains({ "13" }));
	REQUIRE(result.contains({ "14" }));
	REQUIRE(result.contains({ "16" }));
	REQUIRE(result.contains({ "17" }));

	result = pkb->getNextT(18, false);
	REQUIRE(result.size() == 18);
	REQUIRE(result.contains({ "4" }));
	REQUIRE(result.contains({ "5" }));
	REQUIRE(result.contains({ "6" }));
	REQUIRE(result.contains({ "7" }));
	REQUIRE(result.contains({ "8" }));
	REQUIRE(result.contains({ "9" }));
	REQUIRE(result.contains({ "10" }));
	REQUIRE(result.contains({ "11" }));
	REQUIRE(result.contains({ "12" }));
	REQUIRE(result.contains({ "13" }));
	REQUIRE(result.contains({ "14" }));
	REQUIRE(result.contains({ "15" }));
	REQUIRE(result.contains({ "16" }));
	REQUIRE(result.contains({ "17" }));
	REQUIRE(result.contains({ "18" }));
	REQUIRE(result.contains({ "19" }));
	REQUIRE(result.contains({ "20" }));
	REQUIRE(result.contains({ "21" }));

	result = pkb->getNextT(19, false);
	REQUIRE(result.size() == 18);
	REQUIRE(result.contains({ "4" }));
	REQUIRE(result.contains({ "5" }));
	REQUIRE(result.contains({ "6" }));
	REQUIRE(result.contains({ "7" }));
	REQUIRE(result.contains({ "8" }));
	REQUIRE(result.contains({ "9" }));
	REQUIRE(result.contains({ "10" }));
	REQUIRE(result.contains({ "11" }));
	REQUIRE(result.contains({ "12" }));
	REQUIRE(result.contains({ "13" }));
	REQUIRE(result.contains({ "14" }));
	REQUIRE(result.contains({ "15" }));
	REQUIRE(result.contains({ "16" }));
	REQUIRE(result.contains({ "17" }));
	REQUIRE(result.contains({ "18" }));
	REQUIRE(result.contains({ "19" }));
	REQUIRE(result.contains({ "20" }));
	REQUIRE(result.contains({ "21" }));

	result = pkb->getNextT(20, false);
	REQUIRE(result.size() == 18);
	REQUIRE(result.contains({ "4" }));
	REQUIRE(result.contains({ "5" }));
	REQUIRE(result.contains({ "6" }));
	REQUIRE(result.contains({ "7" }));
	REQUIRE(result.contains({ "8" }));
	REQUIRE(result.contains({ "9" }));
	REQUIRE(result.contains({ "10" }));
	REQUIRE(result.contains({ "11" }));
	REQUIRE(result.contains({ "12" }));
	REQUIRE(result.contains({ "13" }));
	REQUIRE(result.contains({ "14" }));
	REQUIRE(result.contains({ "15" }));
	REQUIRE(result.contains({ "16" }));
	REQUIRE(result.contains({ "17" }));
	REQUIRE(result.contains({ "18" }));
	REQUIRE(result.contains({ "19" }));
	REQUIRE(result.contains({ "20" }));
	REQUIRE(result.contains({ "21" }));

	result = pkb->getNextT(21, false);
	REQUIRE(result.size() == 18);
	REQUIRE(result.contains({ "4" }));
	REQUIRE(result.contains({ "5" }));
	REQUIRE(result.contains({ "6" }));
	REQUIRE(result.contains({ "7" }));
	REQUIRE(result.contains({ "8" }));
	REQUIRE(result.contains({ "9" }));
	REQUIRE(result.contains({ "10" }));
	REQUIRE(result.contains({ "11" }));
	REQUIRE(result.contains({ "12" }));
	REQUIRE(result.contains({ "13" }));
	REQUIRE(result.contains({ "14" }));
	REQUIRE(result.contains({ "15" }));
	REQUIRE(result.contains({ "16" }));
	REQUIRE(result.contains({ "17" }));
	REQUIRE(result.contains({ "18" }));
	REQUIRE(result.contains({ "19" }));
	REQUIRE(result.contains({ "20" }));
	REQUIRE(result.contains({ "21" }));

	result = pkb->getNextT(22, false);
	REQUIRE(result.size() == 18);
	REQUIRE(result.contains({ "4" }));
	REQUIRE(result.contains({ "5" }));
	REQUIRE(result.contains({ "6" }));
	REQUIRE(result.contains({ "7" }));
	REQUIRE(result.contains({ "8" }));
	REQUIRE(result.contains({ "9" }));
	REQUIRE(result.contains({ "10" }));
	REQUIRE(result.contains({ "11" }));
	REQUIRE(result.contains({ "12" }));
	REQUIRE(result.contains({ "13" }));
	REQUIRE(result.contains({ "14" }));
	REQUIRE(result.contains({ "15" }));
	REQUIRE(result.contains({ "16" }));
	REQUIRE(result.contains({ "17" }));
	REQUIRE(result.contains({ "18" }));
	REQUIRE(result.contains({ "19" }));
	REQUIRE(result.contains({ "20" }));
	REQUIRE(result.contains({ "21" }));


}

TEST_CASE("Affects 1") {
	std::unique_ptr<PKB> pkb{ new PKB() };
	pkb->setProc("A", 1, 4);
	pkb->setNext(1, 2);
	pkb->setNext(2, 3);
	pkb->setStmtType(1, StatementType::Assign);
	pkb->setStmtType(3, StatementType::Assign);
	pkb->setModifies(1, "a");
	pkb->setUses(3, "a");
	DesignExtractor::populateDesigns(pkb);
	Table result = pkb->getAffects(1, true);

	
	
	REQUIRE(result.contains("3"));
	REQUIRE(pkb->getAffects(1, 3) == true);


	result = pkb->getAffects(3, false);
	REQUIRE(result.contains("1"));

	result = pkb->getAffects();
	REQUIRE(result.contains({ "1", "3" }));

}


