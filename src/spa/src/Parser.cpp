#include <iostream>

#include "Parser.h"

Parser::Parser() {
	token = NULL;
}
Parser::~Parser() {}

void Parser::parse(string path) {}

TNode* Parser::buildAST() {
	getNextToken();
	return procedure();
}

void Parser::getNextToken() {
	Token* nextToken = tokenQueue.front();
	tokenQueue.pop();
	token = nextToken;
}

TNode* Parser::procedure() {
	getNextToken();
	TNode* procedureTNode = new TNode(TNodeType::Procedure, token->getName());
	getNextToken(); // token holding {
	TNode* stmtLstTNode = new TNode(TNodeType::StatementList);
	while (tokenQueue.front()->getName() != "}") {
		stmtLstTNode->addChildren(statement());
	}
	if (stmtLstTNode->getChildren().empty()) {
		return procedureTNode;
	} else {
		procedureTNode->addChildren(stmtLstTNode);
		return procedureTNode;
	}
}

TNode* Parser::assign() {
	TNode* variableTNode = new TNode(TNodeType::Variable, token->getName());
	getNextToken(); // token holding =
	TNode* expressionTNode = expression();
	getNextToken(); // token holding ;
	return createTNodeWithTwoChildren(TNodeType::Assign, variableTNode, expressionTNode);
}

TNode* Parser::read() {
	getNextToken(); // token holding var_name
	TNode* variableTNode = new TNode(TNodeType::Variable, token->getName());
	getNextToken(); // token holding ;
	return createTNodeWithOneChild(TNodeType::Read, variableTNode);
}

TNode* Parser::statement() {
	getNextToken(); // holding token after {
	switch (token->getType()) {
	case TokenType::Identifier:
		return assign();
	case TokenType::Read:
		return read();
	default:
		return new TNode(TNodeType::Error); // error message will be thrown in the future
	}
}

TNode* Parser::expression() {
	TNode* termTNode = term();
	TNode* expressionTNode;
	switch (tokenQueue.front()->getType()) {
	case TokenType::Plus:
		getNextToken();
		expressionTNode = expression();
		return createTNodeWithTwoChildren(TNodeType::Plus, expressionTNode, termTNode);
	case TokenType::Minus:
		getNextToken();
		expressionTNode = expression();
		return createTNodeWithTwoChildren(TNodeType::Minus, expressionTNode, termTNode);
	default:
		return termTNode;
	}
}

TNode* Parser::term() {
	TNode* factorTNode = factor();
	TNode* termTNode;
	switch (tokenQueue.front()->getType()) {
	case TokenType::Multiply:
		getNextToken();
		termTNode = term();
		return createTNodeWithTwoChildren(TNodeType::Multiply, termTNode, factorTNode);
	case TokenType::Divide:
		getNextToken();
		termTNode = term();
		return createTNodeWithTwoChildren(TNodeType::Divide, termTNode, factorTNode);
	case TokenType::Mod:
		getNextToken();
		termTNode = term();
		return createTNodeWithTwoChildren(TNodeType::Mod, termTNode, factorTNode);
	default:
		return factorTNode;
	}
}

TNode* Parser::factor() {
	getNextToken();
	switch (token->getType()) {
	case TokenType::Identifier:
		return new TNode(TNodeType::Variable, token->getName());
	case TokenType::Constant:
		return new TNode(TNodeType::Constant, token->getName());
	case TokenType::OpenParenthesis:
		TNode* expressionTNode = expression();
		getNextToken(); // token holding )
		return expressionTNode;
	}
}

TNode* Parser::createTNodeWithOneChild(TNodeType type, TNode* child) {
	TNode* tNode = new TNode(type);
	tNode->addChildren(child);
	return tNode;
}

TNode* Parser::createTNodeWithTwoChildren(TNodeType type, TNode* leftChild, TNode* rightChild) {
	TNode* tNode = new TNode(type);
	tNode->addChildren(leftChild);
	tNode->addChildren(rightChild);
	return tNode;
}
