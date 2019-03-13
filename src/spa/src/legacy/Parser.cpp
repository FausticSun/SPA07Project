#include "Parser.h"

Parser::Parser() : statementNumber(1){};

std::unique_ptr<TNode> Parser::buildAst(std::queue<Token> &tokenQueue) {
  this->tokenQueue = tokenQueue;
  std::vector<std::unique_ptr<TNode>> procedureTNodes;
  while (!this->tokenQueue.empty()) {
    expectToken("procedure");
    procedureTNodes.push_back(std::move(createTNodeProcedure()));
  }
  return std::unique_ptr<TNode>(
      new TNode(TNodeType::Program, std::move(procedureTNodes)));
}

void Parser::getNextToken() {
  Token nextToken = tokenQueue.front();
  tokenQueue.pop();
  token = nextToken;
}

void Parser::expectToken(std::string expectedToken) {
  if (isExprRelFactor) {
    isExprRelFactor = false;
    return;
  }
  getNextToken();
  if (token.name != expectedToken) {
    throw std::logic_error("Expected '" + expectedToken + "' but got '" +
                           token.name + "'");
  }
}

std::unique_ptr<TNode> Parser::createTNodeProcedure() {
  getNextToken();
  std::string procName = token.name;
  expectToken("{");
  std::unique_ptr<TNode> stmtLstTNode = createTNodeStatementList();
  expectToken("}");
  std::vector<std::unique_ptr<TNode>> childNodes;
  childNodes.push_back(std::move(stmtLstTNode));
  return std::unique_ptr<TNode>(
      new TNode(TNodeType::Procedure, procName, std::move(childNodes)));
}

std::unique_ptr<TNode> Parser::createTNodeAssign() {
  int statementNumber = assignStatementNumber();
  std::unique_ptr<TNode> variableTNode =
      std::unique_ptr<TNode>(new TNode(TNodeType::Variable, token.name));
  expectToken("=");
  std::unique_ptr<TNode> expressionTNode = createTNodeExpression();
  expectToken(";");
  std::vector<std::unique_ptr<TNode>> childNodes;
  childNodes.push_back(std::move(variableTNode));
  childNodes.push_back(std::move(expressionTNode));
  return std::unique_ptr<TNode>(
      new TNode(TNodeType::Assign, std::move(childNodes), "", statementNumber));
}

std::unique_ptr<TNode> Parser::createTNodeRead() {
  int statementNumber = assignStatementNumber();
  getNextToken(); // token holding var_name
  std::unique_ptr<TNode> variableTNode =
      std::unique_ptr<TNode>(new TNode(TNodeType::Variable, token.name));
  expectToken(";");
  std::vector<std::unique_ptr<TNode>> childNodes;
  childNodes.push_back(std::move(variableTNode));
  return std::unique_ptr<TNode>(
      new TNode(TNodeType::Read, std::move(childNodes), "", statementNumber));
}

std::unique_ptr<TNode> Parser::createTNodePrint() {
  int statementNumber = assignStatementNumber();
  getNextToken(); // token holding var_name
  std::unique_ptr<TNode> variableTNode =
      std::unique_ptr<TNode>(new TNode(TNodeType::Variable, token.name));
  expectToken(";");
  std::vector<std::unique_ptr<TNode>> childNodes;
  childNodes.push_back(std::move(variableTNode));
  return std::unique_ptr<TNode>(
      new TNode(TNodeType::Print, std::move(childNodes), "", statementNumber));
}

std::unique_ptr<TNode> Parser::createTNodeWhile() {
  int statementNumber = assignStatementNumber();
  expectToken("(");
  std::unique_ptr<TNode> condExpressionTNode = createTNodeConditionExpression();
  expectToken(")");
  expectToken("{");
  std::unique_ptr<TNode> stmtLstTNode = createTNodeStatementList();
  expectToken("}");
  std::vector<std::unique_ptr<TNode>> childNodes;
  childNodes.push_back(std::move(condExpressionTNode));
  childNodes.push_back(std::move(stmtLstTNode));
  return std::unique_ptr<TNode>(
      new TNode(TNodeType::While, std::move(childNodes), "", statementNumber));
}

std::unique_ptr<TNode> Parser::createTNodeIf() {
  int statementNumber = assignStatementNumber();
  expectToken("(");
  std::unique_ptr<TNode> condExpressionTNode = createTNodeConditionExpression();
  expectToken(")");
  expectToken("then");
  expectToken("{");
  std::unique_ptr<TNode> thenStmtLstTNode = createTNodeStatementList();
  expectToken("}");
  expectToken("else");
  expectToken("{");
  std::unique_ptr<TNode> elseStmtLstTNode = createTNodeStatementList();
  expectToken("}");
  std::vector<std::unique_ptr<TNode>> childNodes;
  childNodes.push_back(std::move(condExpressionTNode));
  childNodes.push_back(std::move(thenStmtLstTNode));
  childNodes.push_back(std::move(elseStmtLstTNode));
  return std::unique_ptr<TNode>(
      new TNode(TNodeType::If, std::move(childNodes), "", statementNumber));
}

std::unique_ptr<TNode> Parser::createTNodeStatementList() {
  std::vector<std::unique_ptr<TNode>> stmtTNodes;
  while (tokenQueue.front().name != "}") {
    stmtTNodes.push_back(std::move(createTNodeStatement()));
    if (tokenQueue.empty()) {
      throw std::logic_error("Missing '}'");
    }
  }
  if (stmtTNodes.empty()) {
    throw std::logic_error("Expected at least 1 statement but got 0");
  }
  return std::unique_ptr<TNode>(
      new TNode(TNodeType::StatementList, std::move(stmtTNodes)));
}

std::unique_ptr<TNode> Parser::createTNodeStatement() {
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
    throw std::invalid_argument(
        "Expected 'Assign', 'Read', "
        "'Print', 'While' or 'If' statements but got '" +
        token.name + "'");
  }
}

std::unique_ptr<TNode> Parser::createTNodeConditionExpression() {
  std::unique_ptr<TNode> condExpressionTNode;
  std::unique_ptr<TNode> leftCondExpressionTNode;
  std::unique_ptr<TNode> rightCondExpressionTNode;
  std::unique_ptr<TNode> tempTNode;
  std::unique_ptr<TNode> previousTNode = NULL;
  TokenType nextTokenType;
  std::vector<std::unique_ptr<TNode>> childNodes;
  switch (tokenQueue.front().type) {
  case TokenType::Identifier:
    return createTNodeRelativeExpression();
  case TokenType::Constant:
    return createTNodeRelativeExpression();
  case TokenType::ExclamationMark:
    expectToken("!");
    expectToken("(");
    condExpressionTNode = createTNodeConditionExpression();
    expectToken(")");
    childNodes.push_back(std::move(condExpressionTNode));
    return std::unique_ptr<TNode>(
        new TNode(TNodeType::Not, std::move(childNodes)));
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
          childNodes.push_back(std::move(leftCondExpressionTNode));
          childNodes.push_back(std::move(rightCondExpressionTNode));
          previousTNode = std::unique_ptr<TNode>(
              new TNode(TNodeType::And, std::move(childNodes)));
        } else {
          childNodes.push_back(std::move(previousTNode));
          childNodes.push_back(std::move(rightCondExpressionTNode));
          previousTNode = std::unique_ptr<TNode>(
              new TNode(TNodeType::And, std::move(childNodes)));
        }
        break;
      case TokenType::Or:
        expectToken("||");
        expectToken("(");
        rightCondExpressionTNode = createTNodeConditionExpression();
        expectToken(")");
        if (previousTNode == NULL) {
          childNodes.push_back(std::move(leftCondExpressionTNode));
          childNodes.push_back(std::move(rightCondExpressionTNode));
          previousTNode = std::unique_ptr<TNode>(
              new TNode(TNodeType::Or, std::move(childNodes)));
        } else {
          childNodes.push_back(std::move(previousTNode));
          childNodes.push_back(std::move(rightCondExpressionTNode));
          previousTNode = std::unique_ptr<TNode>(
              new TNode(TNodeType::Or, std::move(childNodes)));
        }
        break;
      }
      nextTokenType = tokenQueue.front().type;
    }
    if (previousTNode == NULL) {
      // throw std::invalid_argument(
      //     "Expected multiple conditions but got only 1");
      return leftCondExpressionTNode;
    } else {
      return previousTNode;
    }
  default:
    throw std::invalid_argument("Expected 'Identifier', 'Constant', "
                                "'!' or '(' but got '" +
                                token.name + "'");
  }
}

std::unique_ptr<TNode> Parser::createTNodeRelativeExpression() {
  std::unique_ptr<TNode> leftRelFactor = createTNodeRelativeFactor();
  std::unique_ptr<TNode> rightRelFactor;
  std::unique_ptr<TNode> tempTNode;
  std::vector<std::unique_ptr<TNode>> childNodes;
  switch (tokenQueue.front().type) {
  case TokenType::Greater:
    expectToken(">");
    rightRelFactor = createTNodeRelativeFactor();
    childNodes.push_back(std::move(leftRelFactor));
    childNodes.push_back(std::move(rightRelFactor));
    return std::unique_ptr<TNode>(
        new TNode(TNodeType::Greater, std::move(childNodes)));
  case TokenType::GreaterThanOrEqual:
    expectToken(">=");
    rightRelFactor = createTNodeRelativeFactor();
    childNodes.push_back(std::move(leftRelFactor));
    childNodes.push_back(std::move(rightRelFactor));
    return std::unique_ptr<TNode>(
        new TNode(TNodeType::GreaterThanOrEqual, std::move(childNodes)));
  case TokenType::Lesser:
    expectToken("<");
    rightRelFactor = createTNodeRelativeFactor();
    childNodes.push_back(std::move(leftRelFactor));
    childNodes.push_back(std::move(rightRelFactor));
    return std::unique_ptr<TNode>(
        new TNode(TNodeType::Lesser, std::move(childNodes)));
  case TokenType::LesserThanOrEqual:
    expectToken("<=");
    rightRelFactor = createTNodeRelativeFactor();
    childNodes.push_back(std::move(leftRelFactor));
    childNodes.push_back(std::move(rightRelFactor));
    return std::unique_ptr<TNode>(
        new TNode(TNodeType::LesserThanOrEqual, std::move(childNodes)));
  case TokenType::Equal:
    expectToken("==");
    rightRelFactor = createTNodeRelativeFactor();
    childNodes.push_back(std::move(leftRelFactor));
    childNodes.push_back(std::move(rightRelFactor));
    return std::unique_ptr<TNode>(
        new TNode(TNodeType::Equal, std::move(childNodes)));
  case TokenType::NotEqual:
    expectToken("!=");
    rightRelFactor = createTNodeRelativeFactor();
    childNodes.push_back(std::move(leftRelFactor));
    childNodes.push_back(std::move(rightRelFactor));
    return std::unique_ptr<TNode>(
        new TNode(TNodeType::NotEqual, std::move(childNodes)));
  case TokenType::CloseParenthesis:
    expectToken(")"); // needed when left_rel_factor = ( expr )
    return createTNodeRelativeExpression(std::move(leftRelFactor));
  default:
    expectToken(">, >=, <, <=, ==, !=");
  }
}

std::unique_ptr<TNode>
Parser::createTNodeRelativeExpression(std::unique_ptr<TNode> leftRelFactor) {
  std::unique_ptr<TNode> rightRelFactor;
  std::vector<std::unique_ptr<TNode>> childNodes;
  switch (tokenQueue.front().type) {
  case TokenType::Greater:
    expectToken(">");
    rightRelFactor = createTNodeRelativeFactor();
    childNodes.push_back(std::move(leftRelFactor));
    childNodes.push_back(std::move(rightRelFactor));
    isExprRelFactor = true;
    return std::unique_ptr<TNode>(
        new TNode(TNodeType::Greater, std::move(childNodes)));
  case TokenType::GreaterThanOrEqual:
    expectToken(">=");
    rightRelFactor = createTNodeRelativeFactor();
    childNodes.push_back(std::move(leftRelFactor));
    childNodes.push_back(std::move(rightRelFactor));
    isExprRelFactor = true;
    return std::unique_ptr<TNode>(
        new TNode(TNodeType::GreaterThanOrEqual, std::move(childNodes)));
  case TokenType::Lesser:
    expectToken("<");
    rightRelFactor = createTNodeRelativeFactor();
    childNodes.push_back(std::move(leftRelFactor));
    childNodes.push_back(std::move(rightRelFactor));
    isExprRelFactor = true;
    return std::unique_ptr<TNode>(
        new TNode(TNodeType::Lesser, std::move(childNodes)));
  case TokenType::LesserThanOrEqual:
    expectToken("<=");
    rightRelFactor = createTNodeRelativeFactor();
    childNodes.push_back(std::move(leftRelFactor));
    childNodes.push_back(std::move(rightRelFactor));
    isExprRelFactor = true;
    return std::unique_ptr<TNode>(
        new TNode(TNodeType::LesserThanOrEqual, std::move(childNodes)));
  case TokenType::Equal:
    expectToken("==");
    rightRelFactor = createTNodeRelativeFactor();
    childNodes.push_back(std::move(leftRelFactor));
    childNodes.push_back(std::move(rightRelFactor));
    isExprRelFactor = true;
    return std::unique_ptr<TNode>(
        new TNode(TNodeType::Equal, std::move(childNodes)));
  case TokenType::NotEqual:
    expectToken("!=");
    rightRelFactor = createTNodeRelativeFactor();
    childNodes.push_back(std::move(leftRelFactor));
    childNodes.push_back(std::move(rightRelFactor));
    isExprRelFactor = true;
    return std::unique_ptr<TNode>(
        new TNode(TNodeType::NotEqual, std::move(childNodes)));
  default:
    expectToken(">, >=, <, <=, ==, !=");
  }
}

std::unique_ptr<TNode> Parser::createTNodeRelativeFactor() {
  return createTNodeExpression();
}

std::unique_ptr<TNode> Parser::createTNodeExpression() {
  std::unique_ptr<TNode> leftTermTNode = createTNodeTerm();
  std::unique_ptr<TNode> previousTNode = NULL;
  TokenType nextTokenType = tokenQueue.front().type;
  while (nextTokenType == TokenType::Plus ||
         nextTokenType == TokenType::Minus) {
    std::unique_ptr<TNode> rightTermTNode;
    std::vector<std::unique_ptr<TNode>> childNodes;
    switch (nextTokenType) {
    case TokenType::Plus:
      expectToken("+");
      rightTermTNode = createTNodeTerm();
      if (previousTNode == NULL) {
        childNodes.push_back(std::move(leftTermTNode));
        childNodes.push_back(std::move(rightTermTNode));
        previousTNode = std::unique_ptr<TNode>(
            new TNode(TNodeType::Plus, "+", std::move(childNodes)));
      } else {
        childNodes.push_back(std::move(previousTNode));
        childNodes.push_back(std::move(rightTermTNode));
        previousTNode = std::unique_ptr<TNode>(
            new TNode(TNodeType::Plus, "+", std::move(childNodes)));
      }
      break;
    case TokenType::Minus:
      expectToken("-");
      rightTermTNode = createTNodeTerm();
      if (previousTNode == NULL) {
        childNodes.push_back(std::move(leftTermTNode));
        childNodes.push_back(std::move(rightTermTNode));
        previousTNode = std::unique_ptr<TNode>(
            new TNode(TNodeType::Minus, "-", std::move(childNodes)));
      } else {
        childNodes.push_back(std::move(previousTNode));
        childNodes.push_back(std::move(rightTermTNode));
        previousTNode = std::unique_ptr<TNode>(
            new TNode(TNodeType::Minus, "-", std::move(childNodes)));
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

std::unique_ptr<TNode> Parser::createTNodeTerm() {
  std::unique_ptr<TNode> leftFactorTNode = createTNodeFactor();
  std::unique_ptr<TNode> previousTNode = NULL;
  TokenType nextTokenType = tokenQueue.front().type;
  while (nextTokenType == TokenType::Multiply ||
         nextTokenType == TokenType::Divide ||
         nextTokenType == TokenType::Mod) {
    std::unique_ptr<TNode> rightFactorTNode;
    std::vector<std::unique_ptr<TNode>> childNodes;
    switch (nextTokenType) {
    case TokenType::Multiply:
      expectToken("*");
      rightFactorTNode = createTNodeFactor();
      if (previousTNode == NULL) {
        childNodes.push_back(std::move(leftFactorTNode));
        childNodes.push_back(std::move(rightFactorTNode));
        previousTNode = std::unique_ptr<TNode>(
            new TNode(TNodeType::Multiply, "*", std::move(childNodes)));
      } else {
        childNodes.push_back(std::move(previousTNode));
        childNodes.push_back(std::move(rightFactorTNode));
        previousTNode = std::unique_ptr<TNode>(
            new TNode(TNodeType::Multiply, "*", std::move(childNodes)));
      }
      break;
    case TokenType::Divide:
      expectToken("/");
      rightFactorTNode = createTNodeFactor();
      if (previousTNode == NULL) {
        childNodes.push_back(std::move(leftFactorTNode));
        childNodes.push_back(std::move(rightFactorTNode));
        previousTNode = std::unique_ptr<TNode>(
            new TNode(TNodeType::Divide, "/", std::move(childNodes)));
      } else {
        childNodes.push_back(std::move(previousTNode));
        childNodes.push_back(std::move(rightFactorTNode));
        previousTNode = std::unique_ptr<TNode>(
            new TNode(TNodeType::Divide, "/", std::move(childNodes)));
      }
      break;
    case TokenType::Mod:
      expectToken("%");
      rightFactorTNode = createTNodeFactor();
      if (previousTNode == NULL) {
        childNodes.push_back(std::move(leftFactorTNode));
        childNodes.push_back(std::move(rightFactorTNode));
        previousTNode = std::unique_ptr<TNode>(
            new TNode(TNodeType::Mod, "%", std::move(childNodes)));
      } else {
        childNodes.push_back(std::move(previousTNode));
        childNodes.push_back(std::move(rightFactorTNode));
        previousTNode = std::unique_ptr<TNode>(
            new TNode(TNodeType::Mod, "%", std::move(childNodes)));
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

std::unique_ptr<TNode> Parser::createTNodeFactor() {
  getNextToken();
  switch (token.type) {
  case TokenType::Identifier:
    return std::unique_ptr<TNode>(new TNode(TNodeType::Variable, token.name));
  case TokenType::Constant:
    return std::unique_ptr<TNode>(
        new TNode(TNodeType::Constant, std::to_string(std::stoi(token.name))));
  case TokenType::OpenParenthesis:
    std::unique_ptr<TNode> expressionTNode = createTNodeExpression();
    expectToken(")");
    return expressionTNode;
  }
}

int Parser::assignStatementNumber() { return statementNumber++; }
