#pragma once
#include "PKB.h"
#include "TNode.h"

class DesignExtractor {
private:
  std::unique_ptr<PKB> pkb;
  void traverseAST(std::unique_ptr<TNode> &AST);
  void extractFollows(std::unique_ptr<TNode> &AST);
  void extractParent(std::unique_ptr<TNode> &AST);
  void extractParentT(std::unique_ptr<TNode> &AST, int parent);

public:
  DesignExtractor(std::unique_ptr<TNode> &AST);
  std::unique_ptr<PKB> getPKB();
};
