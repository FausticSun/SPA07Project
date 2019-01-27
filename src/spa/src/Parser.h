#pragma once

#include "TNode.h"
#include "Token.h"

#include <queue>

using std::queue;

class Parser {
public:
	Parser();
	~Parser();
	void parse(string);
	TNode* buildAST();

private:
	queue<Token*> tokenQueue;
	Token* token;
	void getNextToken();
	TNode* procedure();
	TNode* assign();
	TNode* read();
	TNode* statement();
	TNode* expression();
	TNode* term();
	TNode* factor();
	TNode* createTNodeWithOneChild(TNodeType, TNode*);
	TNode* createTNodeWithTwoChildren(TNodeType, TNode*, TNode*);
};
