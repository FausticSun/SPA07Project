#pragma once

#include "Relation.h"
#include <datetimeapi.h>
#include <map>
#include <set>
#include <string>

enum class StatementType { Stmt, Assign, If, While, Read, Call, Print };

class PKB {
private:
  int stmtCount;
  std::set<std::string> varTable;
  std::set<std::string> procTable;
  std::map<StatementType, std::set<std::string>> stmtTable;
  std::map<std::string, std::set<std::string>> followsTable;
  std::map<std::string, std::set<std::string>> followedByTable;
  std::map<std::string, std::set<std::string>> followsStarTable;
  std::map<std::string, std::set<std::string>> followedByStarTable;
  std::map<std::string, std::set<std::string>> parentTable;
  std::map<std::string, std::set<std::string>> parentOfTable;
  std::map<std::string, std::set<std::string>> parentStarTable;
  std::map<std::string, std::set<std::string>> parentOfStarTable;
  std::map<std::string, std::set<std::string>> usesTable;
  std::map<std::string, std::set<std::string>> usedByTable;
  std::map<std::string, std::set<std::string>> modifiesTable;
  std::map<std::string, std::set<std::string>> modifiedByTable;
  std::map<std::string, std::string> assignTable;

public:
  PKB();
  void insertVar(std::string);
  void insertProc(std::string);
  void insertStatement(std::string, StatementType);
  void setFollows(std::string, std::string);
  void setParent(std::string, std::string);
  void setUses(std::string, std::string);
  void setModifies(std::string, std::string);

  const std::set<std::string> &getVarTable() const;
  const std::set<std::string> &getProcTable() const;
  const std::set<std::string> &getStatementsOfType(StatementType) const;
  StatementType getStatementType(std::string);
  bool follows(std::string, std::string);
  std::set<std::string> getFollows(std::string);
  std::set<std::string> getFollowedBy(std::string);
  bool followsT(std::string, std::string);
  std::set<std::string> getFollowsT(std::string);
  std::set<std::string> getFollowedByT(std::string);
  bool parent(std::string, std::string);
  std::set<std::string> getParent(std::string);
  std::set<std::string> getParentOf(std::string);
  bool parentT(std::string, std::string);
  std::set<std::string> getParentT(std::string);
  std::set<std::string> getParentOfT(std::string);
  bool uses(std::string, std::string);
  std::set<std::string> getUses(std::string);
  std::set<std::string> getUsedBy(std::string);
  bool modifies(std::string, std::string);
  std::set<std::string> getModifies(std::string);
  std::set<std::string> getModifiedBy(std::string);
  bool matchAssign(std::string stmtNo, std::string expr, bool partial);
  std::set<std::string> getAssignMatches(std::string expr, bool partial);
};
