#pragma once

#include <Table.h>
#include <map>
#include <set>
#include <vector>

using namespace std;

struct node {
public:
  bool isJoin;
  node *left;
  node *right;
  Table data = Table(0);
  vector<string> schema;
  int size;
  // node() {
  //	isJoin = false;
  //}
  // node(bool ij, vector<string> sche, int s) {
  //
  //}
  // node(bool ij, Table t, vector<string> sche, int s) {
  //	isJoin = ij;
  //	data = t;
  //	schema = sche;
  //	size = s;
  //}

  // node(bool ij, node l, node r, vector<string> sche, int s) {
  //	isJoin = ij;
  //	size = s;
  //	schema = sche;
  //	left = l.isJoin? new node(l.isJoin,l.left,l.right,l.schema,l.size):
  //}
};

class Optimizer {
private:
  vector<Table> data;
  vector<node *> nodes;
  set<int> fromlist;
  map<set<int>, node *> m;

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
  Table join(node *root);
};

class PlanCost {
public:
  int cost;
  PlanCost() { cost = 0; }
  int getCost(node *root);
  int calculateCost(node *n);
  int getStats(node n);
};
