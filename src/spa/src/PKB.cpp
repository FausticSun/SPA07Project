#include "PKB.h"

void PKB::insertVar(const std::string &var) { this->varTable.insert(var); }

void PKB::insertProc(const std::string &proc) { this->procTable.insert(proc); }

void PKB::insertConstant(const int constant) {
  this->constTable.insert(std::to_string(constant));
}

void PKB::insertStatement(const std::string &stmt, const StatementType type) {
  stmtCount++;
  this->stmtTable[type].insert(stmt);
}

void PKB::setFollows(int s, int t) {
  this->followsTable.setRelation(std::to_string(s), std::to_string(t));
}

void PKB::setFollowsT(int s, int t) {
  this->followsTTable.setRelation(std::to_string(s), std::to_string(t));
}

void PKB::setParent(int s, int t) {
  this->parentTable.setRelation(std::to_string(s), std::to_string(t));
}

void PKB::setParentT(int s, int t) {
  this->parentTTable.setRelation(std::to_string(s), std::to_string(t));
}

void PKB::setUses(int s, std::string t) {
  this->usesTable.setRelation(std::to_string(s), t);
}

void PKB::setModifies(int s, std::string t) {
  this->modifiesTable.setRelation(std::to_string(s), t);
}

const std::set<std::string> &PKB::getVarTable() const {
  return this->varTable;
}

const std::set<std::string> &PKB::getProcTable() const {
  return this->procTable;
}

const std::set<std::string> &PKB::getConstTable() const {
  return this->constTable;
}

const std::set<std::string> &
PKB::getStatementsOfType(StatementType type) const {
  if (type == StatementType::Stmt) {
    std::set<std::string> allStmt;
    for (int i = 1; i <= stmtCount; i++) {
      allStmt.insert(std::to_string(i));
    }
    return allStmt;
  } else {
    return this->stmtTable.at(type);
  }
}

StatementType PKB::getStatementType(std::string s) {
  for (auto it = this->stmtTable.begin(); it != this->stmtTable.end(); ++it) {
    if (it->second.find(s) != it->second.end()) {
      return it->first;
    }
  }
  return StatementType::Invalid;
}

bool PKB::follows(std::string s, std::string t) {
  return this->followsTable.hasForwardRelation(s, t);
}

std::set<std::string> PKB::getFollows(std::string s) {
  return this->followsTable.getForwardRelations(s);
}

std::set<std::string> PKB::getFollowedBy(std::string s) {
  return this->followsTable.getReverseRelations(s);
}

std::vector<std::vector<std::string>> PKB::getFollowsTable() {
  return this->followsTable.getTable();
}

bool PKB::followsT(std::string s, std::string t) {
  return this->followsTTable.hasForwardRelation(s, t);
}

std::set<std::string> PKB::getFollowsT(std::string s) {
  return this->followsTTable.getForwardRelations(s);
}

std::set<std::string> PKB::getFollowedByT(std::string s) {
  return this->followsTTable.getReverseRelations(s);
}

std::vector<std::vector<std::string>> PKB::getFollowsTTable() {
  return this->followsTTable.getTable();
}

bool PKB::parent(std::string s, std::string t) {
  return this->parentTable.hasForwardRelation(s, t);
}

std::set<std::string> PKB::getParent(std::string s) {
  return this->parentTable.getForwardRelations(s);
}

std::set<std::string> PKB::getParentOf(std::string s) {
  return this->parentTable.getReverseRelations(s);
}

std::vector<std::vector<std::string>> PKB::getParentTable() {
  return this->parentTable.getTable();
}

bool PKB::parentT(std::string s, std::string t) {
  return this->parentTTable.hasForwardRelation(s, t);
}

std::set<std::string> PKB::getParentT(std::string s) {
  return this->parentTTable.getForwardRelations(s);
}

std::set<std::string> PKB::getParentOfT(std::string s) {
  return this->parentTTable.getReverseRelations(s);
}

std::vector<std::vector<std::string>> PKB::getParentTTable() {
  return this->parentTTable.getTable();
}

bool PKB::uses(std::string s, std::string t) {
  return this->usesTable.hasForwardRelation(s, t);
}

std::set<std::string> PKB::getUses(std::string s) {
  return this->usesTable.getForwardRelations(s);
}

std::set<std::string> PKB::getUsedBy(std::string s) {
  return this->usesTable.getReverseRelations(s);
}

bool PKB::modifies(std::string s, std::string t) {
  return this->modifiesTable.hasForwardRelation(s, t);
}

std::set<std::string> PKB::getModifies(std::string s) {
  return this->modifiesTable.getForwardRelations(s);
}

std::set<std::string> PKB::getModifiedBy(std::string s) {
  return this->modifiesTable.getReverseRelations(s);
}

bool PKB::matchAssign(std::string stmtNo, std::string var, std::string expr,
                      bool partial) {
  if (assignTable.find(stmtNo) == assignTable.end()) {
    return false;
  } else {
    return (var == "" || assignTable[stmtNo].first == var) &&
           ((!partial && assignTable[stmtNo].second == expr) ||
            (partial &&
             assignTable[stmtNo].second.find(expr) != std::string::npos));
  }
}

std::set<std::string> PKB::getAssignMatches(std::string expr, std::string var,
                                            bool partial) {
  std::set<std::string> results;
  for (auto it = assignTable.begin(); it != assignTable.end(); ++it) {
    if ((var == "" || it->second.first == var) &&
        ((!partial && it->second.second == expr) ||
         (partial && it->second.second.find(expr) != std::string::npos))) {
      results.insert(it->first);
    }
  }
  return results;
}
