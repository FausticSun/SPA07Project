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
	Greater,
	GreaterThanOrEqual,
	Lesser,
	LesserThanOrEqual,
	Equal,
	NotEqual,
	Not,
	And,
	Or
};

class TNode {
public:
	TNode(TNodeType, string = "", int statementNumber = 0);
	~TNode();
	TNodeType getType();
	string getName();
	int getStatementNumber();
	vector<TNode*> getChildren();
	void setChildren(vector<TNode*>);

private:
	TNodeType type;
	string name;
	int statementNumber;
	vector<TNode*> children;
};
