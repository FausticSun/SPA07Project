#pragma once
#include "Table.h"
#include <map>

class CFG {
private:
  std::vector<std::vector<int>> initialGraph;
  std::vector<std::vector<int>> forwardCompressedGraph;
  std::vector<std::vector<int>> reverseCompressedGraph;
  // Map from original line number to compressed node index
  std::map<int, int> initialToCompressed;
  // Map from compressed node index to original line number
  std::map<int, std::vector<int>> compressedToInitial;
  int numCompressedNodes = 0;
  void populateInitialToCompressed(Table, std::vector<int>);
  void populateCompressedToInitial();
  void populateCompressedGraph();

public:
  CFG();
  CFG(int, Table, Table);
  std::vector<std::vector<int>> getForwardCompressedGraph();
  std::vector<std::vector<int>> getReverseCompressedGraph();
  std::vector<int> getInitial(int c);
  int getCompressed(int i);
};
