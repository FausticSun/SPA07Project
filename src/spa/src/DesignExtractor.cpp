#include "DesignExtractor.h"

DesignExtractor::DesignExtractor(std::unique_ptr<TNode> &AST) : pkb(new PKB()) {
  traverseAST(AST);
}

std::unique_ptr<PKB> DesignExtractor::getPKB() { return std::move(pkb); }

void DesignExtractor::traverseAST(std::unique_ptr<TNode> &AST) {
  switch (AST->type) {
  case TNodeType::Procedure:
    pkb->insertProc(AST->name);
    break;
  case TNodeType::Variable:
    pkb->insertVar(AST->name);
    break;
  default:
    for (std::unique_ptr<TNode> &child : AST->children) {
      traverseAST(child);
    }
    break;
  }
}
