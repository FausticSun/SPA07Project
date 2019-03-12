#include "DesignExtractor.h"
#include "Table.h"

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
  // Extract to adjacent list
  std::map<std::string, std::set<std::string>> adjList;
  for (auto data : pkb->getCalls().getData()) {
    adjList[data[0]].insert(data[1]);
  }
  // Peform DFS
  std::list<std::string> stack;
  std::map<std::string, bool> visited;
  while (!stack.empty()) {
  }
}

void populateFollowsT(std::unique_ptr<PKB> &pkb) {
  auto table = pkb->getFollows();
  table.transitiveClosure();
  for (auto data : table.getData()) {
    pkb->setFollowsT(data[0], data[1]);
  }
}

void populateParentT(std::unique_ptr<PKB> &pkb) {
  auto table = pkb->getParent();
  table.transitiveClosure();
  for (auto data : table.getData()) {
    pkb->setParentT(data[0], data[1]);
  }
}

void populateCallsT(std::unique_ptr<PKB> &pkb) {
  auto table = pkb->getCall();
  table.transitiveClosure();
  for (auto data : table.getData()) {
    pkb->setCallT(data[0], data[1]);
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

  table.concatenate(parentTTable);
  table.concatenate(usesSTable);
  for (auto data : table.getData({"w/ifs", "s1"})) {
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

  table.concatenate(parentTTable);
  table.concatenate(modifiesSTable);
  for (auto data : table.getData({"w/ifs", "s1"})) {
    pkb->setModifies(std::stoi(data[0]), data[1]);
  }
}

void populateUsesAndModifiesS(std::unique_ptr<PKB> &pkb) {
  auto callsTable = pkb->getCallsT();
}

void populateCFG(std::unique_ptr<PKB> &pkb) {}

void DesignExtractor::populateDesigns(std::unique_ptr<PKB> &pkb) {
  validateProcs(pkb);
  validateCyclicCalls(pkb);
  populateFollowsT(pkb);
  populateParentT(pkb);
  populateCallsT(pkb);
  populateUsesS(pkb);
  populateModifiesS(pkb);
  populateUsesAndModifiesS(pkb);
  populateCFG(pkb);
}
