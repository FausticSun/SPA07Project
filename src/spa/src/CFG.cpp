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
	std::vector<int> result;
	std::vector<std::vector<int>> compressedCFG;
	if (isForward) {
		compressedCFG = forwardCompressedGraph;
	}
	else {
		compressedCFG = reverseCompressedGraph;
	}
	std::vector<bool> visited(forwardCompressedGraph.size() + 1, false); 
	visited[0] = true; // no stmt line 0

	int startNode = initialToCompressed.at(start);
	std::queue<int> q;

	//add everything in node
	std::vector<int> linesInNode = compressedToInitial.at(startNode);
	int index = start - linesInNode[0] + 1;

	for (int i = index; i < linesInNode.size(); i++) {
		result.push_back(linesInNode[i]);
		visited[linesInNode[i]] = true;
	}

	q.push(startNode);
	while (!q.empty()) {
		int curr = q.front();
		q.pop();

		for (int i : compressedCFG[curr]) {
			if (!visited[compressedToInitial.at(i)[0]]) {
				for (int j : compressedToInitial.at(i)) {
					if (!visited[j]) {
						visited[j] = true;
						result.push_back(j);
					}
				}
				q.push(i);
			}
		}
	}

	return result;
}

Table CFG::getNextT() const {
  Table table{2};
  for (int i = 1; i < initialGraph.size() + 1; i++) {
	  std::vector<int> result = traverseCFG(i, true);
	  for (int j : result) {
		  table.insertRow({ std::to_string(i), std::to_string(j) });
	  }
  }
  return table;
}

Table CFG::getNextT(int start, bool isForward) const {
  Table table{1};
  std::vector<int> result = traverseCFG(start, isForward);
  for (int i : result) {
	  table.insertRow({ std::to_string(i) });
  }
  return table;
}
