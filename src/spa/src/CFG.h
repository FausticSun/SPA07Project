#pragma once
#include "Table.h"
#include "Util.h"
#include <map>
#include <list>

class CFG {
private:
  // Map from original line number to compressed node index
  std::map<int, int> initialToCompressed;
  // Map from compressed node index to original line number
  std::map<int, std::vector<int>> compressedToInitial;
  std::vector<std::vector<int>> initialGraph;
  std::vector<std::vector<int>> forwardCompressedGraph;
  std::vector<std::vector<int>> reverseCompressedGraph;
  int numCompressedNodes = 0;

  Table whileIfTable{1};
  Table procStmtTable{1};
  std::map<int, std::set<int>> whileParentMap;
  std::vector<int> inDegree;

  void populateInitialToCompressed(int, Table, std::vector<int>);
  void populateCompressedToInitial();
  void populateCompressedGraph(Table);

  std::list<int> getNextTForward(int, int) const;
  std::list<int> getNextTReverse(int) const;
  std::list<int> getAffectsForward(int, std::string, Table, Table) const;
  std::list<int> getAffectsReverse(int, std::string, Table, Table) const;

  std::map<int, std::set<int>> getAffectsTResults(
      int, Table, Table, std::map<int, StatementType>,
      std::map<int, std::pair<std::string, std::vector<std::string>>>) const;

public:
  CFG();
  CFG(Table, Table, Table, Table, int);
  bool isNextT(int, int) const;
  Table getNextT() const;
  Table getNextT(int, bool) const;
  bool isAffects(int, int, Table, Table) const;
  Table getAffects(Table, Table, std::set<int>) const;
  Table getAffects(int, bool, Table, Table, std::set<int>) const;
  bool isAffectsT(
      int, int, Table, Table, std::map<int, StatementType>,
      std::map<int, std::pair<std::string, std::vector<std::string>>>) const;
  Table getAffectsT(
      Table, Table, std::map<int, StatementType>,
      std::map<int, std::pair<std::string, std::vector<std::string>>>) const;
  Table getAffectsT(
      int, bool, Table, Table, std::map<int, StatementType>,
      std::map<int, std::pair<std::string, std::vector<std::string>>>) const;
};
