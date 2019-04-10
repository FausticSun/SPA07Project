#include "DesignExtractor.h"
#include "Table.h"
#include <list>
#include <queue>
#include <stack>

// Kahn's algorithm
std::vector<std::string> topologicalSort(Table callsTable) {
  // Create adjMap as graph DS and initialize all in degrees to 0
  std::map<std::string, std::vector<std::string>> adjMap;
  std::map<std::string, int> inDegree;
  for (auto data : callsTable.getData()) {
    adjMap[data[0]].push_back(data[1]);
    inDegree[data[0]] = 0;
    inDegree[data[1]] = 0;
  }

  // Traverse adjMap to fill in degrees of vertices
  for (auto const &i : adjMap) {
    for (auto const &j : adjMap[i.first]) {
      inDegree[j]++;
    }
  }

  // Enqueue all vertices with in degree 0
  std::queue<std::string> queue;
  for (auto const &i : inDegree) {
    if (inDegree[i.first] == 0) {
      queue.push(i.first);
    }
  }

  // Counter to track number of vertices visited
  int counter = 0;
  std::stack<std::string> stack;
  while (!queue.empty()) {
    std::string u = queue.front();
    queue.pop();
    stack.push(u);

    // Iterate through neighbours of u and decrease their in degree by 1
    // Enqueue if in degree of v becomes 0
    for (auto const &v : adjMap[u]) {
      inDegree[v]--;
      if (inDegree[v] == 0) {
        queue.push(v);
      }
    }
    counter++;
  }

  // Check for cycle
  if (counter != adjMap.size()) {
    throw std::logic_error("PKB contains cyclical calls");
  }

  // Transfer from stack to vector
  std::vector<std::string> topologicalOrder;
  while (!stack.empty()) {
    topologicalOrder.push_back(stack.top());
    stack.pop();
  }

  return topologicalOrder;
}

// Check program contains no calls to non-existing procedure
void validateProcs(std::unique_ptr<PKB> &pkb) {
  auto procTable = pkb->getProcTable();
  std::set<std::string> actualProcs;
  for (auto i : procTable.getData()) {
    actualProcs.insert(i[0]);
  }

  auto callTable = pkb->getCalls();
  for (auto i : callTable.getData()) {
    if (actualProcs.find(i[1]) == actualProcs.end()) {
      throw std::logic_error("PKB contains call to non-existent procedure");
    }
  }
}

// Check program contains no cyclic calls
void validateCyclicCalls(std::unique_ptr<PKB> &pkb) {
  auto callsTable = pkb->getCalls();
  topologicalSort(callsTable);
}

void populateFollowsT(std::unique_ptr<PKB> &pkb) {
  auto table = pkb->getFollows();
  table.transitiveClosure();
  for (auto data : table.getData()) {
    pkb->setFollowsT(std::stoi(data[0]), std::stoi(data[1]));
  }
}

void populateParentT(std::unique_ptr<PKB> &pkb) {
  auto table = pkb->getParent();
  table.transitiveClosure();
  for (auto data : table.getData()) {
    pkb->setParentT(std::stoi(data[0]), std::stoi(data[1]));
  }
}

void populateCallsT(std::unique_ptr<PKB> &pkb) {
  auto table = pkb->getCalls();
  table.transitiveClosure();
  for (auto data : table.getData()) {
    pkb->setCallsT(data[0], data[1]);
  }
}

void populateUsesS(std::unique_ptr<PKB> &pkb) {
  // parent*(w/ifs, s1) and uses(s1, v)
  auto table = pkb->getStmtType(StatementType::While);
  table.concatenate(pkb->getStmtType(StatementType::If));
  table.setHeader({"w/ifs"});

  auto parentTTable = pkb->getParentT();
  parentTTable.setHeader({"w/ifs", "s1"});

  auto usesSTable = pkb->getUsesS();
  usesSTable.setHeader({"s1", "v"});

  table.mergeWith(parentTTable);
  table.mergeWith(usesSTable);
  for (auto data : table.getData({"w/ifs", "v"})) {
    pkb->setUses(std::stoi(data[0]), data[1]);
  }
}

void populateModifiesS(std::unique_ptr<PKB> &pkb) {
  // parent*(w/ifs, s1) and modifies(s1, v)
  auto table = pkb->getStmtType(StatementType::While);
  table.concatenate(pkb->getStmtType(StatementType::If));
  table.setHeader({"w/ifs"});

  auto parentTTable = pkb->getParentT();
  parentTTable.setHeader({"w/ifs", "s1"});

  auto modifiesSTable = pkb->getModifiesS();
  modifiesSTable.setHeader({"s1", "v"});

  table.mergeWith(parentTTable);
  table.mergeWith(modifiesSTable);
  for (auto data : table.getData({"w/ifs", "v"})) {
    pkb->setModifies(std::stoi(data[0]), data[1]);
  }
}

Table getTableForCallProc(Table callsTable, Table otherTable,
                          std::string proc) {
  callsTable.setHeader({"p1", "p2"});
  otherTable.setHeader({"p2", "v"});
  Table table{1};
  table.setHeader({"p2"});
  table.insertRow({proc});
  table.mergeWith(callsTable);
  table.mergeWith(otherTable);
  return table;
}

Table getTableForCallStmt(Table callProcNameTable, Table otherTable,
                          std::string proc) {
  callProcNameTable.setHeader({"s", "p2"});
  otherTable.setHeader({"p2", "v"});
  Table table{1};
  table.setHeader({"p2"});
  table.insertRow({proc});
  table.mergeWith(callProcNameTable);
  table.mergeWith(otherTable);
  return table;
}

void populateUsesAndModifiesC(std::unique_ptr<PKB> &pkb) {
  auto callsTable = pkb->getCalls();
  std::vector<std::string> topologicalOrder = topologicalSort(callsTable);
  for (auto const proc : topologicalOrder) {
    // Update procedure that contains call stmt
    Table t1 = getTableForCallProc(pkb->getCalls(), pkb->getUsesP(), proc);
    for (auto data : t1.getData({"p1", "v"})) {
      pkb->setUses(data[0], data[1]);
    }
    Table t2 = getTableForCallProc(pkb->getCalls(), pkb->getModifiesP(), proc);
    for (auto data : t2.getData({"p1", "v"})) {
      pkb->setModifies(data[0], data[1]);
    }

    // Update call stmt itself
    Table t3 =
        getTableForCallStmt(pkb->getCallProcNameTable(), pkb->getUsesP(), proc);
    for (auto data : t3.getData({"s", "v"})) {
      pkb->setUses(std::stoi(data[0]), data[1]);
    }
    Table t4 = getTableForCallStmt(pkb->getCallProcNameTable(),
                                   pkb->getModifiesP(), proc);
    for (auto data : t4.getData({"s", "v"})) {
      pkb->setModifies(std::stoi(data[0]), data[1]);
    }
  }
}

void populateCFG(std::unique_ptr<PKB> &pkb) {
  // Get all while and if line numbers
  auto whileIfTable = pkb->getStmtType(StatementType::While);
  auto ifTable = pkb->getStmtType(StatementType::If);
  whileIfTable.concatenate(ifTable);
  CFG graph = CFG{pkb->getProcStmt(), pkb->getNext(), whileIfTable,
                  pkb->getStmtCount()};
  pkb->setCFG(graph);
}

/**
void populateNextBip(std::unique_ptr<PKB> &pkb) {
	auto callTable = pkb->getStmtType(StatementType::Call); //1-col call stmt numbers
	auto nextTable = pkb->getNext(); //2-col next Table

	auto procTable = pkb->getProcStmt(); //3-col procName, start, end
	std::map<std::string, std::vector<int>> procMap;
	for (auto data : procTable.getData()) {
		procMap.insert(std::make_pair(data[0], std::vector<int>{std::stoi(data[1]), std::stoi(data[2])}));
	}

	auto callProcNameTable = pkb->getCallProcName(); //2-col stmt, proc
	std::map<int, std::string> callProcNameMap;
	for (auto data : callProcNameTable.getData()) {
		callProcNameMap.insert(std::make_pair(std::stoi(data[0]), data[1]));
	}

	auto procExitStmtTable = pkb->getProcExitStmt(); //2-col proc exit-i
	std::map<std::string, std::vector<int>> procExitStmtMap;
	for (auto data : procExitStmtTable.getData()) {
		if (procExitStmtMap.count(data[0]) == 1) {
			procExitStmtMap[data[0]].push_back(std::stoi(data[1]));
		}
		else {
			procExitStmtMap.insert(std::make_pair(data[0], std::vector<int>{std::stoi(data[1])}));
		}
	}

	std::map<int, int> callNext;
	for (auto data : nextTable.getData()) {
		// not a call statement
		if (!callTable.contains({ data[0] })) {
			pkb->setNextBip(std::stoi(data[0]), std::stoi(data[1]));
		}
		else {
			//store next for call statements
			callNext.insert(std::pair<int, int> (std::stoi(data[0]), std::stoi(data[1])));
		}
	}

	for (auto data : callProcNameTable.getData()) {
		//create branch in for all calls
		int stmt = std::stoi(data[0]);
		std::string procedure = data[1];
		pkb->setNextBip(stmt, procMap[procedure][0]);

		//create dummy node for call statements at exit stmt of procedure
		for (auto i : procExitStmtTable.getData()) {
			if (i[1] == std::to_string(stmt)) {
				callNext.insert(std::pair<int, int>(stmt, stmt * -1));
			}
		}
	}

	std::vector<int> uncheckedExits;
	std::map<int, int> branchbacks;
	for (auto i : callNext) {
		//create branch back
		int curr = i.first;
		int next = i.second;
		std::string procedure = callProcNameMap[curr];
		for (int exit : procExitStmtMap[procedure]) {
			if ((next > 0) && (!callTable.contains({std::to_string(exit)}))) {
				pkb->setNextBip(exit, next);
				branchbacks.insert(std::pair<int, int> (exit, next));
			}

			if (callTable.contains({ std::to_string(exit) })) {
				branchbacks.insert(std::pair<int, int> (exit * -1, next));
			}

			if ((next < 0) && (!callTable.contains({ std::to_string(exit) }))) {
				branchbacks.insert(std::pair<int, int> (exit, next));
				uncheckedExits.push_back(exit);
			}
		}
	}

	for (int exit : uncheckedExits) {
		int bb = branchbacks[exit];
		while (bb < 0) {
			bb = branchbacks[bb];
		}

		if (bb > 0) {
			pkb->setNextBip(exit, bb);
		}
	}

}
**/

void populateCFGBip(std::unique_ptr<PKB> &pkb) {
	CFGBip graph = CFGBip{ pkb->getStmtType(StatementType::Call), pkb->getNext(), pkb->getProcStmt(), 
		pkb->getCallProcName(), pkb->getProcExitStmt(), pkb->getStmtCount() };
	pkb->setNextBip(graph.getNextBip());
	pkb->setNextBipT(graph.getNextBipT());
}
/**
void populateNextBipT(std::unique_ptr<PKB> &pkb) {

	//creating adjacency list
	auto nextBipTable = pkb->getNextBip();
	int stmtCount = pkb->getStmtCount();
	std::vector<std::vector<int>> adjLst;
	adjLst.resize(stmtCount + 1);
	for (auto data : nextBipTable.getData()) {
		adjLst[std::stoi(data[0])].push_back(std::stoi(data[1]));
	}

	//BFS from every statement number
	for (int i = 1; i < stmtCount + 1; i++) {
		std::vector<bool> visited(stmtCount + 1, false);
		visited[0] = true;

		std::queue<int> q;
		q.push(i);
		while (!q.empty()) {
			int curr = q.front();
			q.pop();

			for (int j : adjLst[curr]) {
				if (!visited[j]) {
					visited[j] = true;
					q.push(j);
					pkb->setNextBipT(i, j);
				}
			}
		}
	}
	
}
**/

void DesignExtractor::populateDesigns(std::unique_ptr<PKB> &pkb) {
  validateProcs(pkb);
  validateCyclicCalls(pkb);
  populateFollowsT(pkb);
  populateParentT(pkb);
  populateCallsT(pkb);
  populateUsesS(pkb);
  populateModifiesS(pkb);
  populateUsesAndModifiesC(pkb);
  populateCFG(pkb);
  populateCFGBip(pkb);
  //populateNextBipT(pkb);
}
