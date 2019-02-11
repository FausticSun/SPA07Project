#include "DesignExtractor.h"

DesignExtractor::DesignExtractor(TNode *AST) : pkb() { traverseAST(AST); }

PKB DesignExtractor::getPKB(TNode *AST) { return pkb; }

void DesignExtractor::traverseAST(TNode *AST) {
  switch (AST->type) {
  case TNodeType::Procedure:
    pkb.insertProc(AST->name);
    break;
  case TNodeType::Variable:
    pkb.insertVar(AST->name);
    break;
  default:
    for (auto c : AST->children) {
      traverseAST(c);
    }
    break;
  }
}
