#pragma once
#include "Table.h"
#include "Util.h"
#include <deque>
#include <map>

class CFG {
private:
  // CFG information
  std::map<int, int> initialToCompressed;
  std::map<int, std::vector<int>> compressedToInitial;
  std::vector<std::vector<int>> initialGraph;
  std::vector<std::vector<int>> forwardCompressedGraph;
  std::vector<std::vector<int>> reverseCompressedGraph;
  int numCompressedNodes = 0;

  // Cache
  std::map<int, std::deque<int>> affectsForwardCache;
  std::map<std::pair<int, std::string>, std::deque<int>> affectsReverseCache;
  Table affectsTCache{1};
  std::map<int, Table> whileBlockCache;

  // Other information
  Table procStmtTable{1};
  Table modifiesTable{2};
  Table usesTable{2};
  Table modifiesAssignTable{2};
  Table usesAssignTable{2};
  Table assignTable{1};
  Table whileIfTable{1};
  Table whileParentTable{2};
  std::map<int, std::pair<std::string, std::vector<std::string>>> assignMap;
  std::map<int, StatementType> stmtMap;
  std::map<int, std::vector<int>> assignWhileMap;
  std::vector<int> inDegree;

  // Methods to get CFG information
  void populateInitialToCompressed(int, Table, std::vector<int>);
  void populateCompressedToInitial();
  void populateCompressedGraph(Table);

  // Methods for traversal to retrieve Next* relations
  std::deque<int> getNextTForward(int, int);
  std::deque<int> getNextTReverse(int);

  // Methods for traversal to retrieve Affects relations
  std::deque<int> getAffectsForward(int, std::string);
  std::deque<int> getAffectsReverse(int, std::string);

public:
  CFG();
  CFG(Table procStmtTable, Table nextTable, Table modifiesTable,
      Table usesTable, Table whileIfTable, Table whileParentTable,
      Table assignTable, int stmtCount);

  // Getters for Next*
  bool isNextT(int, int);
  Table getNextT();
  Table getNextT(int, bool);

  // Getters for Affects
  bool isAffects(int, int);
  Table getAffects();
  Table getAffects(int, bool);

  // Getter for Affects*
  Table getAffectsT();

  void clearCache();
};
