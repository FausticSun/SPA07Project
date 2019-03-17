#include "CFG.h"
#include <queue>
#include <stack>

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

  // Populate compressedGraph
  populateReferenceMap(whileIfTable, inDegree);
  compressedGraph.resize(numCompressedNodes + 1);
  populateCompressedGraph();
}

std::vector<std::vector<int>> CFG::getCompressedGraph() {
  return compressedGraph;
}

void CFG::populateReferenceMap(Table whileIfTable, std::vector<int> inDegree) {
  // Iterative DFS to populate referenceMap
  std::vector<bool> visited(initialGraph.size() + 1, false);
  std::stack<int> stack;
  stack.push(1);
  referenceMap[1] = numCompressedNodes;

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
          referenceMap[v] = numCompressedNodes;
        } else if (!initialGraph[v].empty() && initialGraph[v][0] < v) {
          // Child is last statement of while loop
          numCompressedNodes++;
          referenceMap[v] = numCompressedNodes;
        } else if (inDegree[v] > 1) {
          // Child has in degree > 1
          numCompressedNodes++;
          referenceMap[v] = numCompressedNodes;
        } else {
          // Parent is while or if statement
          if (whileIfTable.contains({std::to_string(u)})) {
            numCompressedNodes++;
            referenceMap[v] = numCompressedNodes;
          } else {
            referenceMap[v] = referenceMap[u];
          }
        }
      }
    }
  }
}

void CFG::populateCompressedGraph() {
  // Traverse the initialGraph and populate compressedGraph using referenceMap
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

      if (referenceMap[v] != referenceMap[u]) {
        compressedGraph[referenceMap[u]].push_back(referenceMap[v]);
      }
    }
  }
}
