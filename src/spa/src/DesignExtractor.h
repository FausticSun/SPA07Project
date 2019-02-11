#pragma once
#include "PKB.h"
#include "TNode.h"

class DesignExtractor {
private:
  PKB pkb;
  void traverseAST(const std::unique_ptr<TNode> &AST);

public:
  DesignExtractor(const std::unique_ptr<TNode> &AST);
  PKB getPKB();
};
