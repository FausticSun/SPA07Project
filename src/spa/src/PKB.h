#pragma once
#include "CFG.h"
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

  // General entities
  int stmtCount = 0;
  Table varTable{1};
  std::map<std::string, StmtRange> procTable;
  Table constTable{1};
  std::map<StatementType, std::set<int>> stmtTable;
  CFG cfg;

  // PQL Relation Tables
  Table followsTable{2};
  Table followsTTable{2};
  Table parentTable{2};
  Table parentTTable{2};
  Table usesPTable{2};
  Table usesSTable{2};
  Table modifiesPTable{2};
  Table modifiesSTable{2};
  Table callsTable{2};
  Table callsTTable{2};
  Table nextTable{2};

  // Other Relation Tables
  Table callProcNameTable{2};

  // Pattern Tables
  std::map<int, std::pair<std::string, std::string>> assignTable;
  Table whileTable{2};
  Table ifTable{2};

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
  void setIf(int, const std::string &);
  void setWhile(int, const std::string &);
  // Other setters
  void setCallProcName(int, const std::string &);
  void setCFG(CFG &);

  // Getters
  // Entity getter
  Table getVarTable() const;
  Table getProcTable() const;
  Table getConstTable() const;
  Table getStmtType(StatementType);
  Table getProcStmt();
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
  Table getCallProcNameTable() const;
  // Pattern getter
  Table getAssignMatches(std::string expr, bool partial);
  Table getWhileMatches();
  Table getIfMatches();
  // Other getters
  Table getCallProcName();
  CFG getCFG();
  int getStmtCount();
};
