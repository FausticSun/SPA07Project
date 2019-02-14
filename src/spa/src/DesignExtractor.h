#pragma once
#include "PKB.h"
#include "TNode.h"

class DesignExtractor {
private:
  std::unique_ptr<PKB> pkb;
  void traverseAST(std::unique_ptr<TNode> &AST);
  void extractUses(std::unique_ptr<TNode> &AST, int parent);
  void extractModifies(std::unique_ptr<TNode> &AST, int parent);

public:
  DesignExtractor(std::unique_ptr<TNode> &AST);
  std::unique_ptr<PKB> getPKB();
};
