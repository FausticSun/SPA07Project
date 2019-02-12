#include "DesignExtractor.h"

DesignExtractor::DesignExtractor(std::unique_ptr<TNode> &AST) : pkb(new PKB()) {
  traverseAST(AST);
}

std::unique_ptr<PKB> DesignExtractor::getPKB() { return std::move(pkb); }

void DesignExtractor::traverseAST(std::unique_ptr<TNode> &AST,
                                  std::vector<int> parents) {
  switch (AST->type) {
  case TNodeType::Procedure:
    pkb->insertProc(AST->name);
    break;
  case TNodeType::Variable:
    pkb->insertVar(AST->name);
    break;
  case TNodeType::StatementList:
    handleStmtLst(AST);
  case TNodeType::If:
    break;
  case TNodeType::While:
    break;
  default:
    break;
  }
  for (std::unique_ptr<TNode> &child : AST->children) {
    traverseAST(child);
  }
}

void DesignExtractor::handleStmtLst(std::unique_ptr<TNode> &AST) {}
