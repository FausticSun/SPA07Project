#include "CFG.h"
#include <queue>
#include <stack>

CFG::CFG() {}

CFG::CFG(Table procStmtTable, Table nextTable, Table whileIfTable,
         int stmtCount) {
  // Populate initialGraph from nextTable (1-based indexing)
  initialGraph.resize(stmtCount + 1);
  for (auto data : nextTable.getData()) {
    int u = std::stoi(data[0]);
    int v = std::stoi(data[1]);
    initialGraph[u].push_back(v);
  }
  // Populate inDegree
  std::vector<int> inDegree(stmtCount + 1, 0);
  for (int i = 1; i < initialGraph.size(); ++i) {
    for (auto j : initialGraph[i]) {
      inDegree[j]++;
    }
  }
  for (auto data : procStmtTable.getData()) {
    int start = std::stoi(data[1]);
    // Not first procedure of program
    if (start != 1) {
      numCompressedNodes++;
    }
    // Procedure only has one statement, initialize 1 node compressed graph
    if (initialGraph[start].empty()) {
      initialToCompressed[start] = numCompressedNodes;
      compressedToInitial[numCompressedNodes] = {start};
      forwardCompressedGraph.push_back({});
      reverseCompressedGraph.push_back({});
    } else {
      // Populate forward and reverse CompressedGraph
      populateInitialToCompressed(start, whileIfTable, inDegree);
      populateCompressedToInitial();
      forwardCompressedGraph.resize(numCompressedNodes + 1);
      reverseCompressedGraph.resize(numCompressedNodes + 1);
      populateCompressedGraph();
    }
  }
}

void CFG::populateInitialToCompressed(int start, Table whileIfTable,
                                      std::vector<int> inDegree) {
  // Iterative DFS to populate initialToCompressed
  std::vector<bool> visited(initialGraph.size() + 1, false);
  std::stack<int> stack;
  stack.push(start);
  initialToCompressed[start] = numCompressedNodes;

  while (!stack.empty()) {
    int u = stack.top();
    stack.pop();
    if (!visited[u]) {
      visited[u] = true;
    }

    for (auto v : initialGraph[u]) {
      if (!visited[v]) {
        stack.push(v);

        if (whileIfTable.contains({std::to_string(v)})) {
          // Child is while or if statement
          numCompressedNodes++;
          initialToCompressed[v] = numCompressedNodes;
        } else if (inDegree[v] > 1) {
          // Child has in degree > 1
          numCompressedNodes++;
          initialToCompressed[v] = numCompressedNodes;
        } else {
          // Parent is while or if statement
          if (whileIfTable.contains({std::to_string(u)})) {
            numCompressedNodes++;
            initialToCompressed[v] = numCompressedNodes;
          } else {
            initialToCompressed[v] = initialToCompressed[u];
          }
        }
      }
    }
  }
}

void CFG::populateCompressedToInitial() {
  for (auto i : initialToCompressed) {
    compressedToInitial[i.second].push_back(i.first);
  }
}

void CFG::populateCompressedGraph() {
  // Traverse the initialGraph and populate forward and reverse CompressedGraph
  // using initialToCompressed
  std::vector<bool> visited(initialGraph.size() + 1, false);
  std::queue<int> queue;
  queue.push(1);
  visited[1] = true;

  while (!queue.empty()) {
    int u = queue.front();
    queue.pop();

    for (auto v : initialGraph[u]) {
      if (!visited[v]) {
        queue.push(v);
        visited[v] = true;
      }

      if (initialToCompressed[v] != initialToCompressed[u]) {
        forwardCompressedGraph[initialToCompressed[u]].push_back(
            initialToCompressed[v]);
        reverseCompressedGraph[initialToCompressed[v]].push_back(
            initialToCompressed[u]);
      }
    }
  }
}

std::vector<int> CFG::traverseCFG(int start, bool isForward) const {
  // flora's traversal code here
  return {};
}

Table CFG::getNextT() const {
  Table table{2};
  // traverse from every node and put results into table
  return table;
}

Table CFG::getNextT(int start, bool isForward) const {
  Table table{1};
  std::vector<int> result = traverseCFG(start, isForward);
  // put result into table
  return table;
}
