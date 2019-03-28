#pragma once
#include "Table.h"
#include <map>

class CFG {

	enum class TraverseType{ ConstantNext, ForwardNext, ReverseNext };

private:
  // Map from original line number to compressed node index
  std::map<int, int> initialToCompressed;
  // Map from compressed node index to original line number
  std::map<int, std::vector<int>> compressedToInitial;
  std::vector<std::vector<int>> initialGraph;
  std::vector<std::vector<int>> forwardCompressedGraph;
  std::vector<std::vector<int>> reverseCompressedGraph;
  int numCompressedNodes = 0;

  void populateInitialToCompressed(int, Table, std::vector<int>);
  void populateCompressedToInitial();
  void populateCompressedGraph(Table);
  std::vector<int> traverseForwardCFG(int, int) const;
  std::vector<int> traverseReverseCFG(int) const;

public:
  CFG();
  CFG(Table, Table, Table, int);
  Table getNextT() const;
  Table getNextT(int, bool) const;
  bool getNextT(int, int) const;
};
