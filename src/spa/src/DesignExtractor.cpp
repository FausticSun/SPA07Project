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
  case TNodeType::If:
    extractUses(AST->children.front(), AST->statementNumber);
    break;
  case TNodeType::While:
    extractUses(AST->children.front(), AST->statementNumber);
    break;
  case TNodeType::Assign:
    extractUses(AST->children.back(), AST->statementNumber);
    extractModifies(AST->children.front(), AST->statementNumber);
    break;
  case TNodeType::Print:
    extractUses(AST->children.front(), AST->statementNumber);
    break;
  case TNodeType::Read:
    extractModifies(AST->children.front(), AST->statementNumber);
    break;
  default:
    break;
  }
  for (std::unique_ptr<TNode> &child : AST->children) {
    traverseAST(child);
  }
}

void DesignExtractor::extractUses(std::unique_ptr<TNode> &AST, int parent) {
  if (AST->children.empty() && AST->type == TNodeType::Variable) {
    pkb->setUses(parent, AST->name);
    return;
  }
  for (auto &tNode : AST->children) {
    extractUses(tNode, parent);
  }
}

void DesignExtractor::extractModifies(std::unique_ptr<TNode> &AST, int parent) {
  pkb->setModifies(parent, AST->name);
}

void DesignExtractor::deriveUsesAndModifies() {
  for (auto type : {StatementType::While, StatementType::If}) {
    const std::set<std::string> stmts = pkb->getStatementsOfType(type);
    for (auto stmt : stmts) {
      const std::set<std::string> containerStmts = pkb->getParentT(stmt);
      for (auto containerStmt : containerStmts) {
        const std::set<std::string> usesVars = pkb->getUses(containerStmt);
        const std::set<std::string> modifiesVars =
            pkb->getModifies(containerStmt);
        for (auto usesVar : usesVars) {
          pkb->setUses(std::stoi(stmt), usesVar);
        }
        for (auto modifiesVar : modifiesVars) {
          pkb->setModifies(std::stoi(stmt), modifiesVar);
        }
      }
    }
  }
}
