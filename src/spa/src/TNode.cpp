#include "TNode.h"

TNode::TNode(TNodeType type, string name, int statementNumber) {
  this->type = type;
  this->name = name;
  this->statementNumber = statementNumber;
}

TNode::~TNode() {}

TNodeType TNode::getType() { return type; }

string TNode::getName() { return name; }

int TNode::getStatementNumber() { return statementNumber; }

vector<TNode *> TNode::getChildren() { return children; }

void TNode::setChildren(vector<TNode *> children) { this->children = children; }
