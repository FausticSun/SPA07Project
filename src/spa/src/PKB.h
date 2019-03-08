#pragma once
#include "Table.h"
#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>

enum class StatementType { Stmt, Assign, If, While, Read, Call, Print };

class PKB {
private:
  // Range of statements from first (inclusive) and last (exclusive)
  typedef std::pair<int, int> StmtRange;
  typedef std::set<std::pair<std::string, std::string>> EdgeList;

  int stmtCount = 0;
  std::set<std::string> varTable;
  std::map<std::string, StmtRange> procTable;
  std::set<int> constTable;
  std::map<StatementType, std::set<int>> stmtTable;
  EdgeList followsTable;
  EdgeList followsTTable;
  EdgeList parentTable;
  EdgeList parentTTable;
  EdgeList usesPTable;
  EdgeList usesSTable;
  EdgeList modifiesPTable;
  EdgeList modifiesSTable;
  EdgeList callsTable;
  EdgeList callsTTable;
  EdgeList nextTable;
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
  void setUsesP(int, std::string);
  void setUsesS(std::string, std::string);
  void setModifiesP(int, std::string);
  void setModifiesS(std::string, std::string);
  void setCalls(std::string, std::string);
  void setCallsT(std::string, std::string);
  void setNext(int, int);
  // Pattern setters
  void setAssign(int, std::string &, std::string &);
  void setIf(int, std::string &);
  void setWhile(int, std::string &);

  // Getters
  // Entity getter
  Table getVarTable() const;
  Table getProcTable() const;
  Table getConstTable() const;
  Table getStmtType(StatementType);
  // Relation getter
  Table getFollows() const;
  Table getFollowsT() const;
  Table getParent() const;
  Table getParentT() const;
  Table getUsesP() const;
  Table getUsesS() const;
  Table getModifiesP() const;
  Table getModifiesS() const;
  Table getCalls() const;
  Table getCallsT() const;
  Table getNext() const;
  // Table getNextT() const;
  // Table getAffects() const;
  // Table getAffectsT() const;
  // Pattern getter
  Table getAssignMatches(std::string var, std::string expr, bool partial);
  Table getWhileMatches(std::string var);
  Table getIfMatches(std::string var);
};
