#include "DesignExtractor.h"

DesignExtractor::DesignExtractor(std::unique_ptr<TNode> &AST) : pkb(new PKB()) {
  traverseAST(AST);
  deriveUsesAndModifies();
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
    pkb->insertStatement(std::to_string(AST->statementNumber),
                         StatementType::If);
    extractParent(AST);

    extractUses(AST->children.front(), AST->statementNumber);
    break;
  case TNodeType::While:
    pkb->insertStatement(std::to_string(AST->statementNumber),
                         StatementType::While);
    extractParent(AST);

    extractUses(AST->children.front(), AST->statementNumber);
    break;
  case TNodeType::Assign:
    pkb->insertStatement(std::to_string(AST->statementNumber),
                         StatementType::Assign);
    extractUses(AST->children.back(), AST->statementNumber);
    extractModifies(AST->children.front(), AST->statementNumber);
    break;
  case TNodeType::Print:
    pkb->insertStatement(std::to_string(AST->statementNumber),
                         StatementType::Print);
    extractUses(AST->children.front(), AST->statementNumber);
    break;
  case TNodeType::Read:
    pkb->insertStatement(std::to_string(AST->statementNumber),
                         StatementType::Read);
    extractModifies(AST->children.front(), AST->statementNumber);
  case TNodeType::StatementList:
    extractFollows(AST);
    break;
  case TNodeType::Constant:
    pkb->insertConstant(std::stoi(AST->name));
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
        pkb->setParentT(AST->statementNumber, (*childIt)->statementNumber);
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
