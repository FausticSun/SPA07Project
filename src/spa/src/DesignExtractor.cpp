#include "DesignExtractor.h"

DesignExtractor::DesignExtractor(const std::unique_ptr<TNode> &AST) : pkb() {
  traverseAST(AST);
}

PKB DesignExtractor::getPKB() { return pkb; }

void DesignExtractor::traverseAST(const std::unique_ptr<TNode> &AST) {
  switch (AST->type) {
  case TNodeType::Procedure:
    pkb.insertProc(AST->name);
    break;
  case TNodeType::Variable:
    pkb.insertVar(AST->name);
    break;
  default:
    for (const std::unique_ptr<TNode> &child : AST->children) {
      traverseAST(child);
    }
    break;
  }
}
