#pragma once
#include "PKB.h"
#include "TNode.h"

class DesignExtractor {
private:
  PKB pkb;
  void traverseAST(TNode *AST);

public:
  DesignExtractor(TNode *AST);
  PKB getPKB();
};
