#include "DesignExtractor.h"
#include "PKB.h"
#include "catch.hpp"

TEST_CASE("One call from procedure with no nesting") {
	std::unique_ptr<PKB> pkb{ new PKB() };
	pkb->setProc("A", 1, 5);
	pkb->setProc("B", 5, 8);
	pkb->setNext(1, 2);
	pkb->setNext(2, 3);
	pkb->setNext(3, 4);
	pkb->setNext(5, 6);
	pkb->setNext(6, 7);
	pkb->setStmtType(2, StatementType::Call);
	pkb->setCallProcName(2, "B");
	pkb->setProcExitStmt("A", std::vector<int> {4});
	pkb->setProcExitStmt("B", std::vector<int> {7});
	DesignExtractor::populateDesigns(pkb);

	//testing nextBip
	Table result = pkb->getNextBip();
	REQUIRE(result.size() == 6);
	REQUIRE(result.contains({"1", "2"}));
	REQUIRE(result.contains({"2", "5"}));
	REQUIRE(result.contains({"5", "6"}));
	REQUIRE(result.contains({"6", "7"}));
	REQUIRE(result.contains({"7", "3"}));
	REQUIRE(result.contains({"3", "4"}));

	//testing nextBipT
	result = pkb->getNextBipT();
	REQUIRE(result.size() == 21);
	REQUIRE(result.contains({ "1", "2" }));
	REQUIRE(result.contains({ "1", "3" }));
	REQUIRE(result.contains({ "1", "4" }));
	REQUIRE(result.contains({ "1", "5" }));
	REQUIRE(result.contains({ "1", "6" }));
	REQUIRE(result.contains({ "1", "7" }));
	REQUIRE(result.contains({ "2", "3" }));
	REQUIRE(result.contains({ "2", "4" }));
	REQUIRE(result.contains({ "2", "5" }));
	REQUIRE(result.contains({ "2", "6" }));
	REQUIRE(result.contains({ "2", "7" }));
	REQUIRE(result.contains({ "3", "4" }));
	REQUIRE(result.contains({ "5", "6" }));
	REQUIRE(result.contains({ "5", "7" }));
	REQUIRE(result.contains({ "5", "3" }));
	REQUIRE(result.contains({ "5", "4" }));
	REQUIRE(result.contains({ "6", "7" }));
	REQUIRE(result.contains({ "6", "3" }));
	REQUIRE(result.contains({ "6", "4" }));
	REQUIRE(result.contains({ "7", "3" }));
	REQUIRE(result.contains({ "7", "4" }));

}

TEST_CASE("One call from procedure inside while loop") {
	std::unique_ptr<PKB> pkb{ new PKB() };
	pkb->setProc("A", 1, 5);
	pkb->setProc("B", 5, 7);
	pkb->setNext(1, 2);
	pkb->setNext(2, 3);
	pkb->setNext(3, 2);
	pkb->setNext(2, 4);
	pkb->setNext(5, 6);
	pkb->setStmtType(2, StatementType::While);
	pkb->setStmtType(3, StatementType::Call);
	pkb->setCallProcName(3, "B");
	pkb->setProcExitStmt("A", std::vector<int> {4});
	pkb->setProcExitStmt("B", std::vector<int> {6});
	DesignExtractor::populateDesigns(pkb);

	//testing nextBip
	Table result = pkb->getNextBip();
	REQUIRE(result.size() == 6);
	REQUIRE(result.contains({ "1", "2" }));
	REQUIRE(result.contains({ "2", "3" }));
	REQUIRE(result.contains({ "3", "5" }));
	REQUIRE(result.contains({ "5", "6" }));
	REQUIRE(result.contains({ "6", "2" }));
	REQUIRE(result.contains({ "2", "4" }));

	//testing nextBipT

	result = pkb->getNextBipT();
	REQUIRE(result.size() == 25);
	REQUIRE(result.contains({ "1", "2" }));
	REQUIRE(result.contains({ "1", "3" }));
	REQUIRE(result.contains({ "1", "4" }));
	REQUIRE(result.contains({ "1", "5" }));
	REQUIRE(result.contains({ "1", "6" }));
	REQUIRE(result.contains({ "2", "2" }));
	REQUIRE(result.contains({ "2", "3" }));
	REQUIRE(result.contains({ "2", "4" }));
	REQUIRE(result.contains({ "2", "5" }));
	REQUIRE(result.contains({ "2", "6" }));
	REQUIRE(result.contains({ "3", "2" }));
	REQUIRE(result.contains({ "3", "3" }));
	REQUIRE(result.contains({ "3", "4" }));
	REQUIRE(result.contains({ "3", "5" }));
	REQUIRE(result.contains({ "3", "6" }));
	REQUIRE(result.contains({ "5", "2" }));
	REQUIRE(result.contains({ "5", "3" }));
	REQUIRE(result.contains({ "5", "4" }));
	REQUIRE(result.contains({ "5", "5" }));
	REQUIRE(result.contains({ "5", "6" }));
	REQUIRE(result.contains({ "6", "2" }));
	REQUIRE(result.contains({ "6", "3" }));
	REQUIRE(result.contains({ "6", "4" }));
	REQUIRE(result.contains({ "6", "5" }));
	REQUIRE(result.contains({ "6", "6" }));

}

TEST_CASE("One call from procedure inside if statement") {
	std::unique_ptr<PKB> pkb{ new PKB() };
	pkb->setProc("A", 1, 6);
	pkb->setProc("B", 6, 8);
	pkb->setNext(1, 2);
	pkb->setNext(2, 3);
	pkb->setNext(2, 4);
	pkb->setNext(3, 5);
	pkb->setNext(4, 5);
	pkb->setNext(6, 7);
	pkb->setStmtType(2, StatementType::If);
	pkb->setStmtType(3, StatementType::Call);
	pkb->setCallProcName(3, "B");
	pkb->setProcExitStmt("A", std::vector<int> {5});
	pkb->setProcExitStmt("B", std::vector<int> {7});
	DesignExtractor::populateDesigns(pkb);

	//testing nextBip
	Table result = pkb->getNextBip();
	REQUIRE(result.size() == 7);
	REQUIRE(result.contains({ "1", "2" }));
	REQUIRE(result.contains({ "2", "3" }));
	REQUIRE(result.contains({ "2", "4" }));
	REQUIRE(result.contains({ "3", "6" }));
	REQUIRE(result.contains({ "6", "7" }));
	REQUIRE(result.contains({ "7", "5" }));
	REQUIRE(result.contains({ "4", "5" }));

	//testing nextBipT
	result = pkb->getNextBipT();
	REQUIRE(result.size() == 18);
	REQUIRE(result.contains({ "1", "2" }));
	REQUIRE(result.contains({ "1", "3" }));
	REQUIRE(result.contains({ "1", "4" }));
	REQUIRE(result.contains({ "1", "5" }));
	REQUIRE(result.contains({ "1", "6" }));
	REQUIRE(result.contains({ "1", "7" }));
	REQUIRE(result.contains({ "2", "3" }));
	REQUIRE(result.contains({ "2", "4" }));
	REQUIRE(result.contains({ "2", "5" }));
	REQUIRE(result.contains({ "2", "6" }));
	REQUIRE(result.contains({ "2", "7" }));
	REQUIRE(result.contains({ "3", "5" }));
	REQUIRE(result.contains({ "3", "6" }));
	REQUIRE(result.contains({ "3", "7" }));
	REQUIRE(result.contains({ "4", "5" }));
	REQUIRE(result.contains({ "6", "5" }));
	REQUIRE(result.contains({ "6", "5" }));
	REQUIRE(result.contains({ "7", "5" }));

}

TEST_CASE("One call from last statement in procedure") {
	std::unique_ptr<PKB> pkb{ new PKB() };
	pkb->setProc("A", 1, 4);
	pkb->setProc("B", 4, 6);
	pkb->setNext(1, 2);
	pkb->setNext(2, 3);
	pkb->setNext(4, 5);
	pkb->setStmtType(3, StatementType::Call);
	pkb->setCallProcName(3, "B");
	pkb->setProcExitStmt("A", std::vector<int> {3});
	pkb->setProcExitStmt("B", std::vector<int> {5});
	DesignExtractor::populateDesigns(pkb);

	//testing nextBip
	Table result = pkb->getNextBip();
	REQUIRE(result.size() == 4);
	REQUIRE(result.contains({ "1", "2" }));
	REQUIRE(result.contains({ "2", "3" }));
	REQUIRE(result.contains({ "3", "4" }));
	REQUIRE(result.contains({ "4", "5" }));

	//testing nextBipT

	result = pkb->getNextBipT();
	REQUIRE(result.size() == 10);
	REQUIRE(result.contains({ "1", "2" }));
	REQUIRE(result.contains({ "1", "3" }));
	REQUIRE(result.contains({ "1", "4" }));
	REQUIRE(result.contains({ "1", "5" }));
	REQUIRE(result.contains({ "2", "3" }));
	REQUIRE(result.contains({ "2", "4" }));
	REQUIRE(result.contains({ "2", "5" }));
	REQUIRE(result.contains({ "3", "4" }));
	REQUIRE(result.contains({ "3", "5" }));
	REQUIRE(result.contains({ "4", "5" }));

}

TEST_CASE("Two consecutive calls with no nesting") {
	std::unique_ptr<PKB> pkb{ new PKB() };
	pkb->setProc("A", 1, 5);
	pkb->setProc("B", 5, 7);
	pkb->setProc("C", 7, 9);
	pkb->setNext(1, 2);
	pkb->setNext(2, 3);
	pkb->setNext(3, 4);
	pkb->setNext(5, 6);
	pkb->setNext(7, 8);
	pkb->setStmtType(2, StatementType::Call);
	pkb->setStmtType(3, StatementType::Call);
	pkb->setProcExitStmt("A", std::vector<int> {4});
	pkb->setProcExitStmt("B", std::vector<int> {6});
	pkb->setProcExitStmt("C", std::vector<int> {8});

	WHEN("consecutively calling the same method") {
		pkb->setCallProcName(2, "B");
		pkb->setCallProcName(3, "B");
		DesignExtractor::populateDesigns(pkb);

		//testing nextBip
		Table result = pkb->getNextBip();
		REQUIRE(result.size() == 7);
		REQUIRE(result.contains({ "1", "2" }));
		REQUIRE(result.contains({ "2", "5" }));
		REQUIRE(result.contains({ "5", "6" }));
		REQUIRE(result.contains({ "6", "3" }));
		REQUIRE(result.contains({ "3", "5" }));
		REQUIRE(result.contains({ "6", "4" }));
		REQUIRE(result.contains({ "7", "8" }));

		//testing nextBipT

		result = pkb->getNextBipT();
		REQUIRE(result.size() == 21);
		REQUIRE(result.contains({ "1", "2" }));
		REQUIRE(result.contains({ "1", "3" }));
		REQUIRE(result.contains({ "1", "4" }));
		REQUIRE(result.contains({ "1", "5" }));
		REQUIRE(result.contains({ "1", "6" }));
		REQUIRE(result.contains({ "2", "3" }));
		REQUIRE(result.contains({ "2", "4" }));
		REQUIRE(result.contains({ "2", "5" }));
		REQUIRE(result.contains({ "2", "6" }));
		REQUIRE(result.contains({ "3", "4" }));
		REQUIRE(result.contains({ "3", "5" }));
		REQUIRE(result.contains({ "3", "6" }));
		REQUIRE(result.contains({ "5", "3" }));
		REQUIRE(result.contains({ "5", "4" }));
		REQUIRE(result.contains({ "5", "5" }));
		REQUIRE(result.contains({ "5", "6" }));
		REQUIRE(result.contains({ "6", "3" }));
		REQUIRE(result.contains({ "6", "4" }));
		REQUIRE(result.contains({ "6", "5" }));
		REQUIRE(result.contains({ "6", "6" }));
		REQUIRE(result.contains({ "7", "8" }));

	}

	WHEN("consecutively calling two different methods") {
		pkb->setCallProcName(2, "B");
		pkb->setCallProcName(3, "C");

		DesignExtractor::populateDesigns(pkb);

		//testing nextBip
		Table result = pkb->getNextBip();
		REQUIRE(result.size() == 7);
		REQUIRE(result.contains({ "1", "2" }));
		REQUIRE(result.contains({ "2", "5" }));
		REQUIRE(result.contains({ "5", "6" }));
		REQUIRE(result.contains({ "6", "3" }));
		REQUIRE(result.contains({ "3", "7" }));
		REQUIRE(result.contains({ "7", "8" }));
		REQUIRE(result.contains({ "8", "4" }));

		//testing nextBipT

		result = pkb->getNextBipT();
		REQUIRE(result.size() == 28);
		REQUIRE(result.contains({ "1", "2" }));
		REQUIRE(result.contains({ "1", "3" }));
		REQUIRE(result.contains({ "1", "4" }));
		REQUIRE(result.contains({ "1", "5" }));
		REQUIRE(result.contains({ "1", "6" }));
		REQUIRE(result.contains({ "1", "7" }));
		REQUIRE(result.contains({ "1", "8" }));
		REQUIRE(result.contains({ "2", "3" }));
		REQUIRE(result.contains({ "2", "4" }));
		REQUIRE(result.contains({ "2", "5" }));
		REQUIRE(result.contains({ "2", "6" }));
		REQUIRE(result.contains({ "2", "7" }));
		REQUIRE(result.contains({ "2", "8" }));
		REQUIRE(result.contains({ "3", "4" }));
		REQUIRE(result.contains({ "3", "7" }));
		REQUIRE(result.contains({ "3", "8" }));
		REQUIRE(result.contains({ "5", "3" }));
		REQUIRE(result.contains({ "5", "4" }));
		REQUIRE(result.contains({ "5", "6" }));
		REQUIRE(result.contains({ "5", "7" }));
		REQUIRE(result.contains({ "5", "8" }));
		REQUIRE(result.contains({ "6", "3" }));
		REQUIRE(result.contains({ "6", "4" }));
		REQUIRE(result.contains({ "6", "7" }));
		REQUIRE(result.contains({ "6", "8" }));
		REQUIRE(result.contains({ "7", "4" }));
		REQUIRE(result.contains({ "7", "8" }));
		REQUIRE(result.contains({ "8", "4" }));

	}

}

TEST_CASE("One call in if statement list and one call in else statement list") {
	std::unique_ptr<PKB> pkb{ new PKB() };
	pkb->setProc("A", 1, 5);
	pkb->setProc("B", 5, 7);
	pkb->setProc("C", 7, 9);
	pkb->setNext(1, 2);
	pkb->setNext(1, 3);
	pkb->setNext(2, 4);
	pkb->setNext(3, 4);
	pkb->setNext(5, 6);
	pkb->setNext(7, 8);
	pkb->setStmtType(1, StatementType::If);
	pkb->setStmtType(2, StatementType::Call);
	pkb->setStmtType(3, StatementType::Call);
	pkb->setProcExitStmt("A", std::vector<int> {4});
	pkb->setProcExitStmt("B", std::vector<int> {6});
	pkb->setProcExitStmt("C", std::vector<int> {8});

	WHEN("both calls same method") {
		pkb->setCallProcName(2, "B");
		pkb->setCallProcName(3, "B");

		DesignExtractor::populateDesigns(pkb);

		//testing nextBip
		Table result = pkb->getNextBip();
		REQUIRE(result.size() == 7);
		REQUIRE(result.contains({ "1", "2" }));
		REQUIRE(result.contains({ "1", "3" }));
		REQUIRE(result.contains({ "2", "5" }));
		REQUIRE(result.contains({ "5", "6" }));
		REQUIRE(result.contains({ "6", "4" }));
		REQUIRE(result.contains({ "3", "5" }));
		REQUIRE(result.contains({ "7", "8" }));

		//testing nextBipT
		result = pkb->getNextBipT();
		REQUIRE(result.size() == 15);
		REQUIRE(result.contains({ "1", "2" }));
		REQUIRE(result.contains({ "1", "3" }));
		REQUIRE(result.contains({ "1", "4" }));
		REQUIRE(result.contains({ "1", "5" }));
		REQUIRE(result.contains({ "1", "6" }));
		REQUIRE(result.contains({ "2", "4" }));
		REQUIRE(result.contains({ "2", "5" }));
		REQUIRE(result.contains({ "2", "6" }));
		REQUIRE(result.contains({ "3", "4" }));
		REQUIRE(result.contains({ "3", "5" }));
		REQUIRE(result.contains({ "3", "6" }));
		REQUIRE(result.contains({ "5", "4" }));
		REQUIRE(result.contains({ "5", "6" }));
		REQUIRE(result.contains({ "6", "4" }));
		REQUIRE(result.contains({ "7", "8" }));

	}

	WHEN("both calls different method") {
		pkb->setCallProcName(2, "B");
		pkb->setCallProcName(3, "C");

		DesignExtractor::populateDesigns(pkb);

		//testing nextBip
		Table result = pkb->getNextBip();
		REQUIRE(result.size() == 8);
		REQUIRE(result.contains({ "1", "2" }));
		REQUIRE(result.contains({ "1", "3" }));
		REQUIRE(result.contains({ "2", "5" }));
		REQUIRE(result.contains({ "5", "6" }));
		REQUIRE(result.contains({ "6", "4" }));
		REQUIRE(result.contains({ "3", "7" }));
		REQUIRE(result.contains({ "7", "8" }));
		REQUIRE(result.contains({ "8", "4" }));

		//testing nextBipT

		result = pkb->getNextBipT();
		REQUIRE(result.size() == 19);
		REQUIRE(result.contains({ "1", "2" }));
		REQUIRE(result.contains({ "1", "3" }));
		REQUIRE(result.contains({ "1", "4" }));
		REQUIRE(result.contains({ "1", "5" }));
		REQUIRE(result.contains({ "1", "6" }));
		REQUIRE(result.contains({ "1", "7" }));
		REQUIRE(result.contains({ "1", "8" }));
		REQUIRE(result.contains({ "2", "4" }));
		REQUIRE(result.contains({ "2", "5" }));
		REQUIRE(result.contains({ "2", "6" }));
		REQUIRE(result.contains({ "3", "4" }));
		REQUIRE(result.contains({ "3", "7" }));
		REQUIRE(result.contains({ "3", "8" }));
		REQUIRE(result.contains({ "5", "4" }));
		REQUIRE(result.contains({ "5", "6" }));
		REQUIRE(result.contains({ "6", "4" }));
		REQUIRE(result.contains({ "7", "4" }));
		REQUIRE(result.contains({ "7", "8" }));
		REQUIRE(result.contains({ "8", "4" }));

	}
}

TEST_CASE("Call to procedure ending with while loop") {
	std::unique_ptr<PKB> pkb{ new PKB() };
	pkb->setProc("A", 1, 4);
	pkb->setProc("B", 4, 6);
	pkb->setNext(1, 2);
	pkb->setNext(2, 3);
	pkb->setNext(4, 5);
	pkb->setNext(5, 4);
	pkb->setStmtType(2, StatementType::Call);
	pkb->setCallProcName(2, "B");
	pkb->setStmtType(4, StatementType::While);
	pkb->setProcExitStmt("A", std::vector<int> {3});
	pkb->setProcExitStmt("B", std::vector<int> {4});

	DesignExtractor::populateDesigns(pkb);

	//testing nextBip
	Table result = pkb->getNextBip();
	REQUIRE(result.size() == 5);
	REQUIRE(result.contains({ "1", "2" }));
	REQUIRE(result.contains({ "2", "4" }));
	REQUIRE(result.contains({ "4", "5" }));
	REQUIRE(result.contains({ "5", "4" }));
	REQUIRE(result.contains({ "4", "3" }));

	//testing nextBipT

	result = pkb->getNextBipT();
	REQUIRE(result.size() == 13);
	REQUIRE(result.contains({ "1", "2" }));
	REQUIRE(result.contains({ "1", "3" }));
	REQUIRE(result.contains({ "1", "4" }));
	REQUIRE(result.contains({ "1", "5" }));
	REQUIRE(result.contains({ "2", "3" }));
	REQUIRE(result.contains({ "2", "4" }));
	REQUIRE(result.contains({ "2", "5" }));
	REQUIRE(result.contains({ "4", "3" }));
	REQUIRE(result.contains({ "4", "4" }));
	REQUIRE(result.contains({ "4", "5" }));
	REQUIRE(result.contains({ "5", "3" }));
	REQUIRE(result.contains({ "5", "4" }));
	REQUIRE(result.contains({ "5", "5" }));

}

TEST_CASE("Call to procedure ending with if statement") {
	std::unique_ptr<PKB> pkb{ new PKB() };
	pkb->setProc("A", 1, 4);
	pkb->setProc("B", 4, 7);
	pkb->setNext(1, 2);
	pkb->setNext(2, 3);
	pkb->setNext(4, 5);
	pkb->setNext(4, 6);
	pkb->setStmtType(2, StatementType::Call);
	pkb->setCallProcName(2, "B");
	pkb->setStmtType(4, StatementType::If);
	pkb->setProcExitStmt("A", std::vector<int> {3});
	pkb->setProcExitStmt("B", std::vector<int> {5, 6});

	DesignExtractor::populateDesigns(pkb);

	//testing nextBip
	Table result = pkb->getNextBip();
	REQUIRE(result.size() == 6);
	REQUIRE(result.contains({ "1", "2" }));
	REQUIRE(result.contains({ "2", "4" }));
	REQUIRE(result.contains({ "4", "5" }));
	REQUIRE(result.contains({ "4", "6" }));
	REQUIRE(result.contains({ "5", "3" }));
	REQUIRE(result.contains({ "6", "3" }));

	//testing nextBipT

	result = pkb->getNextBipT();
	REQUIRE(result.size() == 14);
	REQUIRE(result.contains({ "1", "2" }));
	REQUIRE(result.contains({ "1", "3" }));
	REQUIRE(result.contains({ "1", "4" }));
	REQUIRE(result.contains({ "1", "5" }));
	REQUIRE(result.contains({ "1", "6" }));
	REQUIRE(result.contains({ "2", "3" }));
	REQUIRE(result.contains({ "2", "4" }));
	REQUIRE(result.contains({ "2", "5" }));
	REQUIRE(result.contains({ "2", "6" }));
	REQUIRE(result.contains({ "4", "3" }));
	REQUIRE(result.contains({ "4", "5" }));
	REQUIRE(result.contains({ "4", "6" }));
	REQUIRE(result.contains({ "5", "3" }));
	REQUIRE(result.contains({ "6", "3" }));


}

TEST_CASE("Non last statement call to procedure ending with call statement") {
	std::unique_ptr<PKB> pkb{ new PKB() };
	pkb->setProc("A", 1, 4);
	pkb->setProc("B", 4, 6);
	pkb->setProc("C", 6, 8);
	pkb->setNext(1, 2);
	pkb->setNext(2, 3);
	pkb->setNext(4, 5);
	pkb->setNext(6, 7);

	pkb->setStmtType(2, StatementType::Call);
	pkb->setCallProcName(2, "B");
	pkb->setStmtType(5, StatementType::Call);
	pkb->setCallProcName(5, "C");
	pkb->setProcExitStmt("A", std::vector<int> {3});
	pkb->setProcExitStmt("B", std::vector<int> {5});
	pkb->setProcExitStmt("C", std::vector<int> {7});

	DesignExtractor::populateDesigns(pkb);

	//testing nextBip
	Table result = pkb->getNextBip();
	REQUIRE(result.size() == 6);
	REQUIRE(result.contains({ "1", "2" }));
	REQUIRE(result.contains({ "2", "4" }));
	REQUIRE(result.contains({ "4", "5" }));
	REQUIRE(result.contains({ "5", "6" }));
	REQUIRE(result.contains({ "6", "7" }));
	REQUIRE(result.contains({ "7", "3" }));

	//testing nextBipT

	result = pkb->getNextBipT();
	REQUIRE(result.size() == 21);
	REQUIRE(result.contains({ "1", "2" }));
	REQUIRE(result.contains({ "1", "3" }));
	REQUIRE(result.contains({ "1", "4" }));
	REQUIRE(result.contains({ "1", "5" }));
	REQUIRE(result.contains({ "1", "6" }));
	REQUIRE(result.contains({ "1", "7" }));
	REQUIRE(result.contains({ "2", "3" }));
	REQUIRE(result.contains({ "2", "4" }));
	REQUIRE(result.contains({ "2", "5" }));
	REQUIRE(result.contains({ "2", "6" }));
	REQUIRE(result.contains({ "2", "7" }));
	REQUIRE(result.contains({ "4", "3" }));
	REQUIRE(result.contains({ "4", "5" }));
	REQUIRE(result.contains({ "4", "6" }));
	REQUIRE(result.contains({ "4", "7" }));
	REQUIRE(result.contains({ "5", "3" }));
	REQUIRE(result.contains({ "5", "6" }));
	REQUIRE(result.contains({ "5", "7" }));
	REQUIRE(result.contains({ "6", "3" }));
	REQUIRE(result.contains({ "6", "7" }));
	REQUIRE(result.contains({ "7", "3" }));


}

TEST_CASE("Non last statement call to procedure ending with call statement to procedure ending with if statement") {
	std::unique_ptr<PKB> pkb{ new PKB() };
	pkb->setProc("A", 1, 4);
	pkb->setProc("B", 4, 5);
	pkb->setProc("C", 5, 8);
	pkb->setNext(1, 2);
	pkb->setNext(2, 3);
	pkb->setNext(5, 6);
	pkb->setNext(5, 7);

	pkb->setStmtType(2, StatementType::Call);
	pkb->setCallProcName(2, "B");
	pkb->setStmtType(4, StatementType::Call);
	pkb->setCallProcName(4, "C");
	pkb->setStmtType(5, StatementType::If);
	pkb->setProcExitStmt("A", std::vector<int> {3});
	pkb->setProcExitStmt("B", std::vector<int> {4});
	pkb->setProcExitStmt("C", std::vector<int> {6, 7});

	DesignExtractor::populateDesigns(pkb);

	//testing nextBip
	Table result = pkb->getNextBip();
	REQUIRE(result.size() == 7);
	REQUIRE(result.contains({ "1", "2" }));
	REQUIRE(result.contains({ "2", "4" }));
	REQUIRE(result.contains({ "4", "5" }));
	REQUIRE(result.contains({ "5", "6" }));
	REQUIRE(result.contains({ "5", "7" }));
	REQUIRE(result.contains({ "6", "3" }));
	REQUIRE(result.contains({ "7", "3" }));

	//testing nextBipT
	result = pkb->getNextBipT();
	REQUIRE(result.size() == 20);
	REQUIRE(result.contains({ "1", "2" }));
	REQUIRE(result.contains({ "1", "3" }));
	REQUIRE(result.contains({ "1", "4" }));
	REQUIRE(result.contains({ "1", "5" }));
	REQUIRE(result.contains({ "1", "6" }));
	REQUIRE(result.contains({ "1", "7" }));
	REQUIRE(result.contains({ "2", "3" }));
	REQUIRE(result.contains({ "2", "4" }));
	REQUIRE(result.contains({ "2", "5" }));
	REQUIRE(result.contains({ "2", "6" }));
	REQUIRE(result.contains({ "2", "7" }));
	REQUIRE(result.contains({ "4", "3" }));
	REQUIRE(result.contains({ "4", "5" }));
	REQUIRE(result.contains({ "4", "6" }));
	REQUIRE(result.contains({ "4", "7" }));
	REQUIRE(result.contains({ "5", "3" }));
	REQUIRE(result.contains({ "5", "6" }));
	REQUIRE(result.contains({ "5", "7" }));
	REQUIRE(result.contains({ "6", "3" }));
	REQUIRE(result.contains({ "7", "3" }));


}

TEST_CASE("Non last statement call to procedure ending with call statement to procedure ending with while loop") {
	std::unique_ptr<PKB> pkb{ new PKB() };
	pkb->setProc("A", 1, 4);
	pkb->setProc("B", 4, 5);
	pkb->setProc("C", 5, 7);
	pkb->setNext(1, 2);
	pkb->setNext(2, 3);
	pkb->setNext(5, 6);
	pkb->setNext(6, 5);

	pkb->setStmtType(2, StatementType::Call);
	pkb->setCallProcName(2, "B");
	pkb->setStmtType(4, StatementType::Call);
	pkb->setCallProcName(4, "C");
	pkb->setStmtType(5, StatementType::While);
	pkb->setProcExitStmt("A", std::vector<int> {3});
	pkb->setProcExitStmt("B", std::vector<int> {4});
	pkb->setProcExitStmt("C", std::vector<int> {5});

	DesignExtractor::populateDesigns(pkb);

	//testing nextBip
	Table result = pkb->getNextBip();
	REQUIRE(result.size() == 6);
	REQUIRE(result.contains({ "1", "2" }));
	REQUIRE(result.contains({ "2", "4" }));
	REQUIRE(result.contains({ "4", "5" }));
	REQUIRE(result.contains({ "5", "6" }));
	REQUIRE(result.contains({ "6", "5" }));
	REQUIRE(result.contains({ "5", "3" }));

	//testing nextBipT

	result = pkb->getNextBipT();
	REQUIRE(result.size() == 18);
	REQUIRE(result.contains({ "1", "2" }));
	REQUIRE(result.contains({ "1", "3" }));
	REQUIRE(result.contains({ "1", "4" }));
	REQUIRE(result.contains({ "1", "5" }));
	REQUIRE(result.contains({ "1", "6" }));
	REQUIRE(result.contains({ "2", "3" }));
	REQUIRE(result.contains({ "2", "4" }));
	REQUIRE(result.contains({ "2", "5" }));
	REQUIRE(result.contains({ "2", "6" }));
	REQUIRE(result.contains({ "4", "3" }));
	REQUIRE(result.contains({ "4", "5" }));
	REQUIRE(result.contains({ "4", "6" }));
	REQUIRE(result.contains({ "5", "3" }));
	REQUIRE(result.contains({ "5", "5" }));
	REQUIRE(result.contains({ "5", "6" }));
	REQUIRE(result.contains({ "6", "3" }));
	REQUIRE(result.contains({ "6", "5" }));
	REQUIRE(result.contains({ "6", "6" }));

}

TEST_CASE("Two procedures calling same procedure") {
	std::unique_ptr<PKB> pkb{ new PKB() };
	pkb->setProc("A", 1, 4);
	pkb->setProc("B", 4, 7);
	pkb->setProc("C", 7, 9);
	pkb->setNext(1, 2);
	pkb->setNext(2, 3);
	pkb->setNext(4, 5);
	pkb->setNext(5, 6);
	pkb->setNext(7, 8);
	pkb->setStmtType(2, StatementType::Call);
	pkb->setCallProcName(2, "C");
	pkb->setStmtType(5, StatementType::Call);
	pkb->setCallProcName(5, "C");
	pkb->setProcExitStmt("A", std::vector<int> {3});
	pkb->setProcExitStmt("B", std::vector<int> {6});
	pkb->setProcExitStmt("C", std::vector<int> {8});

	DesignExtractor::populateDesigns(pkb);

	//testing nextBip
	Table result = pkb->getNextBip();
	REQUIRE(result.size() == 7);
	REQUIRE(result.contains({ "1", "2" }));
	REQUIRE(result.contains({ "2", "7" }));
	REQUIRE(result.contains({ "7", "8" }));
	REQUIRE(result.contains({ "8", "3" }));
	REQUIRE(result.contains({ "4", "5" }));
	REQUIRE(result.contains({ "5", "7" }));
	REQUIRE(result.contains({ "8", "6" }));

	//testing nextBipT
	result = pkb->getNextBipT();
	REQUIRE(result.size() == 19);
	REQUIRE(result.contains({ "1", "2" }));
	REQUIRE(result.contains({ "1", "3" }));
	REQUIRE(result.contains({ "1", "7" }));
	REQUIRE(result.contains({ "1", "8" }));
	REQUIRE(result.contains({ "2", "3" }));
	REQUIRE(result.contains({ "2", "7" }));
	REQUIRE(result.contains({ "2", "8" }));
	REQUIRE(result.contains({ "4", "5" }));
	REQUIRE(result.contains({ "4", "6" }));
	REQUIRE(result.contains({ "4", "7" }));
	REQUIRE(result.contains({ "4", "8" }));
	REQUIRE(result.contains({ "5", "6" }));
	REQUIRE(result.contains({ "5", "7" }));
	REQUIRE(result.contains({ "5", "8" }));
	REQUIRE(result.contains({ "7", "3" }));
	REQUIRE(result.contains({ "7", "6" }));
	REQUIRE(result.contains({ "7", "8" }));
	REQUIRE(result.contains({ "8", "3" }));
	REQUIRE(result.contains({ "8", "6" }));


}

TEST_CASE("Two procedures calling same procedure ending with if statement") {
	std::unique_ptr<PKB> pkb{ new PKB() };
	pkb->setProc("A", 1, 4);
	pkb->setProc("B", 4, 7);
	pkb->setProc("C", 7, 10);
	pkb->setNext(1, 2);
	pkb->setNext(2, 3);
	pkb->setNext(4, 5);
	pkb->setNext(5, 6);
	pkb->setNext(7, 8);
	pkb->setNext(7, 9);
	pkb->setStmtType(2, StatementType::Call);
	pkb->setCallProcName(2, "C");
	pkb->setStmtType(5, StatementType::Call);
	pkb->setCallProcName(5, "C");
	pkb->setStmtType(7, StatementType::If);
	pkb->setProcExitStmt("A", std::vector<int> {3});
	pkb->setProcExitStmt("B", std::vector<int> {6});
	pkb->setProcExitStmt("C", std::vector<int> {8, 9});
	DesignExtractor::populateDesigns(pkb);

	//testing nextBip
	Table result = pkb->getNextBip();
	REQUIRE(result.size() == 10);
	REQUIRE(result.contains({ "1", "2" }));
	REQUIRE(result.contains({ "2", "7" }));
	REQUIRE(result.contains({ "7", "8" }));
	REQUIRE(result.contains({ "7", "9" }));
	REQUIRE(result.contains({ "8", "3" }));
	REQUIRE(result.contains({ "9", "3" }));
	REQUIRE(result.contains({ "4", "5" }));
	REQUIRE(result.contains({ "5", "7" }));
	REQUIRE(result.contains({ "8", "6" }));
	REQUIRE(result.contains({ "9", "6" }));

	//testing nextBipT
	result = pkb->getNextBipT();
	REQUIRE(result.size() == 26);
	REQUIRE(result.contains({ "1", "2" }));
	REQUIRE(result.contains({ "1", "3" }));
	REQUIRE(result.contains({ "1", "7" }));
	REQUIRE(result.contains({ "1", "8" }));
	REQUIRE(result.contains({ "1", "9" }));
	REQUIRE(result.contains({ "2", "3" }));
	REQUIRE(result.contains({ "2", "7" }));
	REQUIRE(result.contains({ "2", "8" }));
	REQUIRE(result.contains({ "2", "9" }));
	REQUIRE(result.contains({ "4", "5" }));
	REQUIRE(result.contains({ "4", "6" }));
	REQUIRE(result.contains({ "4", "7" }));
	REQUIRE(result.contains({ "4", "8" }));
	REQUIRE(result.contains({ "4", "9" }));
	REQUIRE(result.contains({ "5", "6" }));
	REQUIRE(result.contains({ "5", "7" }));
	REQUIRE(result.contains({ "5", "8" }));
	REQUIRE(result.contains({ "5", "9" }));
	REQUIRE(result.contains({ "7", "3" }));
	REQUIRE(result.contains({ "7", "6" }));
	REQUIRE(result.contains({ "7", "8" }));
	REQUIRE(result.contains({ "7", "9" }));
	REQUIRE(result.contains({ "8", "3" }));
	REQUIRE(result.contains({ "8", "6" }));
	REQUIRE(result.contains({ "9", "3" }));
	REQUIRE(result.contains({ "9", "6" }));


}

TEST_CASE("Two procedures calling same procedure ending with while statement") {
	std::unique_ptr<PKB> pkb{ new PKB() };
	pkb->setProc("A", 1, 4);
	pkb->setProc("B", 4, 7);
	pkb->setProc("C", 7, 10);
	pkb->setNext(1, 2);
	pkb->setNext(2, 3);
	pkb->setNext(4, 5);
	pkb->setNext(5, 6);
	pkb->setNext(7, 8);
	pkb->setNext(8, 7);
	pkb->setStmtType(2, StatementType::Call);
	pkb->setCallProcName(2, "C");
	pkb->setStmtType(5, StatementType::Call);
	pkb->setCallProcName(5, "C");
	pkb->setStmtType(7, StatementType::While);
	pkb->setProcExitStmt("A", std::vector<int> {3});
	pkb->setProcExitStmt("B", std::vector<int> {6});
	pkb->setProcExitStmt("C", std::vector<int> {7});
	DesignExtractor::populateDesigns(pkb);

	//testing nextBip
	Table result = pkb->getNextBip();
	REQUIRE(result.size() == 8);
	REQUIRE(result.contains({ "1", "2" }));
	REQUIRE(result.contains({ "2", "7" }));
	REQUIRE(result.contains({ "7", "8" }));
	REQUIRE(result.contains({ "8", "7" }));
	REQUIRE(result.contains({ "7", "3" }));
	REQUIRE(result.contains({ "4", "5" }));
	REQUIRE(result.contains({ "5", "7" }));
	REQUIRE(result.contains({ "7", "6" }));

	//testing nextBipT
	result = pkb->getNextBipT();
	REQUIRE(result.size() == 22);
	REQUIRE(result.contains({ "1", "2" }));
	REQUIRE(result.contains({ "1", "3" }));
	REQUIRE(result.contains({ "1", "7" }));
	REQUIRE(result.contains({ "1", "8" }));
	REQUIRE(result.contains({ "2", "3" }));
	REQUIRE(result.contains({ "2", "7" }));
	REQUIRE(result.contains({ "2", "8" }));
	REQUIRE(result.contains({ "4", "5" }));
	REQUIRE(result.contains({ "4", "6" }));
	REQUIRE(result.contains({ "4", "7" }));
	REQUIRE(result.contains({ "4", "8" }));
	REQUIRE(result.contains({ "5", "6" }));
	REQUIRE(result.contains({ "5", "7" }));
	REQUIRE(result.contains({ "5", "8" }));
	REQUIRE(result.contains({ "7", "3" }));
	REQUIRE(result.contains({ "7", "6" }));
	REQUIRE(result.contains({ "7", "7" }));
	REQUIRE(result.contains({ "7", "8" }));
	REQUIRE(result.contains({ "8", "3" }));
	REQUIRE(result.contains({ "8", "6" }));
	REQUIRE(result.contains({ "8", "7" }));
	REQUIRE(result.contains({ "8", "8" }));

}

TEST_CASE("Two procedures calling same procedure ending with call statement") {
	std::unique_ptr<PKB> pkb{ new PKB() };
	pkb->setProc("A", 1, 4);
	pkb->setProc("B", 4, 6);
	pkb->setProc("C", 6, 8);
	pkb->setProc("D", 8, 11);

	pkb->setNext(1, 2);
	pkb->setNext(2, 3);
	pkb->setNext(4, 5);
	pkb->setNext(6, 7);
	pkb->setNext(8, 9);
	pkb->setNext(9, 10);
	pkb->setStmtType(2, StatementType::Call);
	pkb->setCallProcName(2, "B");
	pkb->setStmtType(5, StatementType::Call);
	pkb->setCallProcName(5, "C");
	pkb->setStmtType(9, StatementType::Call);
	pkb->setCallProcName(9, "B");
	pkb->setProcExitStmt("A", std::vector<int> {3});
	pkb->setProcExitStmt("B", std::vector<int> {5});
	pkb->setProcExitStmt("C", std::vector<int> {7});
	pkb->setProcExitStmt("D", std::vector<int> {10});
	DesignExtractor::populateDesigns(pkb);

	//testing nextBip
	Table result = pkb->getNextBip();
	REQUIRE(result.size() == 9);
	REQUIRE(result.contains({ "1", "2" }));
	REQUIRE(result.contains({ "2", "4" }));
	REQUIRE(result.contains({ "4", "5" }));
	REQUIRE(result.contains({ "5", "6" }));
	REQUIRE(result.contains({ "6", "7" }));
	REQUIRE(result.contains({ "7", "3" }));
	REQUIRE(result.contains({ "8", "9" }));
	REQUIRE(result.contains({ "9", "4" }));
	REQUIRE(result.contains({ "7", "10" }));

	//testing nextBipT
	result = pkb->getNextBipT();
	REQUIRE(result.size() == 36);
	REQUIRE(result.contains({ "1", "2" }));
	REQUIRE(result.contains({ "1", "3" }));
	REQUIRE(result.contains({ "1", "4" }));
	REQUIRE(result.contains({ "1", "5" }));
	REQUIRE(result.contains({ "1", "6" }));
	REQUIRE(result.contains({ "1", "7" }));
	REQUIRE(result.contains({ "2", "3" }));
	REQUIRE(result.contains({ "2", "4" }));
	REQUIRE(result.contains({ "2", "5" }));
	REQUIRE(result.contains({ "2", "6" }));
	REQUIRE(result.contains({ "2", "7" }));
	REQUIRE(result.contains({ "4", "5" }));
	REQUIRE(result.contains({ "4", "6" }));
	REQUIRE(result.contains({ "4", "7" }));
	REQUIRE(result.contains({ "4", "3" }));
	REQUIRE(result.contains({ "4", "10" }));
	REQUIRE(result.contains({ "5", "3" }));
	REQUIRE(result.contains({ "5", "6" }));
	REQUIRE(result.contains({ "5", "7" }));
	REQUIRE(result.contains({ "5", "10" }));
	REQUIRE(result.contains({ "6", "3" }));
	REQUIRE(result.contains({ "6", "7" }));
	REQUIRE(result.contains({ "6", "10" }));
	REQUIRE(result.contains({ "7", "3" }));
	REQUIRE(result.contains({ "7", "10" }));
	REQUIRE(result.contains({ "8", "4" }));
	REQUIRE(result.contains({ "8", "5" }));
	REQUIRE(result.contains({ "8", "6" }));
	REQUIRE(result.contains({ "8", "7" }));
	REQUIRE(result.contains({ "8", "9" }));
	REQUIRE(result.contains({ "8", "10" }));
	REQUIRE(result.contains({ "9", "4" }));
	REQUIRE(result.contains({ "9", "5" }));
	REQUIRE(result.contains({ "9", "6" }));
	REQUIRE(result.contains({ "9", "7" }));
	REQUIRE(result.contains({ "9", "10" }));


}
TEST_CASE("More complex call graph") {
	std::unique_ptr<PKB> pkb{ new PKB() };
	pkb->setProc("A", 1, 6);
	pkb->setProc("B", 6, 9);
	pkb->setProc("C", 9, 12);
	pkb->setNext(1, 2);
	pkb->setNext(2, 3);
	pkb->setNext(3, 4);
	pkb->setNext(4, 5);
	pkb->setNext(6, 7);
	pkb->setNext(7, 8);
	pkb->setNext(9, 10);
	pkb->setNext(9, 11);
	pkb->setStmtType(2, StatementType::Call);
	pkb->setCallProcName(2, "B");
	pkb->setStmtType(4, StatementType::Call);
	pkb->setCallProcName(4, "C");
	pkb->setStmtType(7, StatementType::Call);
	pkb->setCallProcName(7, "C");
	pkb->setStmtType(9, StatementType::If);
	pkb->setProcExitStmt("A", std::vector<int> {5});
	pkb->setProcExitStmt("B", std::vector<int> {8});
	pkb->setProcExitStmt("C", std::vector<int> {10, 11});

	DesignExtractor::populateDesigns(pkb);

	//testing nextBip
	Table result = pkb->getNextBip();
	REQUIRE(result.size() == 13);
	REQUIRE(result.contains({ "1", "2" }));
	REQUIRE(result.contains({ "2", "6" }));
	REQUIRE(result.contains({ "6", "7" }));
	REQUIRE(result.contains({ "7", "9" }));
	REQUIRE(result.contains({ "9", "10" }));
	REQUIRE(result.contains({ "9", "11" }));
	REQUIRE(result.contains({ "10", "8" }));
	REQUIRE(result.contains({ "11", "8" }));
	REQUIRE(result.contains({ "8", "3" }));
	REQUIRE(result.contains({ "3", "4" }));
	REQUIRE(result.contains({ "4", "9" }));
	REQUIRE(result.contains({ "10", "5" }));
	REQUIRE(result.contains({ "11", "5" }));

	//testing nextBipT
	
	result = pkb->getNextBipT();
	REQUIRE(result.size() == 70);
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

	REQUIRE(result.contains({ "2", "3" }));
	REQUIRE(result.contains({ "2", "4" }));
	REQUIRE(result.contains({ "2", "5" }));
	REQUIRE(result.contains({ "2", "6" }));
	REQUIRE(result.contains({ "2", "7" }));
	REQUIRE(result.contains({ "2", "8" }));
	REQUIRE(result.contains({ "2", "9" }));
	REQUIRE(result.contains({ "2", "10" }));
	REQUIRE(result.contains({ "2", "11" }));

	REQUIRE(result.contains({ "3", "4" }));
	REQUIRE(result.contains({ "3", "5" }));
	REQUIRE(result.contains({ "3", "9" }));
	REQUIRE(result.contains({ "3", "10" }));
	REQUIRE(result.contains({ "3", "11" }));

	REQUIRE(result.contains({ "4", "5" }));
	REQUIRE(result.contains({ "4", "9" }));
	REQUIRE(result.contains({ "4", "10" }));
	REQUIRE(result.contains({ "4", "11" }));

	REQUIRE(result.contains({ "6", "3" }));
	REQUIRE(result.contains({ "6", "4" }));
	REQUIRE(result.contains({ "6", "5" }));
	REQUIRE(result.contains({ "6", "7" }));
	REQUIRE(result.contains({ "6", "8" }));
	REQUIRE(result.contains({ "6", "9" }));
	REQUIRE(result.contains({ "6", "10" }));
	REQUIRE(result.contains({ "6", "11" }));

	REQUIRE(result.contains({ "7", "3" }));
	REQUIRE(result.contains({ "7", "4" }));
	REQUIRE(result.contains({ "7", "5" }));
	REQUIRE(result.contains({ "7", "8" }));
	REQUIRE(result.contains({ "7", "9" }));
	REQUIRE(result.contains({ "7", "10" }));
	REQUIRE(result.contains({ "7", "11" }));

	REQUIRE(result.contains({ "8", "3" }));
	REQUIRE(result.contains({ "8", "4" }));
	REQUIRE(result.contains({ "8", "5" }));
	REQUIRE(result.contains({ "8", "9" }));
	REQUIRE(result.contains({ "8", "10" }));
	REQUIRE(result.contains({ "8", "11" }));

	REQUIRE(result.contains({ "9", "3" }));
	REQUIRE(result.contains({ "9", "4" }));
	REQUIRE(result.contains({ "9", "5" }));
	REQUIRE(result.contains({ "9", "8" }));
	REQUIRE(result.contains({ "9", "9" }));
	REQUIRE(result.contains({ "9", "10" }));
	REQUIRE(result.contains({ "9", "11" }));

	REQUIRE(result.contains({ "10", "3" }));
	REQUIRE(result.contains({ "10", "4" }));
	REQUIRE(result.contains({ "10", "5" }));
	REQUIRE(result.contains({ "10", "8" }));
	REQUIRE(result.contains({ "10", "9" }));
	REQUIRE(result.contains({ "10", "10" }));
	REQUIRE(result.contains({ "10", "11" }));

	REQUIRE(result.contains({ "11", "3" }));
	REQUIRE(result.contains({ "11", "4" }));
	REQUIRE(result.contains({ "11", "5" }));
	REQUIRE(result.contains({ "11", "8" }));
	REQUIRE(result.contains({ "11", "9" }));
	REQUIRE(result.contains({ "11", "10" }));
	REQUIRE(result.contains({ "11", "11" }));

}


