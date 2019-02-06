#pragma once
#include "TNode.h"
#include "Token.h"

#include <queue>

using std::queue;

class Parser {
public:
	Parser();
	~Parser();
	queue<Token*> parse(string);
	TNode* buildAST(queue<Token*> &tokenQueue);

private:
	queue<Token*> tokenQueue;
	Token* token;
	void getNextToken();
	void expectToken(string);
	TNode* createTNodeProcedure();
	TNode* createTNodeAssign();
	TNode* createTNodeRead();
	TNode* createTNodePrint();
	TNode* createTNodeWhile();
	TNode* createTNodeIf();
	TNode* createTNodeStatement();
	TNode* createTNodeStatementList();
	TNode* createTNodeConditionExpression();
	TNode* createTNodeRelativeExpression();
	TNode* createTNodeRelativeFactor();
	TNode* createTNodeExpression();
	TNode* createTNodeTerm();
	TNode* createTNodeFactor();
	TNode* createTNode(TNodeType, vector<TNode*>);
};
