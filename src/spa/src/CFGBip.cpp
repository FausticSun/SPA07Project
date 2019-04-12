#include "CFGBip.h"

CFGBip::CFGBip() {}

CFGBip::CFGBip(Table callTable, Table nextTable, Table procTable,
               Table callProcNameTable, Table procExitStmtTable,
               int stmtCount) {
  populateNextBip(callTable, nextTable, procTable, callProcNameTable,
                  procExitStmtTable);
  populateNextBipT(stmtCount);
}

void CFGBip::populateNextBip(Table callTable, Table nextTable, Table procTable,
                             Table callProcNameTable, Table procExitStmtTable) {
  std::map<std::string, std::vector<int>> procMap;
  for (auto data : procTable.getData()) {
    procMap.insert(std::make_pair(
        data[0], std::vector<int>{std::stoi(data[1]), std::stoi(data[2])}));
  }

  for (auto data : callProcNameTable.getData()) {
    callProcNameMap.insert(std::make_pair(std::stoi(data[0]), data[1]));
  }

  for (auto data : procExitStmtTable.getData()) {
    if (procExitStmtMap.count(data[0]) == 1) {
      procExitStmtMap[data[0]].push_back(std::stoi(data[1]));
    } else {
      procExitStmtMap.insert(
          std::make_pair(data[0], std::vector<int>{std::stoi(data[1])}));
    }
  }

  std::map<int, int> callNext;

  for (auto data : nextTable.getData()) {
    // not a call statement
    if (!callTable.contains({data[0]})) {
      controlFlowLink.push_back(
          std::vector<int>{std::stoi(data[0]), std::stoi(data[1])});
      nextBip.insertRow(data);
    } else {
      // store next for call statements
      callNext.insert(
          std::pair<int, int>(std::stoi(data[0]), std::stoi(data[1])));
    }
  }

  for (auto data : callProcNameTable.getData()) {
    // create branch in for all calls
    int stmt = std::stoi(data[0]);
    std::string procedure = data[1];
    nextBip.insertRow(
        {std::to_string(stmt), std::to_string(procMap[procedure][0])});
    branchIn.push_back(std::vector<int>{stmt, procMap[procedure][0]});
    // create dummy node for call statements at exit stmt of procedure
    for (auto i : procExitStmtTable.getData()) {
      if (i[1] == std::to_string(stmt)) {
        callNext.insert(std::pair<int, int>(stmt, stmt * -1));
      }
    }
  }

  std::vector<int> uncheckedExits;
  std::map<int, std::vector<int>> tempBranchbacks;

  for (auto i : callNext) {
    // create branch back
    int curr = i.first;
    int next = i.second;
    std::string procedure = callProcNameMap[curr];
    for (int exit : procExitStmtMap[procedure]) {
      if ((next > 0) && (!callTable.contains({std::to_string(exit)}))) {
        nextBip.insertRow({std::to_string(exit), std::to_string(next)});
        tempBranchbacks.insert(
            std::make_pair(exit, std::vector<int>({next, curr})));
        branchBack.push_back(std::vector<int>{exit, next, curr});
      }

      if (callTable.contains({std::to_string(exit)})) {
        tempBranchbacks.insert(
            std::make_pair(exit * -1, std::vector<int>({next, curr})));
      }

      if ((next < 0) && (!callTable.contains({std::to_string(exit)}))) {
        tempBranchbacks.insert(
            std::make_pair(exit, std::vector<int>({next, curr})));
        uncheckedExits.push_back(exit);
      }
    }
  }

  for (int exit : uncheckedExits) {
    int bb = tempBranchbacks[exit][0];
    int prev = tempBranchbacks[exit][1];
    std::vector<int> temp = {exit, prev};
    while ((bb < 0) && (tempBranchbacks.count(bb) == 1)) {
      prev = tempBranchbacks[bb][1];
      bb = tempBranchbacks[bb][0];
      temp.push_back(prev);
    }

    if (bb > 0) {
      nextBip.insertRow({std::to_string(exit), std::to_string(bb)});
      auto it = temp.insert(temp.begin() + 1, bb);
      branchBack.push_back(temp);
    }
  }
}

void CFGBip::populateNextBipT(int stmtCount) {
  // create adjacency list
  adjLst.resize(stmtCount + 1);
  for (auto data : controlFlowLink) {
    Link l;
    l.type = LinkType::CFLink;
    l.v = data[1];
    adjLst[data[0]].push_back(l);
  }

  for (auto data : branchIn) {
    Link l;
    l.type = LinkType::BranchIn;
    l.v = data[1];
    adjLst[data[0]].push_back(l);
  }

  for (auto data : branchBack) {
    Link l;
    l.type = LinkType::BranchBack;
    l.v = data[1];
    for (int i = 2; i < data.size(); i++) {
      l.sources.insert(data[i]);
    }
    adjLst[data[0]].push_back(l);
  }

  // BFS from every statement number
  for (int i = 1; i < stmtCount + 1; i++) {
    std::vector<bool> visited(stmtCount + 1, false);
    visited[0] = true;

    std::queue<int> q;
    std::stack<int> branches;
    q.push(i);
    while (!q.empty()) {
      int curr = q.front();
      q.pop();
      for (Link j : adjLst[curr]) {
        int v = j.v;
        LinkType type = j.type;
        if (type == LinkType::CFLink) {
          if (!visited[v]) {
            visited[v] = true;
            q.push(v);
            nextBipT.insertRow({std::to_string(i), std::to_string(v)});
          }
        } else if (type == LinkType::BranchIn) {
          if (!visited[v]) {
            visited[v] = true;
            q.push(v);
            for (int k = 0; k < procExitStmtMap[callProcNameMap[curr]].size();
                 k++) {
              branches.push(curr);
            }
            nextBipT.insertRow({std::to_string(i), std::to_string(v)});
          } else {
            for (auto k : procExitStmtMap[callProcNameMap[curr]]) {
              q.push(k);
              branches.push(curr);
            }
          }
        } else if (type == LinkType::BranchBack) {
          if (branches.empty()) {
            visited[v] = true;
            q.push(v);
            nextBipT.insertRow({std::to_string(i), std::to_string(v)});
          } else {
            if (j.sources.count(branches.top()) == 1) {
              branches.pop();
              visited[v] = true;
              q.push(v);
              nextBipT.insertRow({std::to_string(i), std::to_string(v)});
              break;
            }
          }
        }
      }
    }
  }
}

Table CFGBip::getNextBip() { return nextBip; }

Table CFGBip::getNextBipT() { return nextBipT; }
