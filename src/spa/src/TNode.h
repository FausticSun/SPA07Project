#pragma once

#include <vector>
#include <string>

using std::vector;
using std::string;

enum class TNodeType
{
	Program,
	Procedure,
	StatementList,
	Assign,
	Call,
	Print,
	Read,
	If,
	Then,
	Else,
	While,
	Variable,
	Constant,
	SemiColon,
	OpenCurlyBrace,
	CloseCurlyBrace,
	Plus,
	Minus,
	Multiply,
	Divide,
	Mod,
	Equal,
	Error
};

class TNode {
public:
	TNode(TNodeType, string = "");
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
