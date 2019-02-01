#pragma once

#include <string>

using std::string;

enum class TokenType
{
	Constant,
	Identifier,
	If,
	Print,
	Procedure,
	Read,
	While,
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
