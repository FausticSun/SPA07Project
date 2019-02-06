#include "PKB.h"
PKB::PKB() { this->setPKB(); }
void PKB::setPKB(const std::set<std::string> &variableTable,
                 const std::set<std::string> &procedureTable,
                 const std::set<Relation> &relationTable) {
  this->varTable = variableTable;
  this->procTable = procedureTable;
  this->relTable = relationTable;
}
const std::set<std::string> PKB::getVarTable() const { return this->varTable; }
const std::set<std::string> PKB::getProcTable() const {
  return this->procTable;
}
const std::set<Relation> PKB::getRelTable() const { return this->relTable; }
