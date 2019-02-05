#pragma once

#include <string>

using std::string;

enum class TokenType
{
	Assign,
	Procedure,
	Print,
	Read,
	If,
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
	Equal
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
};
