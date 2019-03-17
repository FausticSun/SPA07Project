#pragma once
#include "Table.h"
#include <map>

class CFG {
private:
  std::vector<std::vector<int>> initialGraph;
  std::vector<std::vector<int>> compressedGraph;
  std::map<int, int> referenceMap;
  int numCompressedNodes = 0;
  void populateReferenceMap(Table, std::vector<int>);
  void populateCompressedGraph();

public:
  CFG(int, Table, Table);
  std::vector<std::vector<int>> getCompressedGraph();
};
