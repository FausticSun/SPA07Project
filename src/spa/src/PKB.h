#pragma once

#include "Relation.h"
#include <set>
#include <string>
#include <vector>
#include <unordered_set>
#include <queue>
#include <Token.h>

using namespace std;
typedef short PROC;

class TNode;

class VarTable {  // no need to #include "VarTable.h" as all I need is pointer
public: 
	VarTable();
	VarTable* buildVarTable(queue<Token*> tokens);
	bool contains(string s);
	unordered_set<string> getVarList();
 	string toString();
	int add(string s); //for testing

private:
	unordered_set<string> varList;
	int numOfVars;
};

class ProcTable {
public:
	ProcTable();
	ProcTable* buildProcTable(queue<Token*> tokens);
	bool contains(string s);
	unordered_set<string> getProcList();
	string toString();

private:
	unordered_set<string> procList;
	int numOfProcs;
	int add(string s);
};


class PKB {
private:
  std::set<std::string> varTable;
  std::set<std::string> procTable;
  std::set<Relation> relTable;

public:
  PKB();
  void setPKB(const std::set<std::string> &variableTable = {},
              const std::set<std::string> &procedureTable = {},
              const std::set<Relation> &relationTable = {});
  const std::set<std::string> getVarTable() const;
  const std::set<std::string> getProcTable() const;
  const std::set<Relation> getRelTable() const;
};
