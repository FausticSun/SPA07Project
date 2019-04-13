#pragma once

#include <Table.h>
#include <map>
#include <set>
#include <vector>

using namespace std;

struct node {
public:
  bool isJoin;
  shared_ptr<node> left;
  shared_ptr<node> right;
  Table data = Table(0);
  vector<string> schema;
  int size;
};

class Optimizer {
private:
  vector<Table> data;
  vector<shared_ptr<node>> nodes;
  set<int> fromlist;
  map<set<int>, shared_ptr<node>> m;

public:
  Optimizer(vector<Table> d) {
    data = d;
    wrapData(data);
    for (int i = 0; i < nodes.size(); i++) {
      fromlist.insert(i);
    }
  }
  Table getResult();
  void wrapData(vector<Table> t);
  vector<set<int>> powerSet(set<int> data_set, const int &n);
  vector<set<int>> generateAllSets(set<int> s);
  set<int> removeSets(set<int> from, set<int> to);
  static vector<string> joinSchema(vector<string> left, vector<string> right);
  Table join(shared_ptr<node> root);
};

class PlanCost {
public:
  int cost;
  PlanCost() { cost = 0; }
  int getCost(shared_ptr<node> root);
  int calculateCost(shared_ptr<node> n);
  int getStats(node n);
};
