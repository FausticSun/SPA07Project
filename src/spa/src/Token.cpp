#include "Token.h"

Token::Token(TokenType type, string name) {
	this->type = type;
	this->name = name;
}

Token::~Token() {}

TokenType Token::getType() {
	return type;
}

string Token::getName() {
	return name;
}
