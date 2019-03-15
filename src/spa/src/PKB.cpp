#include "PKB.h"

void PKB::setVar(const std::string &var) { varTable.insertRow({var}); }

void PKB::setProc(const std::string &proc, int start, int end) {
  if (procTable.count(proc) == 1) {
    throw std::logic_error("Procedure already exists");
  }
  procTable.insert(std::make_pair(proc, StmtRange(start, end)));
  stmtCount = end - 1 > stmtCount ? end - 1 : stmtCount;
}

void PKB::setStmtType(int stmt, StatementType type) {
  stmtTable[type].insert(stmt);
}

void PKB::setConst(int cons) { constTable.insertRow({std::to_string(cons)}); }

void PKB::setFollows(int s1, int s2) {
  followsTable.insertRow({std::to_string(s1), std::to_string(s2)});
}

void PKB::setFollowsT(int s1, int s2) {
  followsTTable.insertRow({std::to_string(s1), std::to_string(s2)});
}

void PKB::setParent(int s1, int s2) {
  parentTable.insertRow({std::to_string(s1), std::to_string(s2)});
}

void PKB::setParentT(int s1, int s2) {
  parentTTable.insertRow({std::to_string(s1), std::to_string(s2)});
}

void PKB::setUses(int s, std::string v) {
  setVar({v});
  usesSTable.insertRow({std::to_string(s), v});
}

void PKB::setUses(std::string p, std::string v) {
  setVar({v});
  usesPTable.insertRow({p, v});
}

void PKB::setModifies(int s, std::string v) {
  setVar({v});
  modifiesSTable.insertRow({std::to_string(s), v});
}

void PKB::setModifies(std::string p, std::string v) {
  setVar({v});
  modifiesPTable.insertRow({p, v});
}

void PKB::setCalls(std::string p1, std::string p2) {
  callsTable.insertRow({p1, p2});
}

void PKB::setCallsT(std::string p1, std::string p2) {
  callsTTable.insertRow({p1, p2});
}

// void PKB::setCallProcName(int s, std::string p) {
//   callProcNameTable.insertRow({std::to_string(s), p});
// }

void PKB::setNext(int s1, int s2) {
  nextTable.insertRow({std::to_string(s1), std::to_string(s2)});
}

void PKB::setAssign(int a, std::string &v, std::string &expr) {
  assignTable.insert(std::make_pair(a, std::make_pair(v, expr)));
}

void PKB::setIf(int ifs, const std::string &v) {
  ifTable.insertRow({std::to_string(ifs), v});
}

void PKB::setWhile(int w, const std::string &v) {
  whileTable.insertRow({std::to_string(w), v});
}

void PKB::setCallProcName(int stmtNo, const std::string &procName) {
  callProcNameTable.insertRow({std::to_string(stmtNo), procName});
}

Table PKB::getVarTable() const { return varTable; }

Table PKB::getProcTable() const {
  Table table{1};
  for (auto p : procTable) {
    table.insertRow({p.first});
  }
  return table;
}

Table PKB::getConstTable() const { return constTable; }

Table PKB::getStmtType(StatementType type) {
  Table table{1};
  if (type == StatementType::Stmt) {
    for (int i = 1; i <= stmtCount; ++i) {
      table.insertRow({std::to_string(i)});
    }
  } else {
    for (auto i : stmtTable[type]) {
      table.insertRow({std::to_string(i)});
    }
  }
  return table;
}

Table PKB::getFollows() const { return followsTable; }
Table PKB::getFollowsT() const { return followsTTable; }
Table PKB::getParent() const { return parentTable; }
Table PKB::getParentT() const { return parentTTable; }
Table PKB::getUsesS() const { return usesSTable; }
Table PKB::getUsesP() const { return usesPTable; }
Table PKB::getModifiesS() const { return modifiesSTable; }
Table PKB::getModifiesP() const { return modifiesPTable; }
Table PKB::getCalls() const { return callsTable; }
Table PKB::getCallsT() const { return callsTTable; }
Table PKB::getNext() const { return nextTable; }
Table PKB::getCallProcNameTable() const { return callProcNameTable; }

Table PKB::getAssignMatches(std::string expr, bool partial) {
  Table table{2};
  for (auto kv : assignTable) {
    if ((partial && kv.second.second.find(expr) != std::string::npos) ||
        (!partial && kv.second.second == expr)) {
      table.insertRow({std::to_string(kv.first), kv.second.first});
    }
  }
  return table;
}

Table PKB::getWhileMatches() { return whileTable; }
Table PKB::getIfMatches() { return ifTable; }
Table PKB::getCallProcName() { return callProcNameTable; }
