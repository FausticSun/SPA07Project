#pragma once
#include "Table.h"
#include <map>

enum class TraverseType { ConstantNext, ForwardNext, ReverseNext };

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

  void populateInitialToCompressed(int, Table, std::vector<int>);
  void populateCompressedToInitial();
  void populateCompressedGraph(Table);

  std::vector<int> getNextTForward(int, int) const;
  std::vector<int> getNextTReverse(int) const;
  std::vector<int> getAffectsForward(int, std::string, Table, Table) const;
  std::vector<int> getAffectsReverse(int, std::string, Table, Table) const;

public:
  CFG();
  CFG(Table, Table, Table, int);
  Table getNextT() const;
  bool isNextT(int, int) const;
  Table getNextT(int, bool) const;
  Table getAffects() const;
  bool isAffects(int, int, Table, Table) const;
  Table getAffects(int, bool, Table, Table, std::set<int>) const;

};
