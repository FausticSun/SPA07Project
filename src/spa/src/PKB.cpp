#include "PKB.h"
PKB::PKB(const std::set<std::string> &varTable,
         const std::set<std::string> &procTable,
         const std::set<Relation> &relTable)
    : varTable(varTable), procTable(procTable), relTable(relTable){};
const std::set<std::string> PKB::getVarTable() const { return this->varTable; }
const std::set<std::string> PKB::getProcTable() const {
  return this->procTable;
}
const std::set<Relation> PKB::getRelTable() const { return this->relTable; }
