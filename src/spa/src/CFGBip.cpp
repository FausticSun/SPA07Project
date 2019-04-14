#include "CFGBip.h"

CFGBip::CFGBip() {}

CFGBip::CFGBip(Table callTable, Table nextTable, Table procTable,
               Table callProcNameTable, Table procExitStmtTable, int stmtCount,
               Table usesTable, Table modifiesTable, Table assignTable,
               Table whileIfTable)
    : modifiesTable(modifiesTable), whileIfTable(whileIfTable),
      stmtCount(stmtCount) {
  modifiesTable = modifiesTable;
  modifiesAssignTable = modifiesTable;
  modifiesAssignTable.mergeWith(assignTable);
  usesAssignTable = usesTable;
  usesAssignTable.mergeWith(assignTable);
  buildCFGBip(callTable, nextTable, procTable, callProcNameTable,
              procExitStmtTable);
  buildAdjacencyLists();
  populateNextBip();
}

void CFGBip::buildCFGBip(Table callTable, Table nextTable, Table procTable,
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

  for (auto data : callTable.getData()) {
    callSet.insert(std::stoi(data[0]));
  }

  std::map<int, int> callNext;

  for (auto data : nextTable.getData()) {
    // not a call statement
    if (!isCallStatement(std::stoi(data[0]))) {
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
      if (i[1] == std::to_string(stmt)) { // call is exit stmt
        callNext.insert(std::pair<int, int>(stmt, stmt * -1));
        controlFlowLink.push_back(std::vector<int>{stmt, stmt * -1});
      }
    }
  }

  // create branch backs
  for (auto i : callNext) {
    int curr = i.first;
    int next = i.second;
    std::string procedure = callProcNameMap[curr];
    for (int exit : procExitStmtMap[procedure]) {
      if (!isCallStatement(exit)) {
        branchBack.push_back(std::vector<int>{exit, next, curr});
      } else {
        branchBack.push_back(std::vector<int>{exit * -1, next, curr});
      }
    }
  }
}

void CFGBip::buildAdjacencyLists() {

  // building adjacency lists (map)
  for (auto data : controlFlowLink) {
    Link l;
    l.type = LinkType::CFLink;
    l.v = data[1];
    addToGenAdjLst(data[0], l);
    addToSimpleAdjLst(data[0], l.v);
  }

  for (auto data : branchIn) {
    Link l;
    l.type = LinkType::BranchIn;
    l.v = data[1];
    addToGenAdjLst(data[0], l);
    addToSimpleAdjLst(data[0], l.v);
  }

  for (auto data : branchBack) {
    Link l;
    l.type = LinkType::BranchBack;
    l.v = data[1];
    l.source = data[2];
    addToGenAdjLst(data[0], l);
    addToSimpleAdjLst(data[0], l.v);
  }
}

void CFGBip::addToGenAdjLst(int stmt, Link l) {
  if (generalizedAdjLst.count(stmt) == 1) {
    generalizedAdjLst[stmt].push_back(l);
  } else {
    generalizedAdjLst[stmt] = std::vector<Link>{l};
  }
}

void CFGBip::addToSimpleAdjLst(int stmt1, int stmt2) {
  if (simpleAdjLst.count(stmt1) == 1) {
    simpleAdjLst[stmt1].push_back(stmt2);
  } else {
    simpleAdjLst[stmt1] = std::vector<int>{stmt2};
  }
}

void CFGBip::populateNextBip() {
  for (int i = 1; i < stmtCount + 1; i++) {
    std::map<int, bool> visited;
    for (auto i : generalizedAdjLst) {
      visited[i.first] = false;
    }
    std::queue<int> q;
    q.push(i);

    while (!q.empty()) {
      int curr = q.front();
      q.pop();
      for (Link l : generalizedAdjLst[curr]) {
        int v = l.v;
        if (!visited[v]) {
          if ((v < 0) &&
              (l.type ==
               LinkType::BranchBack)) { // connected to dummy via branch back
            visited[v] = true;
            q.push(v);
          } else if (v > 0) {
            visited[v] = true;
            nextBip.insertRow({std::to_string(i), std::to_string(v)});
          }
        }
      }
    }
  }
}

void CFGBip::populateNextBipT() {

  // BFS from every statement number
  for (int i = 1; i < stmtCount + 1; i++) {

    std::map<int, bool> visited;
    for (auto i : generalizedAdjLst) {
      visited[i.first] = false;
    }

    visited[0] = true;

    std::queue<int> q;
    std::stack<int> branches;
    q.push(i);

    while (!q.empty()) {
      int curr = q.front();
      q.pop();
      for (Link j : generalizedAdjLst[curr]) {
        int v = j.v;
        LinkType type = j.type;
        if (type == LinkType::CFLink) {
          if (!visited[v]) {
            visited[v] = true;
            q.push(v);
            if (v > 0) {
              nextBipT.insertRow({std::to_string(i), std::to_string(v)});
            }
          }
        } else if (type == LinkType::BranchIn) {
          if (!visited[v]) {
            visited[v] = true;
            q.push(v);
            for (int k = 0; k < procExitStmtMap[callProcNameMap[curr]].size();
                 k++) { // push one in for every exit stmt
              branches.push(curr);
            }
            nextBipT.insertRow({std::to_string(i), std::to_string(v)});
          } else {
            for (auto k :
                 procExitStmtMap[callProcNameMap[curr]]) { // already visited,
                                                           // just enqueue exit
                                                           // stmts
              if (isCallStatement(k)) {
                q.push(k * -1);
              } else {
                q.push(k);
              }
              branches.push(curr);
            }
          }
        } else if (type == LinkType::BranchBack) {
          if (branches.empty()) {
            visited[v] = true;
            q.push(v);
            if (v > 0) {
              nextBipT.insertRow({std::to_string(i), std::to_string(v)});
            }
          } else {
            if (j.source == branches.top()) {
              branches.pop();
              visited[v] = true;
              q.push(v);
              if (v > 0) {
                nextBipT.insertRow({std::to_string(i), std::to_string(v)});
              }
              break;
            }
          }
        }
      }
    }
  }
}


void CFGBip::populateAffectsBip(bool isAffectsBip) {
	for (auto data : modifiesAssignTable.getData()) {
		auto result = getAffectsForward(std::stoi(data[0]), data[1]);
		for (int i : result) {
			if (isAffectsBip) {
				affectsBip.insertRow({ data[0], std::to_string(i) });
			}
			else {
				affectsBipT.insertRow({ data[0], std::to_string(i) });
			}
		}
	}
}

std::deque<int> CFGBip::getAffectsForward(int i, std::string var) {

  std::deque<int> results;
  std::stack<int> branches;
  // initialize visited
  std::map<int, bool> visited;
  for (auto i : generalizedAdjLst) {
    visited[i.first] = false;
  }
  visited[0] = true;

  std::queue<int> q;
  q.push(i);
  while (!q.empty()) {
    int curr = q.front();
    q.pop();
    // looping through neighbours
    for (Link l : generalizedAdjLst[curr]) {
      int v = l.v;
      bool isModified = false; // keep track if statement modifies
      if (l.type == LinkType::CFLink) {
        if (!visited[v]) {
          visited[v] = true;
          isModified = checkUsesModifies(v, var, results);
          if (!isModified) {
            q.push(v);
          }
        }
      } else if (l.type == LinkType::BranchIn) {
        if (!visited[v]) {
          visited[v] = true;
          isModified = checkUsesModifies(v, var, results);
          if (!isModified) {
            for (int k = 0; k < procExitStmtMap[callProcNameMap[curr]].size();
                 k++) {            // push one in for every exit stmt
              branches.push(curr); // curr is a call
            }
            q.push(v);
          }
        } else {
          isModified = checkUsesModifies(v, var, results);
          if (!isModified) {
            for (auto k :
                 procExitStmtMap[callProcNameMap[curr]]) { // already visited,
                                                           // just enqueue exit
                                                           // stmts
              if (isCallStatement(k)) {
                q.push(k * -1);
              } else {
                q.push(k);
              }
              branches.push(curr);
            }
          }
        }
      } else if (l.type == LinkType::BranchBack) {
        if (branches.empty()) {
          visited[v] = true;
          isModified = checkUsesModifies(v, var, results);
          if (!isModified) {
            q.push(v);
          }
        } else {
          if (l.source == branches.top()) {
            branches.pop();
            visited[v] = true;
            isModified = checkUsesModifies(v, var, results);
            if (!isModified) {
              q.push(v);
            }
            break;
          }
        }
      }
    }
  }
  return results;
}

void CFGBip::populateAffectsBipT() {
	populateAffectsBip(false);
	affectsBipT.transitiveClosure();
}

bool CFGBip::isCallStatement(int i) { return (callSet.count(i) == 1); }

bool CFGBip::checkUsesModifies(int v, std::string var,
                               std::deque<int> &results) {
  if (usesAssignTable.contains({std::to_string(v), var})) {
    results.emplace_back(v);
  }
  if (modifiesTable.contains({std::to_string(v), var}) &&
      !whileIfTable.contains({std::to_string(v)}) && !isCallStatement(v)) {
    return true;
  }
  return false;
}

Table CFGBip::getNextBip() { return nextBip; }

Table CFGBip::getNextBipT() {
  populateNextBipT();
  return nextBipT;
}

Table CFGBip::getAffectsBip() {
	populateAffectsBip(true);
	return affectsBip;
}

Table CFGBip::getAffectsBipT() {
  populateAffectsBipT();
  return affectsBipT;
}
