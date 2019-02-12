#pragma once
#include "PKB.h"
#include "TNode.h"

class DesignExtractor {
private:
  std::unique_ptr<PKB> pkb;
  void traverseAST(std::unique_ptr<TNode> &AST, std::vector<int> parents = {});
  void handleStmtLst(std::unique_ptr<TNode> &AST);

public:
  DesignExtractor(std::unique_ptr<TNode> &AST);
  std::unique_ptr<PKB> getPKB();
};
