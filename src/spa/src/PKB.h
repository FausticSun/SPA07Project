#pragma once

#include "RelationTable.h"
#include <map>
#include <set>
#include <string>
#include <vector>

enum class StatementType {
  Stmt,
  Assign,
  If,
  While,
  Read,
  Call,
  Print,
  Invalid
};

class PKB {
private:
  int stmtCount;
  std::set<std::string> varTable;
  std::set<std::string> procTable;
  std::set<std::string> constTable;
  std::map<StatementType, std::set<std::string>> stmtTable;
  RelationTable followsTable;
  RelationTable followsTTable;
  RelationTable parentTable;
  RelationTable parentTTable;
  RelationTable usesTable;
  RelationTable modifiesTable;
  std::map<std::string, std::pair<std::string, std::string>> assignTable;

public:
  void insertVar(const std::string &);
  void insertProc(const std::string &);
  void insertStatement(const std::string &, StatementType);
  void insertConstant(int);
  void setFollows(int, int);
  void setFollowsT(int, int);
  void setParent(int, int);
  void setParentT(int, int);
  void setUses(int, std::string);
  void setModifies(int, std::string);
  const std::set<std::string> &getVarTable() const;
  const std::set<std::string> &getProcTable() const;
  const std::set<std::string> &getConstTable() const;
  bool isVar(std::string);
  bool isProc(std::string);
  const std::set<std::string> &getStatementsOfType(StatementType) const;
  StatementType getStatementType(std::string);
  bool follows(std::string, std::string);
  std::set<std::string> getFollows(std::string);
  std::set<std::string> getFollowedBy(std::string);
  std::vector<std::vector<std::string>> getFollowsTable();
  bool followsT(std::string, std::string);
  std::set<std::string> getFollowsT(std::string);
  std::set<std::string> getFollowedByT(std::string);
  std::vector<std::vector<std::string>> getFollowsTTable();
  bool parent(std::string, std::string);
  std::set<std::string> getParent(std::string);
  std::set<std::string> getParentOf(std::string);
  std::vector<std::vector<std::string>> getParentTable();
  bool parentT(std::string, std::string);
  std::set<std::string> getParentT(std::string);
  std::set<std::string> getParentOfT(std::string);
  std::vector<std::vector<std::string>> getParentTTable();
  bool uses(std::string, std::string);
  std::set<std::string> getUses(std::string);
  std::set<std::string> getUsedBy(std::string);
  bool modifies(std::string, std::string);
  std::set<std::string> getModifies(std::string);
  std::set<std::string> getModifiedBy(std::string);
  bool matchAssign(std::string stmtNo, std::string var, std::string expr,
                   bool partial);
  std::set<std::string> getAssignMatches(std::string var, std::string expr,
                                         bool partial);
};
