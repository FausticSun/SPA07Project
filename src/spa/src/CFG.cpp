#include "CFG.h"
#include "assert.h"
#include <map>
#include <queue>
#include <stack>

CFG::CFG() {}

CFG::CFG(Table procStmtTable, Table nextTable, Table modifiesTable,
         Table usesTable, Table whileIfTable, Table whileParentTable,
         Table assignTable, int stmtCount)
    : procStmtTable(procStmtTable), whileIfTable(whileIfTable),
      modifiesTable(modifiesTable), usesTable(usesTable),
      assignTable(assignTable) {
  // Populate UsesAssignTable and ModifiesAssignTable
  assignTable.setHeader({"s"});
  usesTable.setHeader({"s", "v"});
  modifiesTable.setHeader({"s", "v"});
  usesAssignTable = usesTable;
  modifiesAssignTable = modifiesTable;
  usesAssignTable.mergeWith(assignTable);
  modifiesAssignTable.mergeWith(assignTable);

  // Populate initialGraph from nextTable (1-based indexing)
  initialGraph.resize(stmtCount + 1);
  for (auto data : nextTable.getData()) {
    int u = std::stoi(data[0]);
    int v = std::stoi(data[1]);
    initialGraph[u].emplace_back(v);
  }
  // Populate inDegree
  inDegree.resize(stmtCount + 1);
  for (int i = 1; i < initialGraph.size(); ++i) {
    for (auto j : initialGraph[i]) {
      inDegree[j]++;
    }
  }
  // Populate inDegreeBefore and inDegreeAfter
  inDegreeBefore.resize(stmtCount + 1);
  inDegreeAfter.resize(stmtCount + 1);
  for (int i = 1; i < initialGraph.size(); i++) {
    for (auto j : initialGraph[i]) {
      if (i < j) {
        inDegreeBefore[j]++;
      } else {
        inDegreeAfter[j]++;
      }
    }
  }
  // Populate whileParentMap
  for (auto data : whileParentTable.getData()) {
    whileParentMap[std::stoi(data[0])].insert(std::stoi(data[1]));
  }
  // Populate initial to compressed map
  for (auto data : procStmtTable.getData()) {
    int start = std::stoi(data[1]);
    numCompressedNodes++;
    // Procedure only has one statement
    if (initialGraph[start].empty()) {
      initialToCompressed[start] = numCompressedNodes;
    } else {
      populateInitialToCompressed(start, whileIfTable, inDegree);
    }
  }
  // Populate forward and reverse CompressedGraph
  populateCompressedToInitial();
  forwardCompressedGraph.resize(numCompressedNodes + 1);
  reverseCompressedGraph.resize(numCompressedNodes + 1);
  populateCompressedGraph(procStmtTable);
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
    compressedToInitial[i.second].emplace_back(i.first);
  }
}

void CFG::populateCompressedGraph(Table procStmtTable) {
  // Traverse the initialGraph and populate forward and reverse CompressedGraph
  // using initialToCompressed
  std::vector<bool> visited(initialGraph.size() + 1, false);
  std::queue<int> queue;
  // Mark start of each procedure as visited and push into queue for traversal
  for (auto data : procStmtTable.getData()) {
    int start = std::stoi(data[1]);
    queue.push(start);
    visited[start] = true;
  }
  while (!queue.empty()) {
    int u = queue.front();
    queue.pop();
    for (auto v : initialGraph[u]) {
      if (!visited[v]) {
        queue.push(v);
        visited[v] = true;
      }
      if (initialToCompressed[v] != initialToCompressed[u]) {
        forwardCompressedGraph[initialToCompressed[u]].emplace_back(
            initialToCompressed[v]);
        reverseCompressedGraph[initialToCompressed[v]].emplace_back(
            initialToCompressed[u]);
      }
    }
  }
}

std::deque<int> CFG::getNextTForward(int start, int end = -1) {
  std::deque<int> result;
  std::vector<std::vector<int>> compressedCFG = forwardCompressedGraph;
  bool reachedEnd = false;
  // initialize visited array
  std::vector<bool> visited(initialToCompressed.size() + 1, false);
  visited[0] = true; // no stmt line 0
  // adding all elements in start node
  int startNode = initialToCompressed.at(start);
  std::vector<int> linesInNode = compressedToInitial.at(startNode);
  int index = start - linesInNode[0] + 1;
  for (int i = index; i < linesInNode.size(); i++) {
    result.emplace_back(linesInNode[i]);
    visited[linesInNode[i]] = true;
    if (linesInNode[i] == end) {
      reachedEnd = true;
    }
  }
  std::queue<int> q;
  q.push(startNode);
  while (!q.empty()) {
    int curr = q.front();
    q.pop();
    for (int i : compressedCFG[curr]) {
      if (!visited[compressedToInitial.at(i)[0]]) {
        for (int j : compressedToInitial.at(
                 i)) { // adding all elements within current node
          if (!visited[j]) {
            visited[j] = true;
            result.emplace_back(j);
            if (j == end) { // for nextT with 2 constants
              reachedEnd = true;
              break;
            }
          }
        }
        q.push(i);
      }
    }
  }
  // for nextT with two constants that did not reach end line
  if ((end > 0) && (!reachedEnd)) {
    // return empty vector if end not reachable from start
    return std::deque<int>{};
  }
  return result;
}

std::deque<int> CFG::getNextTReverse(int start) {
  std::deque<int> result;
  std::vector<std::vector<int>> compressedCFG = reverseCompressedGraph;
  std::vector<bool> visited(initialToCompressed.size() + 1, false);
  visited[0] = true;
  // adding all elements in start node
  int startNode = initialToCompressed.at(start);
  std::vector<int> linesInNode = compressedToInitial.at(startNode);
  int index = start - linesInNode[0] - 1;
  for (int i = index; i >= 0; i--) {
    result.emplace_back(linesInNode[i]);
    visited[linesInNode[i]] = true;
  }
  std::queue<int> q;
  q.push(startNode);
  while (!q.empty()) {
    int curr = q.front();
    q.pop();
    for (int i : compressedCFG[curr]) {
      if (!visited[compressedToInitial.at(
              i)[compressedToInitial.at(i).size() - 1]]) {
        for (int j : compressedToInitial.at(
                 i)) { // adding all elements within current node
          if (!visited[j]) {
            visited[j] = true;
            result.emplace_back(j);
          }
        }
        q.push(i);
      }
    }
  }
  return result;
}

Table CFG::getNextT() {
  Table table{2};
  for (int i = 1; i < initialToCompressed.size() + 1; i++) {
    std::deque<int> result = getNextTForward(i);
    for (int j : result) {
      table.insertRow({std::to_string(i), std::to_string(j)});
    }
  }
  return table;
}

Table CFG::getNextT(int start, bool isForward) {
  Table table{1};
  std::deque<int> result;
  if (isForward) {
    result = getNextTForward(start);
  } else {
    result = getNextTReverse(start);
  }
  for (int i : result) {
    table.insertRow({std::to_string(i)});
  }
  return table;
}

bool CFG::isNextT(int start, int end) {
  return getNextTForward(start, end).size() > 0;
}

std::deque<int> CFG::getAffectsForward(int start, std::string v) {
  std::vector<std::vector<int>> compressedCFG = forwardCompressedGraph;
  std::deque<int> results;
  // Initialize visited array
  std::vector<bool> visited(initialToCompressed.size() + 1, false);
  visited[0] = true; // no stmt line 0
  // Add lines in start node to results if modified some variable
  int startNode = initialToCompressed.at(start);
  std::vector<int> linesInNode = compressedToInitial.at(startNode);
  int index = start - linesInNode[0] + 1;
  for (int i = index; i < linesInNode.size(); i++) {
    int node = linesInNode[i];
    if (usesAssignTable.contains({std::to_string(node), v})) {
      results.emplace_back(node);
    }
    if (modifiesTable.contains({std::to_string(node), v})) {
      // v is modified in a line in the start node
      return results;
    }
    visited[node] = true;
  }
  std::queue<int> q;
  q.push(startNode);
  while (!q.empty()) {
    int curr = q.front();
    q.pop();
    // Looping through neighbor nodes
    for (int i : compressedCFG[curr]) {
      if (!visited[compressedToInitial.at(i)[0]]) {
        bool isModified = false;
        for (int j : compressedToInitial.at(i)) {
          if (isModified) {
            visited[j] = true;
          }
          if (!visited[j]) {
            visited[j] = true;
            // Add to results if Uses(j, v) is true
            if (usesAssignTable.contains({std::to_string(j), v})) {
              results.emplace_back(j);
            }
            if (modifiesTable.contains({std::to_string(j), v}) &&
                !whileIfTable.contains({std::to_string(j)})) {
              isModified = true;
            }
          }
        }
        // Enqueue only if no line in compressed node modifies v
        if (!isModified) {
          q.push(i);
        }
      }
    }
  }
  return results;
}

std::deque<int> CFG::getAffectsReverse(int start, std::string v) {
  std::vector<std::vector<int>> compressedCFG = reverseCompressedGraph;
  std::deque<int> results;
  // Initialize visited array
  std::vector<bool> visited(initialToCompressed.size() + 1, false);
  visited[0] = true; // no stmt line 0
  // Add lines in start node to results if modified some variable
  int startNode = initialToCompressed.at(start);
  std::vector<int> linesInNode = compressedToInitial.at(startNode);
  int index = start - linesInNode[0] - 1;
  for (int i = index; i >= 0; i--) {
    int node = linesInNode[i];
    if (modifiesAssignTable.contains({std::to_string(node), v})) {
      results.emplace_back(node);
    }
    if (modifiesTable.contains({std::to_string(node), v})) {
      // v is modified in any line in the start node
      return results;
    }
    visited[node] = true;
  }
  std::queue<int> q;
  q.push(startNode);
  while (!q.empty()) {
    int curr = q.front();
    q.pop();
    // Looping through neighbor nodes
    for (int i : compressedCFG[curr]) {
      if (!visited[compressedToInitial.at(i)[0]]) {
        bool isModified = false;
        auto lines = compressedToInitial.at(i);
        for (int j = lines.size() - 1; j >= 0; j--) {
          int line = lines[j];
          if (isModified) {
            visited[line] = true;
          }
          if (!visited[line]) {
            visited[line] = true;
            // Add to results if Modifies(line, v) and line is assignment stmt
            if (modifiesAssignTable.contains({std::to_string(line), v})) {
              results.emplace_back(line);
            }
            if (modifiesTable.contains({std::to_string(line), v}) &&
                !whileIfTable.contains({std::to_string(line)})) {
              isModified = true;
              break;
            }
          }
        }
        // Enqueue only if no line in compressed node modifies v
        if (!isModified) {
          q.push(i);
        }
      }
    }
  }
  return results;
}

bool CFG::isAffects(int a1, int a2) {
  // Get variable modified in line a1 and used in line a2
  auto modifiesA1Table = modifiesTable;
  modifiesA1Table.setHeader({"a1", "v"});
  modifiesA1Table.filterColumn("a1", {std::to_string(a1)});
  auto usesA2Table = usesTable;
  usesA2Table.setHeader({"a2", "v"});
  usesA2Table.filterColumn("a2", {std::to_string(a2)});
  modifiesA1Table.mergeWith(usesA2Table);
  if (modifiesA1Table.empty()) {
    // No variable modified in line a1 and used in line a2
    return false;
  } else {
    // Continue to get variable
    assert(modifiesA1Table.size() == 1);
    std::string v;
    for (auto data : modifiesA1Table.getData({"v"})) {
      v = data[0];
    }
    auto result = getAffectsForward(a1, v);
    return std::find(result.begin(), result.end(), a2) != result.end();
  }
}

Table CFG::getAffects(int start, bool isForward) {
  Table table{1};
  std::deque<int> result;
  if (isForward) {
    // Affects(k, a) utilizes forwardCompressedGraph, k here is a constant
    auto modifiesATable = modifiesTable;
    modifiesATable.setHeader({"a1", "v"});
    modifiesATable.filterColumn("a1", {std::to_string(start)});
    // modifiesATable has one variable after filter
    assert(modifiesATable.size() == 1);
    std::string v;
    for (auto data : modifiesATable.getData({"v"})) {
      v = data[0];
    }
    result = getAffectsForward(start, v);
  } else {
    // Affects(a, k) utilizes reverseCompressedGraph, k here is a constant
    auto usesATable = usesTable;
    usesATable.setHeader({"a2", "v"});
    usesATable.filter("a2", {std::to_string(start)});
    // usesATable has one or more variables after filter
    std::vector<std::string> variables;
    for (auto data : usesATable.getData({"v"})) {
      variables.emplace_back(data[0]);
    }
    for (auto v : variables) {
      auto temp = getAffectsReverse(start, v);
      result.insert(result.end(), temp.begin(), temp.end());
    }
  }
  for (int i : result) {
    table.insertRow({std::to_string(i)});
  }
  return table;
}

Table CFG::getAffects() {
  Table table{2};
  for (auto data : modifiesAssignTable.getData()) {
    auto result = getAffectsForward(std::stoi(data[0]), data[1]);
    for (int i : result) {
      table.insertRow({data[0], std::to_string(i)});
    }
  }
  return table;
}

std::map<int, std::set<int>> CFG::getAffectsTResults(
    int start, Table modifiesTable, Table parentTable,
    std::map<int, StatementType> stmtMap,
    std::map<int, std::pair<std::string, std::vector<std::string>>> assignMap) {
  std::map<int, std::set<int>> results;
  std::vector<int> visited(initialGraph.size(), 0);
  std::vector<int> inDegreeCopy = inDegree;
  std::vector<int> inDegreeBeforeCopy = inDegreeBefore;
  std::vector<int> inDegreeAfterCopy = inDegreeAfter;
  visited[start]++;
  // allow traversal to go in while loop twice
  for (auto stmt : stmtMap) {
    if (stmt.second == StatementType::While) {
      inDegreeAfterCopy[stmt.first] *= 2;
    }
  }
  // start node is while should not be count as visit
  if (stmtMap[start] == StatementType::While) {
    visited[start]--;
  }
  std::map<int, std::vector<std::pair<std::string, int>>> basketsToMerge;
  std::queue<std::pair<int, std::vector<std::pair<std::string, int>>>> queue;
  std::vector<std::pair<std::string, int>> emptyBasket;
  queue.push(std::make_pair(start, emptyBasket));

  while (!queue.empty()) {
    auto curr = queue.front();
    queue.pop();
    auto node = curr.first;
    auto basket = curr.second;
    auto nodeType = stmtMap.at(node);
    std::vector<std::pair<std::string, int>> newBasket = basket;

    switch (nodeType) {
    case StatementType::Assign: {
      // add to results if used variable is in basket
      auto usedVars = assignMap[node].second;
      for (auto it = newBasket.begin(); it != newBasket.end(); ++it) {
        auto var = (*it).first;
        auto line = (*it).second;
        if (std::find(usedVars.begin(), usedVars.end(), var) !=
            usedVars.end()) {
          results[node].insert(line);
          // backtrack to retrieve more results if any
          std::vector<bool> innerVisited(initialGraph.size(), false);
          std::queue<int> innerQueue;
          innerQueue.push(node);
          while (!innerQueue.empty()) {
            auto i = innerQueue.front();
            innerQueue.pop();
            for (auto j : results[i]) {
              if (!innerVisited[j]) {
                innerVisited[j] = true;
                innerQueue.push(j);
                results[node].insert(j);
              }
            }
          }
        }
      }
      // remove if modified variable is in basket
      auto modifiedVar = assignMap[node].first;
      auto it = newBasket.begin();
      while (it != newBasket.end()) {
        if (modifiesTable.contains(
                {std::to_string((*it).second), modifiedVar})) {
          it = newBasket.erase(it);
        } else {
          ++it;
        }
      }
      newBasket.emplace_back(std::make_pair(modifiedVar, node));
      break;
    }
    case StatementType::Read: {
      // only need to remove only one variable
      auto it = newBasket.begin();
      while (it != newBasket.end()) {
        if (modifiesTable.contains({std::to_string(node), (*it).first})) {
          it = newBasket.erase(it);
          break;
        } else {
          ++it;
        }
      }
      break;
    }
    case StatementType::Call: {
      // can remove more than one variable
      auto it = newBasket.begin();
      while (it != newBasket.end()) {
        if (modifiesTable.contains({std::to_string(node), (*it).first})) {
          it = newBasket.erase(it);
        } else {
          ++it;
        }
      }
      break;
    }
    }

    // traverse to neighbours if possible
    for (int v : initialGraph[node]) {
      // ensure while block is done before enqueue other neighbours of while
      // node
      bool canVisit =
          stmtMap[node] != StatementType::While ||
          parentTable.contains({std::to_string(node), std::to_string(v)}) ||
          visited[node] == inDegreeCopy[node];
      if ((visited[v] < inDegreeBeforeCopy[v] + inDegreeAfterCopy[v]) &&
          canVisit) {
        visited[v]++;
        // merge if neighbour has total indegree > 1
        if (inDegreeBeforeCopy[v] + inDegreeAfterCopy[v] > 1) {
          if (basketsToMerge[v].empty()) {
            basketsToMerge[v] = newBasket;
          } else {
            std::set<std::pair<std::string, int>> basketSet;
            for (auto pair : basketsToMerge[v]) {
              basketSet.insert(pair);
            }
            for (auto pair : newBasket) {
              basketSet.insert(pair);
            }
            basketsToMerge[v].clear();
            for (auto pair : basketSet) {
              basketsToMerge[v].emplace_back(pair);
            }
          }
        }
        // enqueue neighbour if enough visits from before
        if (node < v && visited[v] == inDegreeBeforeCopy[v]) {
          if (inDegreeBeforeCopy[v] + inDegreeAfterCopy[v] > 1) {
            queue.push(std::make_pair(v, basketsToMerge[v]));
          } else {
            queue.push(std::make_pair(v, newBasket));
          }
        }
        bool isWhile = stmtMap[v] == StatementType::While;
        // enqueue neighbour after first loop
        if (isWhile && node > v &&
            visited[v] == inDegreeBeforeCopy[v] + inDegreeAfterCopy[v] / 2) {
          if (inDegreeBeforeCopy[v] + inDegreeAfterCopy[v] > 1) {
            queue.push(std::make_pair(v, basketsToMerge[v]));
          } else {
            queue.push(std::make_pair(v, newBasket));
          }
          // reset for nodes in while block to allow revisit
          for (int s : whileParentMap.at(v)) {
            visited[s] = 0;
          }
        }
        // enqueue neighbour after second loop
        if (isWhile && node > v &&
            visited[v] == inDegreeBeforeCopy[v] + inDegreeAfterCopy[v]) {
          if (inDegreeBeforeCopy[v] + inDegreeAfterCopy[v] > 1) {
            queue.push(std::make_pair(v, basketsToMerge[v]));
          } else {
            queue.push(std::make_pair(v, newBasket));
          }
        }
      }
    }
  }
  return results;
}

Table CFG::getAffectsT(
    Table modifiesTable, Table parentTable,
    std::map<int, StatementType> stmtMap,
    std::map<int, std::pair<std::string, std::vector<std::string>>> assignMap) {
  Table table{2};
  for (auto data : procStmtTable.getData()) {
    std::map<int, std::set<int>> results = getAffectsTResults(
        std::stoi(data[1]), modifiesTable, parentTable, stmtMap, assignMap);
    for (auto to : results) {
      for (auto from : to.second) {
        table.insertRow({std::to_string(from), std::to_string(to.first)});
      }
    }
  }
  return table;
}

void CFG::clearCache() {
  affectsForwardCache.clear();
  affectsReverseCache.clear();
}
