#include <Optimizer.h>
#include <algorithm>

Table Optimizer::getResult() {
  if (nodes.size() == 1) {
    return data[0];
  } else if (nodes.size() == 0) {
    return Table(0);
  }
  // initialize map with only 1 table
  for (int i = 0; i < nodes.size(); i++) {
    set<int> present({i});
    m.insert(pair<set<int>, node *>(present, nodes[i]));
  }
  // from size of 2 to size of n
  for (int i = 2; i <= fromlist.size(); i++) {
    vector<set<int>> subSets = powerSet(fromlist, i);
    for (set<int> subset : subSets) {
      // dummpy cost with infinite
      int min_cost = numeric_limits<int>::max();
      node *min_node;
      vector<set<int>> allSubSets = generateAllSets(subset);
      for (set<int> leftTables : allSubSets) {
        set<int> rightTables = removeSets(subset, leftTables);
        node *left = m[leftTables];
        node *right = m[rightTables];
        vector<string> newSchema = joinSchema(left->schema, right->schema);
        int size = 0;
        if (newSchema.size() < left->schema.size() + right->schema.size()) {
          size = max(left->size, right->size);
        } else {
          size = left->size * right->size;
        }
        node *join = new node;
        join->isJoin = true;
        join->left = left;
        join->right = right;
        join->schema = newSchema;
        join->size = size;
        PlanCost pc;
        int cost = pc.getCost(join);
        if (cost <= min_cost) {
          min_cost = cost;
          min_node = join;
        }
      }
      m.insert(pair<set<int>, node *>(subset, min_node));
    }
  }

  node *best_plan = m[fromlist];
  Table result = join(best_plan);
  return result;
}

Table Optimizer::join(node *root) {
  if (root->isJoin) {
    Table left = join(root->left);
    Table right = join(root->right);
    left.mergeWith(right);
    root->data = left;
    root->schema = left.getHeader();
    root->size = left.size();
    return left;
  } else {
    return root->data;
  }
}

void Optimizer::wrapData(vector<Table> tables) {
  for (Table t : tables) {
    node *present = new node;
    present->isJoin = false;
    present->data = t;
    present->schema = t.getHeader();
    present->size = t.size();
    nodes.push_back(present);
  }
}
vector<set<int>> Optimizer::powerSet(set<int> data_set, const int &n) {
  vector<set<int>> result;
  vector<int> data_list(data_set.begin(), data_set.end());
  vector<int> indices(n);
  for (int i = 0; i < indices.size(); i++) {
    indices[i] = i;
  }
  while (true) {
    set<int> s;
    for (int i : indices) {
      s.insert(data_list[i]);
    }
    result.push_back(s);
    int r = n - 1;
    for (int m = data_list.size(); r >= 0 && indices[r] == --m; r--)
      ;
    if (r == -1)
      return result;
    for (int c = indices[r]; r < n;)
      indices[r++] = ++c;
  }
}

vector<set<int>> Optimizer::generateAllSets(set<int> s) {
  vector<set<int>> allSets;
  for (int i = 1; i < s.size(); i++) {
    vector<set<int>> present = powerSet(s, i);
    for (set<int> s : present) {
      allSets.push_back(s);
    }
  }
  return allSets;
}

set<int> Optimizer::removeSets(set<int> from, set<int> to) {
  for (int i : to) {
    from.erase(i);
  }
  return from;
}
vector<string> Optimizer::joinSchema(vector<string> left,
                                     vector<string> right) {
  vector<string> leftSchema = left;
  vector<string> rightSchema = right;
  vector<pair<int, int>> commonIndices;
  set<int> otherDiffIndices;
  for (int j = 0; j < rightSchema.size(); ++j) {
    otherDiffIndices.insert(j);
  }
  for (int i = 0; i < leftSchema.size(); ++i) {
    for (int j = 0; j < rightSchema.size(); ++j) {
      if (leftSchema[i] == rightSchema[j]) {
        commonIndices.emplace_back(i, j);
        otherDiffIndices.erase(j);
        break;
      }
    }
  }
  for (int i = 0; i < otherDiffIndices.size(); i++) {
    leftSchema.push_back(rightSchema[i]);
  }
  return leftSchema;
}

int PlanCost::getCost(node *root) {
  calculateCost(root);
  return cost;
}

int PlanCost::calculateCost(node *root) {
  if (root->isJoin) {
    calculateCost(root->left);
    calculateCost(root->right);
    vector<string> schema =
        Optimizer::joinSchema(root->left->schema, root->right->schema);
    int diff = (root->left->schema.size() + root->right->schema.size()) -
               schema.size();
    if (diff == 0) {
      cost += root->left->size * root->right->size;
    } else {
      cost += root->left->size * root->right->size * diff;
    }
  }
  return 0;
}
