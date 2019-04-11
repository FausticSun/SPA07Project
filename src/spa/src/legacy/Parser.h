#pragma once

#include "TNode.h"
#include "Token.h"

#include <queue>

class Parser {
public:
  Parser();
  std::unique_ptr<TNode> buildAst(std::queue<Token> &tokenQueue);

private:
  int statementNumber;
  std::queue<Token> tokenQueue;
  Token token;
  bool isExprRelFactor = false;
  void getNextToken();
  void expectToken(std::string);
  std::unique_ptr<TNode> createTNodeProcedure();
  std::unique_ptr<TNode> createTNodeAssign();
  std::unique_ptr<TNode> createTNodeRead();
  std::unique_ptr<TNode> createTNodePrint();
  std::unique_ptr<TNode> createTNodeWhile();
  std::unique_ptr<TNode> createTNodeIf();
  std::unique_ptr<TNode> createTNodeStatement();
  std::unique_ptr<TNode> createTNodeStatementList();
  std::unique_ptr<TNode> createTNodeConditionExpression();
  std::unique_ptr<TNode> createTNodeRelativeExpression();
  std::unique_ptr<TNode> createTNodeRelativeExpression(std::unique_ptr<TNode>);
  std::unique_ptr<TNode> createTNodeRelativeFactor();
  std::unique_ptr<TNode> createTNodeExpression();
  std::unique_ptr<TNode> createTNodeTerm();
  std::unique_ptr<TNode> createTNodeFactor();
  int assignStatementNumber();
};
