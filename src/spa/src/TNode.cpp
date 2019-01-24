#include "TNode.h"


TNode::TNode(TNodeType type, string name) {
	this->type = type;
	this->name = name;
}

TNode::~TNode() {}

void TNode::addChildren(TNode* child) {
	children.push_back(child);
}