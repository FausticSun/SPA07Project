#pragma once

#include <vector>
#include <string>

using std::vector;
using std::string;

enum class TNodeType
{
	Program,
	Procedure,
	If,
	Then,
	Else,
	While,
	Variable,
	Constant,
	StmtLst,
	Assign,
	Call,
	SemiColon,
	CurlyBraceOpen,
	CurlyBraceClose,
	Equals,
	Plus,
	Minus,
	Times,
	Divide,
	Mod
};

class TNode {
public:
	TNode(TNodeType, string);
	~TNode();
	TNodeType getType();
	string getName();
	vector<TNode*> getChildren();
	void addChildren(TNode*);

private:
	TNodeType type;
	string name;
	vector<TNode*> children;
};
