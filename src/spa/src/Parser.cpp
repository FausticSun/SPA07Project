#include "Parser.h"
#include "Lexer.h"

Parser::Parser() : token() {}

std::queue<Token> Parser::parse(std::string filePath) {
  Lexer lexer;
  std::vector<Token> tokens = lexer.tokenizeFile(filePath);
  std::queue<Token> tokenQueue;
  for (Token token : tokens) {
    tokenQueue.push(token);
  }
  return tokenQueue;
}

std::unique_ptr<TNode> Parser::buildAst(std::queue<Token> &tokenQueue) {
  this->tokenQueue = tokenQueue;
  vector<std::unique_ptr<TNode>> procedureTNodes;
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
  return std::unique_ptr<TNode>(
      new TNode(TNodeType::Procedure, procName, {std::move(stmtLstTNode)}));
}

std::unique_ptr<TNode> Parser::createTNodeAssign() {
  std::unique_ptr<TNode> variableTNode =
      std::unique_ptr<TNode>(new TNode(TNodeType::Variable, token.name));
  expectToken("=");
  std::unique_ptr<TNode> expressionTNode = createTNodeExpression();
  expectToken(";");
  return std::unique_ptr<TNode>(
      new TNode(TNodeType::Assign,
                {std::move(variableTNode), std::move(expressionTNode)}));
}

std::unique_ptr<TNode> Parser::createTNodeRead() {
  getNextToken(); // token holding var_name
  std::unique_ptr<TNode> variableTNode =
      std::unique_ptr<TNode>(new TNode(TNodeType::Variable, token.name));
  expectToken(";");
  return std::unique_ptr<TNode>(
      new TNode(TNodeType::Read, {std::move(variableTNode)}));
}

std::unique_ptr<TNode> Parser::createTNodePrint() {
  getNextToken(); // token holding var_name
  std::unique_ptr<TNode> variableTNode =
      std::unique_ptr<TNode>(new TNode(TNodeType::Variable, token.name));
  expectToken(";");
  return std::unique_ptr<TNode>(
      new TNode(TNodeType::Print, {std::move(variableTNode)}));
}

std::unique_ptr<TNode> Parser::createTNodeWhile() {
  expectToken("(");
  std::unique_ptr<TNode> condExpressionTNode = createTNodeConditionExpression();
  expectToken(")");
  expectToken("{");
  std::unique_ptr<TNode> stmtLstTNode = createTNodeStatementList();
  expectToken("}");
  return std::unique_ptr<TNode>(
      new TNode(TNodeType::While,
                {std::move(condExpressionTNode), std::move(stmtLstTNode)}));
}

std::unique_ptr<TNode> Parser::createTNodeIf() {
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
  return std::unique_ptr<TNode>(
      new TNode(TNodeType::While,
                {std::move(condExpressionTNode), std::move(thenStmtLstTNode),
                 std::move(elseStmtLstTNode)}));
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
        "Expected TokenType to be 'IDENTIFIER', 'Read', "
        "'Print', 'While' or 'If' but got '" +
        token.name + "'");
  }
}

std::unique_ptr<TNode> Parser::createTNodeConditionExpression() {
  std::unique_ptr<TNode> condExpressionTNode;
  std::unique_ptr<TNode> leftCondExpressionTNode;
  std::unique_ptr<TNode> rightCondExpressionTNode;
  std::unique_ptr<TNode> previousTNode = NULL;
  TokenType nextTokenType;
  switch (tokenQueue.front().type) {
  case TokenType::ExclamationMark:
    expectToken("!");
    expectToken("(");
    condExpressionTNode = createTNodeConditionExpression();
    expectToken(")");
    return std::unique_ptr<TNode>(
        new TNode(TNodeType::Not, {std::move(condExpressionTNode)}));
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
          previousTNode = std::unique_ptr<TNode>(
              new TNode(TNodeType::And, {std::move(leftCondExpressionTNode),
                                         std::move(rightCondExpressionTNode)}));

        } else {
          previousTNode = std::unique_ptr<TNode>(
              new TNode(TNodeType::And, {std::move(previousTNode),
                                         std::move(rightCondExpressionTNode)}));
        }
        break;
      case TokenType::Or:
        expectToken("||");
        expectToken("(");
        rightCondExpressionTNode = createTNodeConditionExpression();
        expectToken(")");
        if (previousTNode == NULL) {
          previousTNode = std::unique_ptr<TNode>(
              new TNode(TNodeType::Or, {std::move(leftCondExpressionTNode),
                                        std::move(rightCondExpressionTNode)}));
        } else {
          previousTNode = std::unique_ptr<TNode>(
              new TNode(TNodeType::Or, {std::move(previousTNode),
                                        std::move(rightCondExpressionTNode)}));
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

std::unique_ptr<TNode> Parser::createTNodeRelativeExpression() {
  std::unique_ptr<TNode> leftRelFactor = createTNodeRelativeFactor();
  std::unique_ptr<TNode> rightRelFactor;
  switch (tokenQueue.front().type) {
  case TokenType::Greater:
    rightRelFactor = createTNodeRelativeFactor();
    return std::unique_ptr<TNode>(
        new TNode(TNodeType::Greater,
                  {std::move(leftRelFactor), std::move(rightRelFactor)}));
  case TokenType::GreaterThanOrEqual:
    rightRelFactor = createTNodeRelativeFactor();
    return std::unique_ptr<TNode>(
        new TNode(TNodeType::GreaterThanOrEqual,
                  {std::move(leftRelFactor), std::move(rightRelFactor)}));
  case TokenType::Lesser:
    rightRelFactor = createTNodeRelativeFactor();
    return std::unique_ptr<TNode>(
        new TNode(TNodeType::Lesser,
                  {std::move(leftRelFactor), std::move(rightRelFactor)}));
  case TokenType::LesserThanOrEqual:
    rightRelFactor = createTNodeRelativeFactor();
    return std::unique_ptr<TNode>(
        new TNode(TNodeType::LesserThanOrEqual,
                  {std::move(leftRelFactor), std::move(rightRelFactor)}));
  case TokenType::Equal:
    rightRelFactor = createTNodeRelativeFactor();
    return std::unique_ptr<TNode>(
        new TNode(TNodeType::Equal,
                  {std::move(leftRelFactor), std::move(rightRelFactor)}));
  case TokenType::NotEqual:
    rightRelFactor = createTNodeRelativeFactor();
    return std::unique_ptr<TNode>(
        new TNode(TNodeType::NotEqual,
                  {std::move(leftRelFactor), std::move(rightRelFactor)}));
  default:
    expectToken(">, >=, <, <=, ==, !=");
  }
}

std::unique_ptr<TNode> Parser::createTNodeRelativeFactor() {
  getNextToken();
  switch (token.type) {
  case TokenType::Identifier:
    return std::unique_ptr<TNode>(new TNode(TNodeType::Variable, token.name));
  case TokenType::Constant:
    return std::unique_ptr<TNode>(new TNode(TNodeType::Constant, token.name));
  default:
    return createTNodeExpression();
  }
}

std::unique_ptr<TNode> Parser::createTNodeExpression() {
  std::unique_ptr<TNode> leftTermTNode = createTNodeTerm();
  std::unique_ptr<TNode> previousTNode = NULL;
  TokenType nextTokenType = tokenQueue.front().type;
  while (nextTokenType == TokenType::Plus ||
         nextTokenType == TokenType::Minus) {
    std::unique_ptr<TNode> rightTermTNode;
    switch (nextTokenType) {
    case TokenType::Plus:
      expectToken("+");
      rightTermTNode = createTNodeTerm();
      if (previousTNode == NULL) {
        previousTNode = std::unique_ptr<TNode>(
            new TNode(TNodeType::Plus,
                      {std::move(leftTermTNode), std::move(rightTermTNode)}));
      } else {
        previousTNode = std::unique_ptr<TNode>(
            new TNode(TNodeType::Plus,
                      {std::move(previousTNode), std::move(rightTermTNode)}));
      }
      break;
    case TokenType::Minus:
      expectToken("-");
      rightTermTNode = createTNodeTerm();
      if (previousTNode == NULL) {
        previousTNode = std::unique_ptr<TNode>(
            new TNode(TNodeType::Minus,
                      {std::move(leftTermTNode), std::move(rightTermTNode)}));
      } else {
        previousTNode = std::unique_ptr<TNode>(
            new TNode(TNodeType::Minus,
                      {std::move(previousTNode), std::move(rightTermTNode)}));
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
    switch (nextTokenType) {
    case TokenType::Multiply:
      expectToken("*");
      rightFactorTNode = createTNodeFactor();
      if (previousTNode == NULL) {
        previousTNode = std::unique_ptr<TNode>(
            new TNode(TNodeType::Multiply, {std::move(leftFactorTNode),
                                            std::move(rightFactorTNode)}));
      } else {
        previousTNode = std::unique_ptr<TNode>(
            new TNode(TNodeType::Multiply,
                      {std::move(previousTNode), std::move(rightFactorTNode)}));
      }
      break;
    case TokenType::Divide:
      expectToken("/");
      rightFactorTNode = createTNodeFactor();
      if (previousTNode == NULL) {
        previousTNode = std::unique_ptr<TNode>(
            new TNode(TNodeType::Divide, {std::move(leftFactorTNode),
                                          std::move(rightFactorTNode)}));
      } else {
        previousTNode = std::unique_ptr<TNode>(
            new TNode(TNodeType::Divide,
                      {std::move(previousTNode), std::move(rightFactorTNode)}));
      }
      break;
    case TokenType::Mod:
      expectToken("%");
      rightFactorTNode = createTNodeFactor();
      if (previousTNode == NULL) {
        previousTNode = std::unique_ptr<TNode>(
            new TNode(TNodeType::Mod, {std::move(leftFactorTNode),
                                       std::move(rightFactorTNode)}));
      } else {
        previousTNode = std::unique_ptr<TNode>(
            new TNode(TNodeType::Mod,
                      {std::move(previousTNode), std::move(rightFactorTNode)}));
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
    return std::unique_ptr<TNode>(new TNode(TNodeType::Constant, token.name));
  case TokenType::OpenParenthesis:
    std::unique_ptr<TNode> expressionTNode = createTNodeExpression();
    expectToken(")");
    return expressionTNode;
  }
}
