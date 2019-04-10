#pragma once
#include "Table.h"
#include <map>
#include <queue>
#include <set>
#include <stack>

class CFGBip {
  enum class LinkType { CFLink, BranchIn, BranchBack };

  struct Link {
    LinkType type;
    int v;
    std::set<int> sources;
  };

private:
  Table nextBip{2};
  Table nextBipT{2};
  std::vector<std::vector<int>> controlFlowLink;
  std::vector<std::vector<int>> branchIn;
  std::vector<std::vector<int>> branchBack;
  std::vector<std::vector<Link>> adjLst;
  std::map<int, std::string> callProcNameMap;
  std::map<std::string, std::vector<int>> procExitStmtMap;

  void populateNextBip(Table, Table, Table, Table, Table);
  void populateNextBipT(int);

public:
  CFGBip();
  CFGBip(Table, Table, Table, Table, Table, int);
  Table getNextBip();
  Table getNextBipT();
};
