#include <iostream>

#include "Lexer.h"
#include "Parser.h"

Parser::Parser() : token() {}

Parser::~Parser() {}

std::queue<Token> Parser::parse(std::string filePath) {
  Lexer lexer;
  std::vector<Token> tokens = lexer.tokenizeFile(filePath);
  std::queue<Token> tokenQueue;
  for (Token token : tokens) {
    tokenQueue.push(token);
  }
  return tokenQueue;
}

TNode *Parser::buildAst(std::queue<Token> &tokenQueue) {
  this->tokenQueue = tokenQueue;
  vector<TNode *> procedureTNodes;
  while (!this->tokenQueue.empty()) {
	  expectToken("procedure");
	  procedureTNodes.push_back(createTNodeProcedure());
  }
  return createTNode(TNodeType::Program, procedureTNodes);
}

void Parser::getNextToken() {
  Token nextToken = tokenQueue.front();
  tokenQueue.pop();
  token = nextToken;
}

void Parser::expectToken(std::string expectedToken) {
  getNextToken();
  if (token.name != expectedToken) {
    throw std::logic_error("Expected '" + expectedToken + "' but got '" +
                      token.name + "'");
  }
}

TNode *Parser::createTNodeProcedure() {
  getNextToken();
  TNode *procedureTNode =
      new TNode(TNodeType::Procedure, token.name);
  expectToken("{");
  TNode *stmtLstTNode = createTNodeStatementList();
  expectToken("}");
  procedureTNode->setChildren({stmtLstTNode});
  return procedureTNode;
}

TNode *Parser::createTNodeAssign() {
  TNode *variableTNode =
      new TNode(TNodeType::Variable, token.name);
  expectToken("=");
  TNode *expressionTNode = createTNodeExpression();
  expectToken(";");
  return createTNode(TNodeType::Assign, {variableTNode, expressionTNode});
}

TNode *Parser::createTNodeRead() {
  getNextToken(); // token holding var_name
  TNode *variableTNode =
      new TNode(TNodeType::Variable, token.name);
  expectToken(";");
  return createTNode(TNodeType::Read, {variableTNode});
}

TNode *Parser::createTNodePrint() {
  getNextToken(); // token holding var_name
  TNode *variableTNode =
      new TNode(TNodeType::Variable, token.name);
  expectToken(";");
  return createTNode(TNodeType::Print, {variableTNode});
}

TNode *Parser::createTNodeWhile() {
  expectToken("(");
  TNode *condExpressionTNode = createTNodeConditionExpression();
  expectToken(")");
  expectToken("{");
  TNode *stmtLstTNode = createTNodeStatementList();
  expectToken("}");
  return createTNode(TNodeType::While, {condExpressionTNode, stmtLstTNode});
}

TNode *Parser::createTNodeIf() {
  expectToken("(");
  TNode *condExpressionTNode = createTNodeConditionExpression();
  expectToken(")");
  expectToken("then");
  expectToken("{");
  TNode *thenStmtLstTNode = createTNodeStatementList();
  expectToken("}");
  expectToken("else");
  expectToken("{");
  TNode *elseStmtLstTNode = createTNodeStatementList();
  expectToken("}");
  return createTNode(TNodeType::If,
                     {condExpressionTNode, thenStmtLstTNode, elseStmtLstTNode});
}

TNode *Parser::createTNodeStatementList() {
  TNode *stmtLstTNode = new TNode(TNodeType::StatementList);
  std::vector<TNode *> stmtTNodes;
  while (tokenQueue.front().name != "}") {
    stmtTNodes.push_back(createTNodeStatement());
    if (tokenQueue.empty()) {
      throw std::logic_error("Missing '}'");
    }
  }
  if (stmtTNodes.empty()) {
    throw std::logic_error("Expected at least 1 statement but got 0");
  }
  stmtLstTNode->setChildren(stmtTNodes);
  return stmtLstTNode;
}

TNode *Parser::createTNodeStatement() {
  getNextToken(); // holding token after { or ;
  switch (token.type) {
  case TokenType::Identifier:
    return createTNodeAssign();
  case TokenType::Read:
    return createTNodeRead();
  case TokenType::Print:
    return createTNodePrint();
  case TokenType::While:
    return createTNodeWhile();
  case TokenType::If:
    return createTNodeIf();
  default:
    throw std::invalid_argument("Expected TokenType to be 'IDENTIFIER', 'Read', "
		"'Print', 'While' or 'If' but got '" +
		token.name + "'");
  }
}

TNode *Parser::createTNodeConditionExpression() {
  TNode *condExpressionTNode;
  TNode *leftCondExpressionTNode;
  TNode *rightCondExpressionTNode;
  TNode *previousTNode = NULL;
  TokenType nextTokenType;
  switch (tokenQueue.front().type) {
  case TokenType::ExclamationMark:
    expectToken("!");
    expectToken("(");
    condExpressionTNode = createTNodeConditionExpression();
    expectToken(")");
    return createTNode(TNodeType::Not, {condExpressionTNode});
  case TokenType::OpenParenthesis:
    expectToken("(");
    leftCondExpressionTNode = createTNodeConditionExpression();
    expectToken(")");
	nextTokenType = tokenQueue.front().type;
	while (nextTokenType == TokenType::And || nextTokenType == TokenType::Or) {
		switch (nextTokenType) {
		case TokenType::And:
			expectToken("&&");
			expectToken("(");
			rightCondExpressionTNode = createTNodeConditionExpression();
			expectToken(")");
			if (previousTNode == NULL) {
				previousTNode =
					createTNode(TNodeType::And, 
						{ leftCondExpressionTNode, rightCondExpressionTNode });
			}
			else {
				previousTNode =
					createTNode(TNodeType::And, 
						{ previousTNode, rightCondExpressionTNode });
			}
			break;
		case TokenType::Or:
			expectToken("||");
			expectToken("(");
			rightCondExpressionTNode = createTNodeConditionExpression();
			expectToken(")");
			if (previousTNode == NULL) {
				previousTNode =
					createTNode(TNodeType::Or,
						{ leftCondExpressionTNode, rightCondExpressionTNode });
			}
			else {
				previousTNode =
					createTNode(TNodeType::Or,
						{ previousTNode, rightCondExpressionTNode });
			}
			break;
		}
		nextTokenType = tokenQueue.front().type;
	}
	return previousTNode;
  default:
    return createTNodeRelativeExpression();
  }
}

TNode *Parser::createTNodeRelativeExpression() {
  TNode *leftRelFactor = createTNodeRelativeFactor();
  TNode *rightRelFactor;
  switch (tokenQueue.front().type) {
  case TokenType::Greater:
    rightRelFactor = createTNodeRelativeFactor();
    return createTNode(TNodeType::Greater, {leftRelFactor, rightRelFactor});
  case TokenType::GreaterThanOrEqual:
    rightRelFactor = createTNodeRelativeFactor();
    return createTNode(TNodeType::GreaterThanOrEqual,
                       {leftRelFactor, rightRelFactor});
  case TokenType::Lesser:
    rightRelFactor = createTNodeRelativeFactor();
    return createTNode(TNodeType::Lesser, {leftRelFactor, rightRelFactor});
  case TokenType::LesserThanOrEqual:
    rightRelFactor = createTNodeRelativeFactor();
    return createTNode(TNodeType::LesserThanOrEqual,
                       {leftRelFactor, rightRelFactor});
  case TokenType::Equal:
    rightRelFactor = createTNodeRelativeFactor();
    return createTNode(TNodeType::Equal, {leftRelFactor, rightRelFactor});
  case TokenType::NotEqual:
    rightRelFactor = createTNodeRelativeFactor();
    return createTNode(TNodeType::NotEqual, {leftRelFactor, rightRelFactor});
  default:
    expectToken(">, >=, <, <=, ==, !=");
  }
}

TNode *Parser::createTNodeRelativeFactor() {
  getNextToken();
  switch (token.type) {
  case TokenType::Identifier:
    return new TNode(TNodeType::Variable, token.name);
  case TokenType::Constant:
    return new TNode(TNodeType::Constant, token.name);
  default:
    return createTNodeExpression();
  }
}

TNode *Parser::createTNodeExpression() {
  TNode *leftTermTNode = createTNodeTerm();
  TNode *previousTNode = NULL;
  TokenType nextTokenType = tokenQueue.front().type;
  while (nextTokenType == TokenType::Plus ||
         nextTokenType == TokenType::Minus) {
    TNode *rightTermTNode;
    switch (nextTokenType) {
    case TokenType::Plus:
      expectToken("+");
      rightTermTNode = createTNodeTerm();
      if (previousTNode == NULL) {
        previousTNode =
            createTNode(TNodeType::Plus, {leftTermTNode, rightTermTNode});
      } else {
        previousTNode =
            createTNode(TNodeType::Plus, {previousTNode, rightTermTNode});
      }
      break;
    case TokenType::Minus:
      expectToken("-");
      rightTermTNode = createTNodeTerm();
      if (previousTNode == NULL) {
        previousTNode =
            createTNode(TNodeType::Minus, {leftTermTNode, rightTermTNode});
      } else {
        previousTNode =
            createTNode(TNodeType::Minus, {previousTNode, rightTermTNode});
      }
      break;
    }
    nextTokenType = tokenQueue.front().type;
  }
  if (previousTNode == NULL) {
    return leftTermTNode;
  } else {
    return previousTNode;
  }
}

TNode *Parser::createTNodeTerm() {
  TNode *leftFactorTNode = createTNodeFactor();
  TNode *previousTNode = NULL;
  TokenType nextTokenType = tokenQueue.front().type;
  while (nextTokenType == TokenType::Multiply ||
         nextTokenType == TokenType::Divide ||
         nextTokenType == TokenType::Mod) {
    TNode *rightFactorTNode;
    switch (nextTokenType) {
    case TokenType::Multiply:
      expectToken("*");
      rightFactorTNode = createTNodeFactor();
      if (previousTNode == NULL) {
        previousTNode = createTNode(TNodeType::Multiply,
                                    {leftFactorTNode, rightFactorTNode});
      } else {
        previousTNode =
            createTNode(TNodeType::Multiply, {previousTNode, rightFactorTNode});
      }
      break;
    case TokenType::Divide:
      expectToken("/");
      rightFactorTNode = createTNodeFactor();
      if (previousTNode == NULL) {
        previousTNode =
            createTNode(TNodeType::Divide, {leftFactorTNode, rightFactorTNode});
      } else {
        previousTNode =
            createTNode(TNodeType::Divide, {previousTNode, rightFactorTNode});
      }
      break;
    case TokenType::Mod:
      expectToken("%");
      rightFactorTNode = createTNodeFactor();
      if (previousTNode == NULL) {
        previousTNode =
            createTNode(TNodeType::Mod, {leftFactorTNode, rightFactorTNode});
      } else {
        previousTNode =
            createTNode(TNodeType::Mod, {previousTNode, rightFactorTNode});
      }
      break;
    }
    nextTokenType = tokenQueue.front().type;
  }
  if (previousTNode == NULL) {
    return leftFactorTNode;
  } else {
    return previousTNode;
  }
}

TNode *Parser::createTNodeFactor() {
  getNextToken();
  switch (token.type) {
  case TokenType::Identifier:
    return new TNode(TNodeType::Variable, token.name);
  case TokenType::Constant:
    return new TNode(TNodeType::Constant, token.name);
  case TokenType::OpenParenthesis:
    TNode *expressionTNode = createTNodeExpression();
    expectToken(")");
    return expressionTNode;
  }
}

TNode *Parser::createTNode(TNodeType type, std::vector<TNode *> children) {
  TNode *tNode = new TNode(type);
  tNode->setChildren(children);
  return tNode;
}

int Parse() { return 0; }
