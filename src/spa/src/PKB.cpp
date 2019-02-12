#include "PKB.h"

PKB::PKB()
    : varTable(), procTable(), followsTable(), followedByTable(), parentTable(),
      parentOfTable(), usesTable(), usedByTable(), modifiesTable(),
      modifiedByTable() {}

void PKB::insertVar(const std::string var) { varTable.insert(var); }

void PKB::insertProc(const std::string proc) { procTable.insert(proc); }

const std::set<std::string> &PKB::getVarTable() const { return this->varTable; }
const std::set<std::string> &PKB::getProcTable() const {
  return this->procTable;
}
