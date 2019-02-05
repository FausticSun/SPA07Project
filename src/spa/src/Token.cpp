#include "Token.h"



Token::Token(TokenType type, string name, int statementNumber) {

	this->type = type;

	this->name = name;

	this->statementNumber = statementNumber;

}



Token::~Token() {}



TokenType Token::getType() {

	return type;

}



string Token::getName() {

	return name;

}



int Token::getStatementNumber() {

	return statementNumber;

}