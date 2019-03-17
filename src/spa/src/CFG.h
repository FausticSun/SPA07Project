#pragma once
#include "Table.h"
#include <map>

class CFG {
private:
  std::vector<std::vector<int>> initialGraph;
  std::vector<std::vector<int>> forwardCompressedGraph;
  std::vector<std::vector<int>> reverseCompressedGraph;
  std::map<int, int> referenceMap;
  int numCompressedNodes = 0;
  void populateReferenceMap(Table, std::vector<int>);
  void populateCompressedGraph();

public:
  CFG();
  CFG(int, Table, Table);
  std::vector<std::vector<int>> getForwardCompressedGraph();
  std::vector<std::vector<int>> getReverseCompressedGraph();
};
