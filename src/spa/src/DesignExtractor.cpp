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
  case TNodeType::StatementList:
    extractFollows(AST);
  case TNodeType::If:
  case TNodeType::While:
    extractParent(AST);
    break;
  default:
    break;
  }
  for (std::unique_ptr<TNode> &child : AST->children) {
    traverseAST(child);
  }
}

void DesignExtractor::extractFollows(std::unique_ptr<TNode> &AST) {
  if (AST->children.size() < 2) {
    return;
  }
  for (auto it1 = AST->children.begin(); it1 != std::prev(AST->children.end());
       ++it1) {
    pkb->setFollows((*it1)->statementNumber,
                    (*(std::next(it1)))->statementNumber);
    for (auto it2 = std::next(it1); it2 != AST->children.end(); ++it2) {
      pkb->setFollowsT((*it1)->statementNumber, (*it2)->statementNumber);
    }
  }
}

void DesignExtractor::extractParent(std::unique_ptr<TNode> &AST) {
  int parent = AST->statementNumber;
  for (auto tNodeIt = std::next(AST->children.begin());
       tNodeIt != AST->children.end(); ++tNodeIt) {
    for (auto childIt = (*tNodeIt)->children.begin();
         childIt != (*tNodeIt)->children.end(); ++childIt) {
      pkb->setParent(AST->statementNumber, (*childIt)->statementNumber);
      switch ((*childIt)->type) {
      case TNodeType::While:
      case TNodeType::If:
        extractParentT((*childIt), AST->statementNumber);
        break;
      default:
        break;
      }
    }
  }
}

void DesignExtractor::extractParentT(std::unique_ptr<TNode> &AST, int parent) {
  pkb->setParentT(parent, AST->statementNumber);
  for (auto tNodeIt = std::next(AST->children.begin());
       tNodeIt != AST->children.end(); ++tNodeIt) {
    for (auto childIt = (*tNodeIt)->children.begin();
         childIt != (*tNodeIt)->children.end(); ++childIt) {
      pkb->setParentT(parent, (*childIt)->statementNumber);
      switch ((*childIt)->type) {
      case TNodeType::While:
      case TNodeType::If:
        extractParentT((*childIt), AST->statementNumber);
        break;
      default:
        break;
      }
    }
  }
}
