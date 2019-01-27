#include "TNode.h"

TNode::TNode(TNodeType type, string name) {
	this->type = type;
	this->name = name;
}

TNode::~TNode() {}

TNodeType TNode::getType() {
	return type;
}

string TNode::getName() {
	return name;
}

vector<TNode*> TNode::getChildren() {
	return children;
}

void TNode::addChildren(TNode* child) {
	children.push_back(child);
}
