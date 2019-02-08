#include "Parser.h"

#include "catch.hpp"

SCENARIO("Procedure and assignment statements only") {

  Parser parser;
  queue<Token> tokenQueue;
  tokenQueue.push(Token(TokenType::Procedure, "procedure", 1));
  tokenQueue.push(Token(TokenType::Identifier, "Example", 1));
  tokenQueue.push(Token(TokenType::Separator, "{", 1));
  tokenQueue.push(Token(TokenType::Identifier, "x", 2));
  tokenQueue.push(Token(TokenType::Equal, "=", 2));
  tokenQueue.push(Token(TokenType::Constant, "2", 2));
  tokenQueue.push(Token(TokenType::Separator, ";", 2));
  tokenQueue.push(Token(TokenType::Separator, "}", 2));
  TNode *root = parser.buildAST(tokenQueue);
  TNode *stmtLst = root->getChildren().front();
  TNode *assignTNode = stmtLst->getChildren().front();

  SECTION("Root of AST has one StatementList child") {
    REQUIRE(stmtLst->getType() == TNodeType::StatementList);
  }

  SECTION("StatementList has one Assign child") {
    REQUIRE(assignTNode->getType() == TNodeType::Assign);
  }

  SECTION("Assign has Variable and Constant children") {
    TNode *variableTNode = assignTNode->getChildren().front();
    TNode *constantTNode = assignTNode->getChildren().back();
    REQUIRE(variableTNode->getType() == TNodeType::Variable);
    REQUIRE(variableTNode->getName() == "x");
    REQUIRE(constantTNode->getType() == TNodeType::Constant);
    REQUIRE(constantTNode->getName() == "2");
  }
}

SCENARIO("Procedure and read statements only") {

  Parser parser;
  queue<Token> tokenQueue;
  tokenQueue.push(Token(TokenType::Procedure, "procedure", 1));
  tokenQueue.push(Token(TokenType::Identifier, "Example", 1));
  tokenQueue.push(Token(TokenType::Separator, "{", 1));
  tokenQueue.push(Token(TokenType::Read, "read", 2));
  tokenQueue.push(Token(TokenType::Identifier, "x", 2));
  tokenQueue.push(Token(TokenType::Separator, ";", 2));
  tokenQueue.push(Token(TokenType::Separator, "}", 2));
  TNode *root = parser.buildAST(tokenQueue);
  TNode *stmtLst = root->getChildren().front();
  TNode *readTNode = stmtLst->getChildren().front();

  SECTION("Root of AST has one StatementList child") {
    REQUIRE(stmtLst->getType() == TNodeType::StatementList);
  }

  SECTION("StatementList has one Read child") {
    REQUIRE(readTNode->getType() == TNodeType::Read);
  }

  SECTION("Read has one Variable child") {
    TNode *variableTNode = readTNode->getChildren().front();
    REQUIRE(variableTNode->getType() == TNodeType::Variable);
    REQUIRE(variableTNode->getName() == "x");
  }
}

SCENARIO("Procedure and print statements only") {

  Parser parser;
  queue<Token> tokenQueue;
  tokenQueue.push(Token(TokenType::Procedure, "procedure", 1));
  tokenQueue.push(Token(TokenType::Identifier, "Example", 1));
  tokenQueue.push(Token(TokenType::Separator, "{", 1));
  tokenQueue.push(Token(TokenType::Print, "print", 2));
  tokenQueue.push(Token(TokenType::Identifier, "x", 2));
  tokenQueue.push(Token(TokenType::Separator, ";", 2));
  tokenQueue.push(Token(TokenType::Separator, "}", 2));
  TNode *root = parser.buildAST(tokenQueue);
  TNode *stmtLst = root->getChildren().front();
  TNode *printTNode = stmtLst->getChildren().front();

  SECTION("Root of AST has one StatementList child") {
    REQUIRE(stmtLst->getType() == TNodeType::StatementList);
  }

  SECTION("StatementList has one Print child") {
    REQUIRE(printTNode->getType() == TNodeType::Print);
  }

  SECTION("Print has one Variable child") {
    TNode *variableTNode = printTNode->getChildren().front();
    REQUIRE(variableTNode->getType() == TNodeType::Variable);
    REQUIRE(variableTNode->getName() == "x");
  }
}

SCENARIO("Procedure and while statements only") {

  Parser parser;
  queue<Token> tokenQueue;
  tokenQueue.push(Token(TokenType::Procedure, "procedure", 1));
  tokenQueue.push(Token(TokenType::Identifier, "Example", 1));
  tokenQueue.push(Token(TokenType::Separator, "{", 1));
  tokenQueue.push(Token(TokenType::While, "while", 2));
  tokenQueue.push(Token(TokenType::OpenParenthesis, "(", 2));
  tokenQueue.push(Token(TokenType::Identifier, "x", 2));
  tokenQueue.push(Token(TokenType::Equal, "==", 2));
  tokenQueue.push(Token(TokenType::Constant, "0", 2));
  tokenQueue.push(Token(TokenType::CloseParenthesis, ")", 2));
  tokenQueue.push(Token(TokenType::Separator, "{", 2));
  tokenQueue.push(Token(TokenType::Read, "read", 3));
  tokenQueue.push(Token(TokenType::Identifier, "x", 3));
  tokenQueue.push(Token(TokenType::Separator, ";", 3));
  tokenQueue.push(Token(TokenType::Separator, "}", 3));
  tokenQueue.push(Token(TokenType::Separator, "}", 3));
  TNode *root = parser.buildAST(tokenQueue);
  TNode *stmtLst = root->getChildren().front();
  TNode *whileTNode = stmtLst->getChildren().front();
  TNode *whileCondExpr = whileTNode->getChildren().front();
  TNode *whileStmtLst = whileTNode->getChildren().back();
  TNode *readTNode = whileStmtLst->getChildren().front();

  SECTION("Root of AST has one StatementList child") {
    REQUIRE(stmtLst->getType() == TNodeType::StatementList);
  }

  SECTION("StatementList has one While child") {
    REQUIRE(whileTNode->getType() == TNodeType::While);
  }

  SECTION("While has ConditionExpression and StatementList children") {
    REQUIRE(whileCondExpr->getType() == TNodeType::Equal);
    REQUIRE(whileStmtLst->getType() == TNodeType::StatementList);
  }

  SECTION("ConditionExpression has Variable and Constant children") {
    TNode *left = whileCondExpr->getChildren().front();
    TNode *right = whileCondExpr->getChildren().back();
    REQUIRE(left->getType() == TNodeType::Variable);
    REQUIRE(left->getName() == "x");
    REQUIRE(right->getType() == TNodeType::Constant);
    REQUIRE(right->getName() == "0");
  }

  SECTION("WhileStatementList has one Read child") {
    REQUIRE(readTNode->getType() == TNodeType::Read);

    TNode *variableTNode = readTNode->getChildren().front();
    REQUIRE(variableTNode->getType() == TNodeType::Variable);
    REQUIRE(variableTNode->getName() == "x");
  }
}

SCENARIO("Procedure and if statements only") {

  Parser parser;
  queue<Token> tokenQueue;
  tokenQueue.push(Token(TokenType::Procedure, "procedure", 1));
  tokenQueue.push(Token(TokenType::Identifier, "Example", 1));
  tokenQueue.push(Token(TokenType::Separator, "{", 1));
  tokenQueue.push(Token(TokenType::If, "if", 2));
  tokenQueue.push(Token(TokenType::OpenParenthesis, "(", 2));
  tokenQueue.push(Token(TokenType::Identifier, "x", 2));
  tokenQueue.push(Token(TokenType::Equal, "==", 2));
  tokenQueue.push(Token(TokenType::Constant, "0", 2));
  tokenQueue.push(Token(TokenType::CloseParenthesis, ")", 2));
  tokenQueue.push(Token(TokenType::Then, "then", 2));
  tokenQueue.push(Token(TokenType::Separator, "{", 2));
  tokenQueue.push(Token(TokenType::Read, "read", 3));
  tokenQueue.push(Token(TokenType::Identifier, "x", 3));
  tokenQueue.push(Token(TokenType::Separator, ";", 3));
  tokenQueue.push(Token(TokenType::Separator, "}", 3));
  tokenQueue.push(Token(TokenType::Else, "else", 4));
  tokenQueue.push(Token(TokenType::Separator, "{", 4));
  tokenQueue.push(Token(TokenType::Read, "read", 5));
  tokenQueue.push(Token(TokenType::Identifier, "y", 5));
  tokenQueue.push(Token(TokenType::Separator, ";", 5));
  tokenQueue.push(Token(TokenType::Separator, "}", 5));
  tokenQueue.push(Token(TokenType::Separator, "}", 5));
  TNode *root = parser.buildAST(tokenQueue);
  TNode *stmtLst = root->getChildren().front();
  TNode *ifTNode = stmtLst->getChildren().front();
  TNode *ifCondExpr = ifTNode->getChildren()[0];
  TNode *thenStmtLst = ifTNode->getChildren()[1];
  TNode *elseStmtLst = ifTNode->getChildren()[2];
  TNode *readXTNode = thenStmtLst->getChildren().front();
  TNode *readYTNode = elseStmtLst->getChildren().front();

  SECTION("Root of AST has one StatementList child") {
    REQUIRE(stmtLst->getType() == TNodeType::StatementList);
  }

  SECTION("StatementList has one If child") {
    REQUIRE(ifTNode->getType() == TNodeType::If);
  }

  SECTION("If has ConditionExpression, ThenStatementList, ElseStatementList "
          "children") {
    REQUIRE(ifCondExpr->getType() == TNodeType::Equal);
    REQUIRE(thenStmtLst->getType() == TNodeType::StatementList);
    REQUIRE(elseStmtLst->getType() == TNodeType::StatementList);
  }

  SECTION("ConditionExpression has Variable and Constant children") {
    TNode *left = ifCondExpr->getChildren().front();
    TNode *right = ifCondExpr->getChildren().back();
    REQUIRE(left->getType() == TNodeType::Variable);
    REQUIRE(left->getName() == "x");
    REQUIRE(right->getType() == TNodeType::Constant);
    REQUIRE(right->getName() == "0");
  }

  SECTION("ThenStatementList has one Read child") {
    REQUIRE(readXTNode->getType() == TNodeType::Read);

    TNode *variableTNode = readXTNode->getChildren().front();
    REQUIRE(variableTNode->getType() == TNodeType::Variable);
    REQUIRE(variableTNode->getName() == "x");
  }

  SECTION("ElseStatementList has one Read child") {
    REQUIRE(readYTNode->getType() == TNodeType::Read);

    TNode *variableTNode = readYTNode->getChildren().front();
    REQUIRE(variableTNode->getType() == TNodeType::Variable);
    REQUIRE(variableTNode->getName() == "y");
  }
}
