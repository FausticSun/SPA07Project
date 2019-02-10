#include "TNode.h"

TNode::TNode(const TNodeType type, const std::string &name) {
  this->type = type;
  this->name = name;
}
  
TNode::~TNode() = default;

TNodeType TNode::getType() { return type; }

std::string TNode::getName() { return name; }

std::vector<TNode *> TNode::getChildren() { return children; }

void TNode::setChildren(std::vector<TNode *> children) { this->children = children; }
