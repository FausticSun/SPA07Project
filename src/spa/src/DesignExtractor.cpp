#include "DesignExtractor.h"

DesignExtractor::DesignExtractor(std::unique_ptr<TNode> &AST) : pkb(new PKB()) {
  traverseAST(AST);
  derivesFollowsParentTransitiveClosure();
  deriveUsesAndModifies();
}

std::unique_ptr<PKB> DesignExtractor::getPKB() { return std::move(pkb); }

void DesignExtractor::traverseAST(std::unique_ptr<TNode> &AST) {
  switch (AST->type) {
  case TNodeType::Procedure:
    pkb->insertProc(AST->name);
    this->procName = AST->name; // temp solution
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
    extractAssign(AST);
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
    pkb->setUses(procName, AST->name); // temp solution
    return;
  }
  for (auto &tNode : AST->children) {
    extractUses(tNode, parent);
  }
}

void DesignExtractor::extractModifies(std::unique_ptr<TNode> &AST, int parent) {
  pkb->setModifies(parent, AST->name);
  pkb->setModifies(procName, AST->name); // temp solution
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

void DesignExtractor::derivesFollowsParentTransitiveClosure() {
  int size = pkb->getStatementCount();
  auto followsTable = pkb->getFollowsTable();
  auto followsTMat = floydWarshall(followsTable, size);
  for (int i = 0; i < size; ++i) {
    for (int j = 0; j < size; ++j) {
      if (i == j) {
        continue;
      }
      if (followsTMat[i][j]) {
        pkb->setFollowsT(i, j);
      }
    }
  }
  auto parentTable = pkb->getParentTable();
  auto parentTMat = floydWarshall(parentTable, size);
  for (int i = 0; i < size; ++i) {
    for (int j = 0; j < size; ++j) {
      if (i == j) {
        continue;
      }
      if (parentTMat[i][j]) {
        pkb->setParentT(i, j);
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
  }
}

void DesignExtractor::extractParent(std::unique_ptr<TNode> &AST) {
  int parent = AST->statementNumber;
  for (auto tNodeIt = std::next(AST->children.begin());
       tNodeIt != AST->children.end(); ++tNodeIt) {
    for (auto childIt = (*tNodeIt)->children.begin();
         childIt != (*tNodeIt)->children.end(); ++childIt) {
      pkb->setParent(AST->statementNumber, (*childIt)->statementNumber);
    }
  }
}

void DesignExtractor::extractAssign(std::unique_ptr<TNode> &AST) {
  auto var = AST->children.front()->name;
  auto expr = extractPostfix(AST->children.back());
  pkb->insertAssign(AST->statementNumber, var, expr);
}

std::vector<std::vector<bool>>
DesignExtractor::floydWarshall(std::vector<std::vector<std::string>> &table,
                               int size) {
  std::vector<std::vector<bool>> matrix(size, std::vector<bool>(size, false));
  for (auto it = table.begin(); it != table.end(); ++it) {
    int i = std::stoi(it->at(0));
    int j = std::stoi(it->at(1));
    matrix[i][j] = true;
  }
  for (int i = 0; i < size; ++i) {
    matrix[i][i] = true;
  }
  for (int k = 0; k < size; ++k) {
    for (int i = 0; k < size; ++k) {
      for (int j = 0; k < size; ++k) {
        if (matrix[i][k] && matrix[k][j]) {
          matrix[i][j] = true;
        }
      }
    }
  }
  return matrix;
}

std::string DesignExtractor::extractPostfix(std::unique_ptr<TNode> &AST) {
  std::string expr;
  for (auto it = AST->children.begin(); it != AST->children.end(); ++it) {
    expr += extractPostfix(*it);
  }
  expr += AST->name + " ";
  return expr;
}
