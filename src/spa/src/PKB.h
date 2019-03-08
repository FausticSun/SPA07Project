#pragma once

#include "RelationTable.h"
#include <map>
#include <set>
#include <string>
#include <utility>
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
  // Range of statements from first (inclusive) and last (exclusive)
  typedef std::pair<int, int> StmtRange;

  int stmtCount = 0;
  std::set<std::string> varTable;
  std::map<std::string, StmtRange> procTable;
  std::set<int> constTable;
  std::map<StatementType, std::set<int>> stmtTable;
  RelationTable followsTable;
  RelationTable followsTTable;
  RelationTable parentTable;
  RelationTable parentTTable;
  RelationTable usesTable;
  RelationTable modifiesTable;
  RelationTable callsTable;
  RelationTable callsTTable;
  RelationTable nextTable;
  std::map<std::string, std::pair<std::string, std::string>> assignTable;

public:
  // Setters
  // Entity setters
  void setVar(const std::string &);
  void setProc(const std::string &, int, int);
  void setStmtType(int, StatementType);
  void setConst(int);
  // Relation setters
  void setFollows(int, int);
  void setFollowsT(int, int);
  void setParent(int, int);
  void setParentT(int, int);
  void setUses(int, std::string);
  void setUses(std::string, std::string);
  void setModifies(int, std::string);
  void setModifies(std::string, std::string);
  void setCalls(std::string, std::string);
  void setCallsT(std::string, std::string);
  void setNext(int, int);
  // Pattern setters
  void setAssign(int, std::string &, std::string &);
  void setIf(int, std::string &);
  void setWhile(int, std::string &);

  const std::set<std::string> getVarTable() const;
  const std::set<std::string> getProcTable() const;
  const std::set<std::string> getConstTable() const;
  bool isVar(std::string);
  bool isProc(std::string);
  int getStatementCount();
  const std::set<std::string> getStatementsOfType(StatementType) const;
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
