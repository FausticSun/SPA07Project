#pragma once

#include "TNode.h"
#include "Token.h"

#include <queue>

class Parser {
public:
  Parser();
  ~Parser();
  std::queue<Token> parse(std::string);
  TNode *buildAst(std::queue<Token> &tokenQueue);

private:
  std::queue<Token> tokenQueue;
  Token token;
  void getNextToken();
  void expectToken(std::string);
  TNode *createTNodeProcedure();
  TNode *createTNodeAssign();
  TNode *createTNodeRead();
  TNode *createTNodePrint();
  TNode *createTNodeWhile();
  TNode *createTNodeIf();
  TNode *createTNodeStatement();
  TNode *createTNodeStatementList();
  TNode *createTNodeConditionExpression();
  TNode *createTNodeRelativeExpression();
  TNode *createTNodeRelativeFactor();
  TNode *createTNodeExpression();
  TNode *createTNodeTerm();
  TNode *createTNodeFactor();
  TNode *createTNode(TNodeType, std::vector<TNode *>);
};
