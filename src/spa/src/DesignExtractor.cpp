#include "DesignExtractor.h"
#include "Table.h"
#include <list>
#include <queue>
#include <stack>
#define ENABLE_BIP 1

// Kahn's algorithm for toposort
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

// Populate Follows* relations
void populateFollowsT(std::unique_ptr<PKB> &pkb) {
  auto table = pkb->getFollows();
  table.transitiveClosure();
  for (auto data : table.getData()) {
    pkb->setFollowsT(std::stoi(data[0]), std::stoi(data[1]));
  }
}

// Populate Parent* relations
void populateParentT(std::unique_ptr<PKB> &pkb) {
  auto table = pkb->getParent();
  table.transitiveClosure();
  for (auto data : table.getData()) {
    pkb->setParentT(std::stoi(data[0]), std::stoi(data[1]));
  }
}

// Populate Calls* relations
void populateCallsT(std::unique_ptr<PKB> &pkb) {
  auto table = pkb->getCalls();
  table.transitiveClosure();
  for (auto data : table.getData()) {
    pkb->setCallsT(data[0], data[1]);
  }
}

// Populate Uses relations for while/if statements
void populateUsesS(std::unique_ptr<PKB> &pkb) {
  // Uses(w/ifs, v) = Parent*(w/ifs, s) and Uses(s, v)
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

// Populate Modifies relations for while/if statements
void populateModifiesS(std::unique_ptr<PKB> &pkb) {
  // Modifies(w/ifs, v) = Parent*(w/ifs, s) and Modifies(s, v)
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

// Helper to get a merged table with calls and uses/modifies procedures
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

// Helper to get a merged table with call stmts and uses/modifies procedures
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

// Populate Uses and Modifies relations for call stmts and the respective
// procedures that the stmts are in
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

// Populate CFG
void populateCFG(std::unique_ptr<PKB> &pkb) {
  // Get all while and if line numbers
  auto whileIfTable = pkb->getStmtType(StatementType::While);
  auto ifTable = pkb->getStmtType(StatementType::If);
  whileIfTable.concatenate(ifTable);
  auto whileParentTable = pkb->getStmtType(StatementType::While);
  whileParentTable.setHeader({"w"});
  auto parentTTable = pkb->getParentT();
  parentTTable.setHeader({"w", "s"});
  whileParentTable.mergeWith(parentTTable);
  CFG graph = CFG{pkb->getProcStmt(),
                  pkb->getNext(),
                  pkb->getModifiesS(),
                  pkb->getUsesS(),
                  whileIfTable,
                  whileParentTable,
                  pkb->getStmtType(StatementType::Assign),
                  pkb->getStmtCount()};
  pkb->setCFG(graph);
}

void populateCFGBip(std::unique_ptr<PKB> &pkb) {
  auto whileIfTable = pkb->getStmtType(StatementType::While);
  auto ifTable = pkb->getStmtType(StatementType::If);
  whileIfTable.concatenate(ifTable);
  CFGBip graph = CFGBip{pkb->getStmtType(StatementType::Call),
                        pkb->getNext(),
                        pkb->getProcStmt(),
                        pkb->getCallProcName(),
                        pkb->getProcExitStmt(),
                        pkb->getStmtCount(),
                        pkb->getUsesS(),
                        pkb->getModifiesS(),
                        pkb->getStmtType(StatementType::Assign),
                        whileIfTable};

  pkb->setCFGBip(graph);
  pkb->setNextBip(graph.getNextBip());
}

void DesignExtractor::populateDesigns(std::unique_ptr<PKB> &pkb) {
  validateProcs(pkb);
  validateCyclicCalls(pkb);
  populateFollowsT(pkb);
  populateParentT(pkb);
  populateCallsT(pkb);
  populateUsesAndModifiesC(pkb);
  populateUsesS(pkb);
  populateModifiesS(pkb);
  populateCFG(pkb);
#ifdef ENABLE_BIP
  populateCFGBip(pkb);
#endif
}
