#pragma once
#include "Table.h"
#include "Util.h"
#include <list>
#include <map>
#include <deque>

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
  std::map<int, std::deque<int>> affectsReverseCache;

  // Other information
  Table whileIfTable{1};
  Table procStmtTable{1};
  std::map<int, std::set<int>> whileParentMap;
  std::vector<int> inDegree;

  // Methods to get CFG information
  void populateInitialToCompressed(int, Table, std::vector<int>);
  void populateCompressedToInitial();
  void populateCompressedGraph(Table);

  // Methods for traversal to retrieve Next* relations
  std::deque<int> getNextTForward(int, int);
  std::deque<int> getNextTReverse(int);

  // Methods for traversal to retrieve Affects relations
  std::deque<int> getAffectsForward(int, std::string, Table, Table);
  std::deque<int> getAffectsReverse(int, std::string, Table, Table);

  // Method for traversal to retrieve Affects* relations
  std::map<int, std::set<int>> getAffectsTResults(
      int, Table, Table, std::map<int, StatementType>,
      std::map<int, std::pair<std::string, std::vector<std::string>>>);

public:
  CFG();
  CFG(Table, Table, Table, Table, int);

  // Getters for Next*
  bool isNextT(int, int);
  Table getNextT();
  Table getNextT(int, bool);

  // Getters for Affects
  bool isAffects(int, int, Table, Table);
  Table getAffects(Table, Table, std::set<int>);
  Table getAffects(int, bool, Table, Table, std::set<int>);

  // Getter for Affects*
  Table getAffectsT(
      Table, Table, std::map<int, StatementType>,
      std::map<int, std::pair<std::string, std::vector<std::string>>>);

  void clearCache();
};
