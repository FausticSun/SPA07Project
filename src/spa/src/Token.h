#pragma once

#include <string>

using std::string;

enum class TokenType
{
	Assign,
	If,
	Then,
	Else,
	Print,
	Procedure,
	Read,
	While,
	Constant,
	Identifier,
	Separator,
	OpenParenthesis,
	CloseParenthesis,
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
	ExclamationMark,
	And,
	Or
};

class Token {
public:
	Token(TokenType, string);
	~Token();
	TokenType getType();
	string getName();

private:
	TokenType type;
	string name;
	int statementNumber;
};
