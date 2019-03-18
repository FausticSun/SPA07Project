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

Table getTableForCallProc(Table &callsTable, Table &otherTable,
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

Table getTableForCallStmt(Table &callProcNameTable, Table &otherTable,
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
  auto callProcNameTable = pkb->getCallProcNameTable();
  auto usesPTable = pkb->getUsesP();
  auto modifiesPTable = pkb->getModifiesP();

  std::vector<std::string> topologicalOrder = topologicalSort(callsTable);
  for (auto const proc : topologicalOrder) {
    // Update procedure that contains call stmt
    Table t1 = getTableForCallProc(callsTable, usesPTable, proc);
    for (auto data : t1.getData({"p1", "v"})) {
      pkb->setUses(data[0], data[1]);
    }
    Table t2 = getTableForCallProc(callsTable, modifiesPTable, proc);
    for (auto data : t2.getData({"p1", "v"})) {
      pkb->setModifies(data[0], data[1]);
    }

    // Update call stmt itself
    Table t3 = getTableForCallStmt(callProcNameTable, usesPTable, proc);
    for (auto data : t3.getData({"s", "v"})) {
      pkb->setUses(std::stoi(data[0]), data[1]);
    }
    Table t4 = getTableForCallStmt(callProcNameTable, modifiesPTable, proc);
    for (auto data : t4.getData({"s", "v"})) {
      pkb->setModifies(std::stoi(data[0]), data[1]);
    }
  }
}

void populateCFG(std::unique_ptr<PKB> &pkb) {
  // Get first line number of each procedure
  auto procStmtTable = pkb->getProcStmt();
  procStmtTable.setHeader({"proc", "n1"});
  auto nextTable = pkb->getNext();
  nextTable.setHeader({"n1", "n2"});
  procStmtTable.mergeWith(nextTable);

  // Get all while and if line numbers
  auto whileIfTable = pkb->getStmtType(StatementType::While);
  auto ifTable = pkb->getStmtType(StatementType::If);
  whileIfTable.concatenate(ifTable);

  for (auto data : procStmtTable.getData({"proc", "n1"})) {
    CFG graph = CFG{std::stoi(data[1]), pkb->getNext(), whileIfTable};
    pkb->setCFG(data[0], graph);
  }
}

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
}
