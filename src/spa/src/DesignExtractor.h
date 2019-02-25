#pragma once
#include "PKB.h"
#include "TNode.h"

class DesignExtractor {
private:
  std::unique_ptr<PKB> pkb;
  void traverseAST(std::unique_ptr<TNode> &AST);
  void extractUses(std::unique_ptr<TNode> &AST, int parent);
  void extractModifies(std::unique_ptr<TNode> &AST, int parent);
  void deriveUsesAndModifies();
  void derivesFollowsParentTransitiveClosure();
  void extractFollows(std::unique_ptr<TNode> &AST);
  void extractParent(std::unique_ptr<TNode> &AST);
  void extractAssign(std::unique_ptr<TNode> &AST);
  std::vector<std::vector<bool>>
  floydWarshall(std::vector<std::vector<std::string>> &table, int size);
  std::string extractPostfix(std::unique_ptr<TNode> &AST);
  std::string procName;

public:
  DesignExtractor(std::unique_ptr<TNode> &AST);
  std::unique_ptr<PKB> getPKB();
};
