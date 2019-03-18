#include "CFG.h"
#include <queue>
#include <stack>

CFG::CFG() {}

CFG::CFG(int start, Table nextTable, Table whileIfTable) {
  // Count number of lines
  std::set<int> lineNumbers;
  for (auto data : nextTable.getData()) {
    lineNumbers.insert(std::stoi(data[0]));
    lineNumbers.insert(std::stoi(data[1]));
  }

  // Populate initialGraph from nextTable (1-based indexing)
  initialGraph.resize(lineNumbers.size() + 1);
  for (auto data : nextTable.getData()) {
    int u = std::stoi(data[0]);
    int v = std::stoi(data[1]);
    initialGraph[u].push_back(v);
  }

  // Populate inDegree
  std::vector<int> inDegree(lineNumbers.size() + 1, 0);
  for (int i = 1; i < initialGraph.size(); ++i) {
    for (auto j : initialGraph[i]) {
      inDegree[j]++;
    }
  }

  // Populate forward and reverse CompressedGraph
  populateInitialToCompressed(whileIfTable, inDegree);
  populateCompressedToInitial();
  forwardCompressedGraph.resize(numCompressedNodes + 1);
  reverseCompressedGraph.resize(numCompressedNodes + 1);
  populateCompressedGraph();
}

std::vector<std::vector<int>> CFG::getForwardCompressedGraph() {
  return forwardCompressedGraph;
}

std::vector<std::vector<int>> CFG::getReverseCompressedGraph() {
  return reverseCompressedGraph;
}

int CFG::getCompressed(int i) { return initialToCompressed[i]; }

std::vector<int> CFG::getInitial(int c) { return compressedToInitial[c]; }

void CFG::populateInitialToCompressed(Table whileIfTable,
                                      std::vector<int> inDegree) {
  // Iterative DFS to populate initialToCompressed
  std::vector<bool> visited(initialGraph.size() + 1, false);
  std::stack<int> stack;
  stack.push(1);
  initialToCompressed[1] = numCompressedNodes;

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
