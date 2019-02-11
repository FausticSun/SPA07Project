#include "Parser.h"

#include "catch.hpp"

SCENARIO("Procedure with one assignment statement") {
  Parser parser;
  std::queue<Token> tokenQueue;
  tokenQueue.push(Token(TokenType::Procedure, "procedure"));
  tokenQueue.push(Token(TokenType::Identifier, "Example"));
  tokenQueue.push(Token(TokenType::Separator, "{"));
  tokenQueue.push(Token(TokenType::Identifier, "x"));
  tokenQueue.push(Token(TokenType::Separator, "="));
  tokenQueue.push(Token(TokenType::Constant, "0"));
  tokenQueue.push(Token(TokenType::Separator, ";"));
  tokenQueue.push(Token(TokenType::Separator, "}"));
  TNode *programTNode = parser.buildAst(tokenQueue);
  TNode *procedureTNode = programTNode->getChildren().front();
  TNode *stmtLstTNode = procedureTNode->getChildren().front();

  SECTION("procedureTNode of AST has one StatementList child") {
    REQUIRE(stmtLstTNode->getType() == TNodeType::StatementList);
  }

  TNode *assignTNode = stmtLstTNode->getChildren().front();
  SECTION("StatementList has one Assign child") {
    REQUIRE(assignTNode->getType() == TNodeType::Assign);
  }

  SECTION("Assign has Variable and Constant children") {
    TNode *variableTNode = assignTNode->getChildren().front();
    TNode *constantTNode = assignTNode->getChildren().back();
    REQUIRE(variableTNode->getType() == TNodeType::Variable);
    REQUIRE(variableTNode->getName() == "x");
    REQUIRE(constantTNode->getType() == TNodeType::Constant);
    REQUIRE(constantTNode->getName() == "0");
  }
}

SCENARIO("Procedure with one read statement") {
  Parser parser;
  std::queue<Token> tokenQueue;
  tokenQueue.push(Token(TokenType::Procedure, "procedure"));
  tokenQueue.push(Token(TokenType::Identifier, "Example"));
  tokenQueue.push(Token(TokenType::Separator, "{"));
  tokenQueue.push(Token(TokenType::Read, "read"));
  tokenQueue.push(Token(TokenType::Identifier, "x"));
  tokenQueue.push(Token(TokenType::Separator, ";"));
  tokenQueue.push(Token(TokenType::Separator, "}"));
  TNode *programTNode = parser.buildAst(tokenQueue);
  TNode *procedureTNode = programTNode->getChildren().front();
  TNode *stmtLstTNode = procedureTNode->getChildren().front();

  SECTION("procedureTNode of AST has one StatementList child") {
    REQUIRE(stmtLstTNode->getType() == TNodeType::StatementList);
  }

  TNode *readTNode = stmtLstTNode->getChildren().front();
  SECTION("StatementList has one Read child") {
    REQUIRE(readTNode->getType() == TNodeType::Read);
  }

  SECTION("Read has one Variable child") {
    TNode *variableTNode = readTNode->getChildren().front();
    REQUIRE(variableTNode->getType() == TNodeType::Variable);
    REQUIRE(variableTNode->getName() == "x");
  }
}

SCENARIO("Procedure with one print statement") {
  Parser parser;
  std::queue<Token> tokenQueue;
  tokenQueue.push(Token(TokenType::Procedure, "procedure"));
  tokenQueue.push(Token(TokenType::Identifier, "Example"));
  tokenQueue.push(Token(TokenType::Separator, "{"));
  tokenQueue.push(Token(TokenType::Print, "print"));
  tokenQueue.push(Token(TokenType::Identifier, "x"));
  tokenQueue.push(Token(TokenType::Separator, ";"));
  tokenQueue.push(Token(TokenType::Separator, "}"));
  TNode *programTNode = parser.buildAst(tokenQueue);
  TNode *procedureTNode = programTNode->getChildren().front();
  TNode *stmtLstTNode = procedureTNode->getChildren().front();

  SECTION("procedureTNode of AST has one StatementList child") {
    REQUIRE(stmtLstTNode->getType() == TNodeType::StatementList);
  }
  
  TNode *printTNode = stmtLstTNode->getChildren().front();
  SECTION("StatementList has one Print child") {
    REQUIRE(printTNode->getType() == TNodeType::Print);
  }

  SECTION("Print has one Variable child") {
    TNode *variableTNode = printTNode->getChildren().front();
    REQUIRE(variableTNode->getType() == TNodeType::Variable);
    REQUIRE(variableTNode->getName() == "x");
  }
}

SCENARIO("Procedure with one while statement") {
  Parser parser;
  std::queue<Token> tokenQueue;
  tokenQueue.push(Token(TokenType::Procedure, "procedure"));
  tokenQueue.push(Token(TokenType::Identifier, "Example"));
  tokenQueue.push(Token(TokenType::Separator, "{"));
  tokenQueue.push(Token(TokenType::While, "while"));
  tokenQueue.push(Token(TokenType::OpenParenthesis, "("));
  tokenQueue.push(Token(TokenType::Identifier, "x"));
  tokenQueue.push(Token(TokenType::Equal, "=="));
  tokenQueue.push(Token(TokenType::Constant, "0"));
  tokenQueue.push(Token(TokenType::CloseParenthesis, ")"));
  tokenQueue.push(Token(TokenType::Separator, "{"));
  tokenQueue.push(Token(TokenType::Read, "read"));
  tokenQueue.push(Token(TokenType::Identifier, "x"));
  tokenQueue.push(Token(TokenType::Separator, ";"));
  tokenQueue.push(Token(TokenType::Separator, "}"));
  tokenQueue.push(Token(TokenType::Separator, "}"));
  TNode *programTNode = parser.buildAst(tokenQueue);
  TNode *procedureTNode = programTNode->getChildren().front();
  TNode *stmtLstTNode = procedureTNode->getChildren().front();

  SECTION("procedureTNode of AST has one StatementList child") {
    REQUIRE(stmtLstTNode->getType() == TNodeType::StatementList);
  }

  TNode *whileTNode = stmtLstTNode->getChildren().front();
  SECTION("StatementList has one While child") {
    REQUIRE(whileTNode->getType() == TNodeType::While);
  }

  TNode *whileCondExprTNode = whileTNode->getChildren().front();
  TNode *whileStmtLstTNode = whileTNode->getChildren().back();
  SECTION("While has ConditionExpression and StatementList children") {
    REQUIRE(whileCondExprTNode->getType() == TNodeType::Equal);
    REQUIRE(whileStmtLstTNode->getType() == TNodeType::StatementList);
  }

  SECTION("ConditionExpression has Variable and Constant children") {
    TNode *variableTNode = whileCondExprTNode->getChildren().front();
    TNode *constantTNode = whileCondExprTNode->getChildren().back();
    REQUIRE(variableTNode->getType() == TNodeType::Variable);
    REQUIRE(variableTNode->getName() == "x");
    REQUIRE(constantTNode->getType() == TNodeType::Constant);
    REQUIRE(constantTNode->getName() == "0");
  }

  SECTION("WhileStatementList has one Read child") {
    TNode *readTNode = whileStmtLstTNode->getChildren().front();
    TNode *variableTNode = readTNode->getChildren().front();
    REQUIRE(readTNode->getType() == TNodeType::Read);
    REQUIRE(variableTNode->getType() == TNodeType::Variable);
    REQUIRE(variableTNode->getName() == "x");
  }
}

SCENARIO("Procedure with one if statement") {
  Parser parser;
  std::queue<Token> tokenQueue;
  tokenQueue.push(Token(TokenType::Procedure, "procedure"));
  tokenQueue.push(Token(TokenType::Identifier, "Example"));
  tokenQueue.push(Token(TokenType::Separator, "{"));
  tokenQueue.push(Token(TokenType::If, "if"));
  tokenQueue.push(Token(TokenType::OpenParenthesis, "("));
  tokenQueue.push(Token(TokenType::Identifier, "x"));
  tokenQueue.push(Token(TokenType::Equal, "=="));
  tokenQueue.push(Token(TokenType::Constant, "0"));
  tokenQueue.push(Token(TokenType::CloseParenthesis, ")"));
  tokenQueue.push(Token(TokenType::Then, "then"));
  tokenQueue.push(Token(TokenType::Separator, "{"));
  tokenQueue.push(Token(TokenType::Read, "read"));
  tokenQueue.push(Token(TokenType::Identifier, "x"));
  tokenQueue.push(Token(TokenType::Separator, ";"));
  tokenQueue.push(Token(TokenType::Separator, "}"));
  tokenQueue.push(Token(TokenType::Else, "else"));
  tokenQueue.push(Token(TokenType::Separator, "{"));
  tokenQueue.push(Token(TokenType::Read, "read"));
  tokenQueue.push(Token(TokenType::Identifier, "y"));
  tokenQueue.push(Token(TokenType::Separator, ";"));
  tokenQueue.push(Token(TokenType::Separator, "}"));
  tokenQueue.push(Token(TokenType::Separator, "}"));
  TNode *programTNode = parser.buildAst(tokenQueue);
  TNode *procedureTNode = programTNode->getChildren().front();
  TNode *stmtLstTNode = procedureTNode->getChildren().front();

  SECTION("procedureTNode of AST has one StatementList child") {
    REQUIRE(stmtLstTNode->getType() == TNodeType::StatementList);
  }

  TNode *ifTNode = stmtLstTNode->getChildren().front();
  SECTION("StatementList has one If child") {
    REQUIRE(ifTNode->getType() == TNodeType::If);
  }

  TNode *ifCondExprTNode = ifTNode->getChildren()[0];
  TNode *thenStmtLstTNode = ifTNode->getChildren()[1];
  TNode *elseStmtLstTNode = ifTNode->getChildren()[2];
  SECTION("If has ConditionExpression, ThenStatementList, ElseStatementList "
          "children") {
    REQUIRE(ifCondExprTNode->getType() == TNodeType::Equal);
    REQUIRE(thenStmtLstTNode->getType() == TNodeType::StatementList);
    REQUIRE(elseStmtLstTNode->getType() == TNodeType::StatementList);
  }

  SECTION("ConditionExpression has Variable and Constant children") {
    TNode *variableTNode = ifCondExprTNode->getChildren().front();
    TNode *constantTNode = ifCondExprTNode->getChildren().back();
    REQUIRE(variableTNode->getType() == TNodeType::Variable);
    REQUIRE(variableTNode->getName() == "x");
    REQUIRE(constantTNode->getType() == TNodeType::Constant);
    REQUIRE(constantTNode->getName() == "0");
  }

  SECTION("ThenStatementList has one Read child") {
    TNode *readXTNode = thenStmtLstTNode->getChildren().front();
    TNode *variableTNode = readXTNode->getChildren().front();
    REQUIRE(readXTNode->getType() == TNodeType::Read);
    REQUIRE(variableTNode->getType() == TNodeType::Variable);
    REQUIRE(variableTNode->getName() == "x");
  }

  SECTION("ElseStatementList has one Read child") {
    TNode *readYTNode = elseStmtLstTNode->getChildren().front();
    TNode *variableTNode = readYTNode->getChildren().front();
    REQUIRE(readYTNode->getType() == TNodeType::Read);
    REQUIRE(variableTNode->getType() == TNodeType::Variable);
    REQUIRE(variableTNode->getName() == "y");
  }
}

SCENARIO("Assignment statement: x = a + b + c") {
  Parser parser;
  std::queue<Token> tokenQueue;
  tokenQueue.push(Token(TokenType::Procedure, "procedure"));
  tokenQueue.push(Token(TokenType::Identifier, "Example"));
  tokenQueue.push(Token(TokenType::Separator, "{"));
  tokenQueue.push(Token(TokenType::Identifier, "x"));
  tokenQueue.push(Token(TokenType::Separator, "="));
  tokenQueue.push(Token(TokenType::Identifier, "a"));
  tokenQueue.push(Token(TokenType::Plus, "+"));
  tokenQueue.push(Token(TokenType::Identifier, "b"));
  tokenQueue.push(Token(TokenType::Plus, "+"));
  tokenQueue.push(Token(TokenType::Identifier, "c"));
  tokenQueue.push(Token(TokenType::Separator, ";"));
  tokenQueue.push(Token(TokenType::Separator, "}"));
  TNode *programTNode = parser.buildAst(tokenQueue);
  TNode *procedureTNode = programTNode->getChildren().front();
  TNode *stmtLstTNode = procedureTNode->getChildren().front();

  SECTION("procedureTNode of AST has one StatementList child") {
    REQUIRE(stmtLstTNode->getType() == TNodeType::StatementList);
  }

  TNode *assignTNode = stmtLstTNode->getChildren().front();
  SECTION("StatementList has one Assign child") {
    REQUIRE(assignTNode->getType() == TNodeType::Assign);
  }

  TNode *assignVariableTNode = assignTNode->getChildren().front();
  TNode *assignPlusTNode = assignTNode->getChildren().back();
  SECTION("Assign has Variable and Plus children") {
    REQUIRE(assignVariableTNode->getType() == TNodeType::Variable);
    REQUIRE(assignVariableTNode->getName() == "x");
    REQUIRE(assignPlusTNode->getType() == TNodeType::Plus);
  }

  TNode *plusTNode = assignPlusTNode->getChildren().front();
  TNode *variableATNode = plusTNode->getChildren().front();
  TNode *variableBTNode = plusTNode->getChildren().back();
  SECTION("Left subtree of Plus") {
    REQUIRE(plusTNode->getType() == TNodeType::Plus);
    REQUIRE(variableATNode->getType() == TNodeType::Variable);
    REQUIRE(variableATNode->getName() == "a");
    REQUIRE(variableBTNode->getType() == TNodeType::Variable);
    REQUIRE(variableBTNode->getName() == "b");
  }

  TNode *variableCTNode = assignPlusTNode->getChildren().back();
  SECTION("Right subtree of Plus") {
    REQUIRE(variableCTNode->getType() == TNodeType::Variable);
    REQUIRE(variableCTNode->getName() == "c");
  }
}

SCENARIO("Assignment statement: x = a + b * c") {
  Parser parser;
  std::queue<Token> tokenQueue;
  tokenQueue.push(Token(TokenType::Procedure, "procedure"));
  tokenQueue.push(Token(TokenType::Identifier, "Example"));
  tokenQueue.push(Token(TokenType::Separator, "{"));
  tokenQueue.push(Token(TokenType::Identifier, "x"));
  tokenQueue.push(Token(TokenType::Separator, "="));
  tokenQueue.push(Token(TokenType::Identifier, "a"));
  tokenQueue.push(Token(TokenType::Plus, "+"));
  tokenQueue.push(Token(TokenType::Identifier, "b"));
  tokenQueue.push(Token(TokenType::Multiply, "*"));
  tokenQueue.push(Token(TokenType::Identifier, "c"));
  tokenQueue.push(Token(TokenType::Separator, ";"));
  tokenQueue.push(Token(TokenType::Separator, "}"));
  TNode *programTNode = parser.buildAst(tokenQueue);
  TNode *procedureTNode = programTNode->getChildren().front();
  TNode *stmtLstTNode = procedureTNode->getChildren().front();

  SECTION("procedureTNode of AST has one StatementList child") {
    REQUIRE(stmtLstTNode->getType() == TNodeType::StatementList);
  }

  TNode *assignTNode = stmtLstTNode->getChildren().front();
  SECTION("StatementList has one Assign child") {
    REQUIRE(assignTNode->getType() == TNodeType::Assign);
  }

  TNode *assignVariableTNode = assignTNode->getChildren().front();
  TNode *assignPlusTNode = assignTNode->getChildren().back();
  SECTION("Assign has Variable and Plus children") {
    REQUIRE(assignVariableTNode->getType() == TNodeType::Variable);
    REQUIRE(assignVariableTNode->getName() == "x");
    REQUIRE(assignPlusTNode->getType() == TNodeType::Plus);
  }

  TNode *multiplyTNode = assignPlusTNode->getChildren().back();
  TNode *variableBTNode = multiplyTNode->getChildren().front();
  TNode *variableCTNode = multiplyTNode->getChildren().back();
  SECTION("Right subtree of Plus") {
    REQUIRE(multiplyTNode->getType() == TNodeType::Multiply);
    REQUIRE(variableBTNode->getType() == TNodeType::Variable);
    REQUIRE(variableBTNode->getName() == "b");
    REQUIRE(variableCTNode->getType() == TNodeType::Variable);
    REQUIRE(variableCTNode->getName() == "c");
  }

  TNode *variableATNode = assignPlusTNode->getChildren().front();
  SECTION("Left subtree of Plus") {
    REQUIRE(variableATNode->getType() == TNodeType::Variable);
    REQUIRE(variableATNode->getName() == "a");
  }
}

SCENARIO("Assignment statement: x = (a + b) * c") {
  Parser parser;
  std::queue<Token> tokenQueue;
  tokenQueue.push(Token(TokenType::Procedure, "procedure"));
  tokenQueue.push(Token(TokenType::Identifier, "Example"));
  tokenQueue.push(Token(TokenType::Separator, "{"));
  tokenQueue.push(Token(TokenType::Identifier, "x"));
  tokenQueue.push(Token(TokenType::Separator, "="));
  tokenQueue.push(Token(TokenType::OpenParenthesis, "("));
  tokenQueue.push(Token(TokenType::Identifier, "a"));
  tokenQueue.push(Token(TokenType::Plus, "+"));
  tokenQueue.push(Token(TokenType::Identifier, "b"));
  tokenQueue.push(Token(TokenType::CloseParenthesis, ")"));
  tokenQueue.push(Token(TokenType::Multiply, "*"));
  tokenQueue.push(Token(TokenType::Identifier, "c"));
  tokenQueue.push(Token(TokenType::Separator, ";"));
  tokenQueue.push(Token(TokenType::Separator, "}"));
  TNode *programTNode = parser.buildAst(tokenQueue);
  TNode *procedureTNode = programTNode->getChildren().front();
  TNode *stmtLstTNode = procedureTNode->getChildren().front();

  SECTION("procedureTNode of AST has one StatementList child") {
    REQUIRE(stmtLstTNode->getType() == TNodeType::StatementList);
  }

  TNode *assignTNode = stmtLstTNode->getChildren().front();
  SECTION("StatementList has one Assign child") {
    REQUIRE(assignTNode->getType() == TNodeType::Assign);
  }

  TNode *assignVariableTNode = assignTNode->getChildren().front();
  TNode *assignMultiplyTNode = assignTNode->getChildren().back();
  SECTION("Assign has Variable and Plus children") {
    REQUIRE(assignVariableTNode->getType() == TNodeType::Variable);
    REQUIRE(assignVariableTNode->getName() == "x");
    REQUIRE(assignMultiplyTNode->getType() == TNodeType::Multiply);
  }

  TNode *plusTNode = assignMultiplyTNode->getChildren().front();
  TNode *variableATNode = plusTNode->getChildren().front();
  TNode *variableBTNode = plusTNode->getChildren().back();
  SECTION("Left subtree of Multiply") {
    REQUIRE(plusTNode->getType() == TNodeType::Plus);
    REQUIRE(variableATNode->getType() == TNodeType::Variable);
    REQUIRE(variableATNode->getName() == "a");
    REQUIRE(variableBTNode->getType() == TNodeType::Variable);
    REQUIRE(variableBTNode->getName() == "b");
  }

  TNode *variableCTNode = assignMultiplyTNode->getChildren().back();
  SECTION("Right subtree of Multiply") {
    REQUIRE(variableCTNode->getType() == TNodeType::Variable);
    REQUIRE(variableCTNode->getName() == "c");
  }
}

SCENARIO("Assignment statement: x = a * b + c") {
  Parser parser;
  std::queue<Token> tokenQueue;
  tokenQueue.push(Token(TokenType::Procedure, "procedure"));
  tokenQueue.push(Token(TokenType::Identifier, "Example"));
  tokenQueue.push(Token(TokenType::Separator, "{"));
  tokenQueue.push(Token(TokenType::Identifier, "x"));
  tokenQueue.push(Token(TokenType::Separator, "="));
  tokenQueue.push(Token(TokenType::Identifier, "a"));
  tokenQueue.push(Token(TokenType::Multiply, "*"));
  tokenQueue.push(Token(TokenType::Identifier, "b"));
  tokenQueue.push(Token(TokenType::Plus, "+"));
  tokenQueue.push(Token(TokenType::Identifier, "c"));
  tokenQueue.push(Token(TokenType::Separator, ";"));
  tokenQueue.push(Token(TokenType::Separator, "}"));
  TNode *programTNode = parser.buildAst(tokenQueue);
  TNode *procedureTNode = programTNode->getChildren().front();
  TNode *stmtLstTNode = procedureTNode->getChildren().front();

  SECTION("procedureTNode of AST has one StatementList child") {
    REQUIRE(stmtLstTNode->getType() == TNodeType::StatementList);
  }

  TNode *assignTNode = stmtLstTNode->getChildren().front();
  SECTION("StatementList has one Assign child") {
    REQUIRE(assignTNode->getType() == TNodeType::Assign);
  }

  TNode *assignVariableTNode = assignTNode->getChildren().front();
  TNode *assignPlusTNode = assignTNode->getChildren().back();
  SECTION("Assign has Variable and Plus children") {
    REQUIRE(assignVariableTNode->getType() == TNodeType::Variable);
    REQUIRE(assignVariableTNode->getName() == "x");
    REQUIRE(assignPlusTNode->getType() == TNodeType::Plus);
  }

  TNode *multiplyTNode = assignPlusTNode->getChildren().front();
  TNode *variableATNode = multiplyTNode->getChildren().front();
  TNode *variableBTNode = multiplyTNode->getChildren().back();
  SECTION("Left subtree of Plus") {
    REQUIRE(multiplyTNode->getType() == TNodeType::Multiply);
    REQUIRE(variableATNode->getType() == TNodeType::Variable);
    REQUIRE(variableATNode->getName() == "a");
    REQUIRE(variableBTNode->getType() == TNodeType::Variable);
    REQUIRE(variableBTNode->getName() == "b");
  }

  TNode *variableCTNode = assignPlusTNode->getChildren().back();
  SECTION("Right subtree of Plus") {
    REQUIRE(variableCTNode->getType() == TNodeType::Variable);
    REQUIRE(variableCTNode->getName() == "c");
  }
}

SCENARIO("Assignment statement: x = a * (b + c)") {
  Parser parser;
  std::queue<Token> tokenQueue;
  tokenQueue.push(Token(TokenType::Procedure, "procedure"));
  tokenQueue.push(Token(TokenType::Identifier, "Example"));
  tokenQueue.push(Token(TokenType::Separator, "{"));
  tokenQueue.push(Token(TokenType::Identifier, "x"));
  tokenQueue.push(Token(TokenType::Separator, "="));
  tokenQueue.push(Token(TokenType::Identifier, "a"));
  tokenQueue.push(Token(TokenType::Multiply, "*"));
  tokenQueue.push(Token(TokenType::OpenParenthesis, "("));
  tokenQueue.push(Token(TokenType::Identifier, "b"));
  tokenQueue.push(Token(TokenType::Plus, "+"));
  tokenQueue.push(Token(TokenType::Identifier, "c"));
  tokenQueue.push(Token(TokenType::CloseParenthesis, ")"));
  tokenQueue.push(Token(TokenType::Separator, ";"));
  tokenQueue.push(Token(TokenType::Separator, "}"));
  TNode *programTNode = parser.buildAst(tokenQueue);
  TNode *procedureTNode = programTNode->getChildren().front();
  TNode *stmtLstTNode = procedureTNode->getChildren().front();

  SECTION("procedureTNode of AST has one StatementList child") {
    REQUIRE(stmtLstTNode->getType() == TNodeType::StatementList);
  }

  TNode *assignTNode = stmtLstTNode->getChildren().front();
  SECTION("StatementList has one Assign child") {
    REQUIRE(assignTNode->getType() == TNodeType::Assign);
  }

  TNode *assignVariableTNode = assignTNode->getChildren().front();
  TNode *assignMultiplyTNode = assignTNode->getChildren().back();
  SECTION("Assign has Variable and Plus children") {
    REQUIRE(assignVariableTNode->getType() == TNodeType::Variable);
    REQUIRE(assignVariableTNode->getName() == "x");
    REQUIRE(assignMultiplyTNode->getType() == TNodeType::Multiply);
  }

  TNode *plusTNode = assignMultiplyTNode->getChildren().back();
  TNode *variableBTNode = plusTNode->getChildren().front();
  TNode *variableCTNode = plusTNode->getChildren().back();
  SECTION("Right subtree of Plus") {
    REQUIRE(plusTNode->getType() == TNodeType::Plus);
    REQUIRE(variableBTNode->getType() == TNodeType::Variable);
    REQUIRE(variableBTNode->getName() == "b");
    REQUIRE(variableCTNode->getType() == TNodeType::Variable);
    REQUIRE(variableCTNode->getName() == "c");
  }

  TNode *variableATNode = assignMultiplyTNode->getChildren().front();
  SECTION("Left subtree of Plus") {
    REQUIRE(variableATNode->getType() == TNodeType::Variable);
    REQUIRE(variableATNode->getName() == "a");
  }
}

SCENARIO("Assignment statement: x = a * b + c + d") {
  Parser parser;
  std::queue<Token> tokenQueue;
  tokenQueue.push(Token(TokenType::Procedure, "procedure"));
  tokenQueue.push(Token(TokenType::Identifier, "Example"));
  tokenQueue.push(Token(TokenType::Separator, "{"));
  tokenQueue.push(Token(TokenType::Identifier, "x"));
  tokenQueue.push(Token(TokenType::Separator, "="));
  tokenQueue.push(Token(TokenType::Identifier, "a"));
  tokenQueue.push(Token(TokenType::Multiply, "*"));
  tokenQueue.push(Token(TokenType::Identifier, "b"));
  tokenQueue.push(Token(TokenType::Plus, "+"));
  tokenQueue.push(Token(TokenType::Identifier, "c"));
  tokenQueue.push(Token(TokenType::Plus, "+"));
  tokenQueue.push(Token(TokenType::Identifier, "d"));
  tokenQueue.push(Token(TokenType::Separator, ";"));
  tokenQueue.push(Token(TokenType::Separator, "}"));
  TNode *programTNode = parser.buildAst(tokenQueue);
  TNode *procedureTNode = programTNode->getChildren().front();
  TNode *stmtLstTNode = procedureTNode->getChildren().front();

  SECTION("procedureTNode of AST has one StatementList child") {
    REQUIRE(stmtLstTNode->getType() == TNodeType::StatementList);
  }

  TNode *assignTNode = stmtLstTNode->getChildren().front();
  SECTION("StatementList has one Assign child") {
    REQUIRE(assignTNode->getType() == TNodeType::Assign);
  }

  TNode *assignVariableTNode = assignTNode->getChildren().front();
  TNode *assignPlusTNode = assignTNode->getChildren().back();
  SECTION("Assign has Variable and Plus children") {
    REQUIRE(assignVariableTNode->getType() == TNodeType::Variable);
    REQUIRE(assignVariableTNode->getName() == "x");
    REQUIRE(assignPlusTNode->getType() == TNodeType::Plus);
  }

  TNode *plusTNode = assignPlusTNode->getChildren().front();
  TNode *multiplyTNode = plusTNode->getChildren().front();
  TNode *variableCTNode = plusTNode->getChildren().back();
  SECTION("Left subtree of Plus") {
    REQUIRE(plusTNode->getType() == TNodeType::Plus);
    REQUIRE(multiplyTNode->getType() == TNodeType::Multiply);
    REQUIRE(variableCTNode->getType() == TNodeType::Variable);
    REQUIRE(variableCTNode->getName() == "c");
  }

  TNode *variableATNode = multiplyTNode->getChildren().front();
  TNode *variableBTNode = multiplyTNode->getChildren().back();
  SECTION("Left of left subtree of Plus") {
    REQUIRE(variableATNode->getType() == TNodeType::Variable);
    REQUIRE(variableATNode->getName() == "a");
    REQUIRE(variableBTNode->getType() == TNodeType::Variable);
    REQUIRE(variableBTNode->getName() == "b");
  }

  TNode *variableDTNode = assignPlusTNode->getChildren().back();
  SECTION("Right subtree of Plus") {
    REQUIRE(variableDTNode->getType() == TNodeType::Variable);
    REQUIRE(variableDTNode->getName() == "d");
  }
}

SCENARIO("Assignment statement: x = a + b * c + d") {
  Parser parser;
  std::queue<Token> tokenQueue;
  tokenQueue.push(Token(TokenType::Procedure, "procedure"));
  tokenQueue.push(Token(TokenType::Identifier, "Example"));
  tokenQueue.push(Token(TokenType::Separator, "{"));
  tokenQueue.push(Token(TokenType::Identifier, "x"));
  tokenQueue.push(Token(TokenType::Separator, "="));
  tokenQueue.push(Token(TokenType::Identifier, "a"));
  tokenQueue.push(Token(TokenType::Plus, "+"));
  tokenQueue.push(Token(TokenType::Identifier, "b"));
  tokenQueue.push(Token(TokenType::Multiply, "*"));
  tokenQueue.push(Token(TokenType::Identifier, "c"));
  tokenQueue.push(Token(TokenType::Plus, "+"));
  tokenQueue.push(Token(TokenType::Identifier, "d"));
  tokenQueue.push(Token(TokenType::Separator, ";"));
  tokenQueue.push(Token(TokenType::Separator, "}"));
  TNode *programTNode = parser.buildAst(tokenQueue);
  TNode *procedureTNode = programTNode->getChildren().front();
  TNode *stmtLstTNode = procedureTNode->getChildren().front();

  SECTION("procedureTNode of AST has one StatementList child") {
    REQUIRE(stmtLstTNode->getType() == TNodeType::StatementList);
  }

  TNode *assignTNode = stmtLstTNode->getChildren().front();
  SECTION("StatementList has one Assign child") {
    REQUIRE(assignTNode->getType() == TNodeType::Assign);
  }

  TNode *assignVariableTNode = assignTNode->getChildren().front();
  TNode *assignPlusTNode = assignTNode->getChildren().back();
  SECTION("Assign has Variable and Plus children") {
    REQUIRE(assignVariableTNode->getType() == TNodeType::Variable);
    REQUIRE(assignVariableTNode->getName() == "x");
    REQUIRE(assignPlusTNode->getType() == TNodeType::Plus);
  }

  TNode *plusTNode = assignPlusTNode->getChildren().front();
  TNode *multiplyTNode = plusTNode->getChildren().back();
  TNode *variableATNode = plusTNode->getChildren().front();
  SECTION("Left subtree of Plus") {
    REQUIRE(plusTNode->getType() == TNodeType::Plus);
    REQUIRE(multiplyTNode->getType() == TNodeType::Multiply);
    REQUIRE(variableATNode->getType() == TNodeType::Variable);
    REQUIRE(variableATNode->getName() == "a");
  }

  TNode *variableBTNode = multiplyTNode->getChildren().front();
  TNode *variableCTNode = multiplyTNode->getChildren().back();
  SECTION("Right of left subtree of Plus") {
	  REQUIRE(variableBTNode->getType() == TNodeType::Variable);
	  REQUIRE(variableBTNode->getName() == "b");
	  REQUIRE(variableCTNode->getType() == TNodeType::Variable);
	  REQUIRE(variableCTNode->getName() == "c");
  }

  TNode *variableDTNode = assignPlusTNode->getChildren().back();
  SECTION("Right subtree of Plus") {
	  REQUIRE(variableDTNode->getType() == TNodeType::Variable);
	  REQUIRE(variableDTNode->getName() == "d");
  }
}

SCENARIO("Assignment statement: x = a + b + c * d") {
  Parser parser;
  std::queue<Token> tokenQueue;
  tokenQueue.push(Token(TokenType::Procedure, "procedure"));
  tokenQueue.push(Token(TokenType::Identifier, "Example"));
  tokenQueue.push(Token(TokenType::Separator, "{"));
  tokenQueue.push(Token(TokenType::Identifier, "x"));
  tokenQueue.push(Token(TokenType::Separator, "="));
  tokenQueue.push(Token(TokenType::Identifier, "a"));
  tokenQueue.push(Token(TokenType::Plus, "+"));
  tokenQueue.push(Token(TokenType::Identifier, "b"));
  tokenQueue.push(Token(TokenType::Plus, "+"));
  tokenQueue.push(Token(TokenType::Identifier, "c"));
  tokenQueue.push(Token(TokenType::Multiply, "*"));
  tokenQueue.push(Token(TokenType::Identifier, "d"));
  tokenQueue.push(Token(TokenType::Separator, ";"));
  tokenQueue.push(Token(TokenType::Separator, "}"));
  TNode *programTNode = parser.buildAst(tokenQueue);
  TNode *procedureTNode = programTNode->getChildren().front();
  TNode *stmtLstTNode = procedureTNode->getChildren().front();

  SECTION("procedureTNode of AST has one StatementList child") {
    REQUIRE(stmtLstTNode->getType() == TNodeType::StatementList);
  }

  TNode *assignTNode = stmtLstTNode->getChildren().front();
  SECTION("StatementList has one Assign child") {
    REQUIRE(assignTNode->getType() == TNodeType::Assign);
  }

  TNode *assignVariableTNode = assignTNode->getChildren().front();
  TNode *assignPlusTNode = assignTNode->getChildren().back();
  SECTION("Assign has Variable and Plus children") {
    REQUIRE(assignVariableTNode->getType() == TNodeType::Variable);
    REQUIRE(assignVariableTNode->getName() == "x");
    REQUIRE(assignPlusTNode->getType() == TNodeType::Plus);
  }

  TNode *plusTNode = assignPlusTNode->getChildren().front();
  TNode *variableATNode = plusTNode->getChildren().front();
  TNode *variableBTNode = plusTNode->getChildren().back();
  SECTION("Left subtree of Plus") {
    REQUIRE(plusTNode->getType() == TNodeType::Plus);
    REQUIRE(variableATNode->getType() == TNodeType::Variable);
    REQUIRE(variableATNode->getName() == "a");
    REQUIRE(variableBTNode->getType() == TNodeType::Variable);
    REQUIRE(variableBTNode->getName() == "b");
  }

  TNode *multiplyTNode = assignPlusTNode->getChildren().back();
  TNode *variableCTNode = multiplyTNode->getChildren().front();
  TNode *variableDTNode = multiplyTNode->getChildren().back();
  SECTION("Right subtree of Plus") {
    REQUIRE(multiplyTNode->getType() == TNodeType::Multiply);
    REQUIRE(variableCTNode->getType() == TNodeType::Variable);
    REQUIRE(variableCTNode->getName() == "c");
    REQUIRE(variableDTNode->getType() == TNodeType::Variable);
    REQUIRE(variableDTNode->getName() == "d");
  }
}

SCENARIO("Condition Expressions: Not") {
  Parser parser;
  std::queue<Token> tokenQueue;
  tokenQueue.push(Token(TokenType::Procedure, "procedure"));
  tokenQueue.push(Token(TokenType::Identifier, "Example"));
  tokenQueue.push(Token(TokenType::Separator, "{"));
  tokenQueue.push(Token(TokenType::While, "while"));
  tokenQueue.push(Token(TokenType::OpenParenthesis, "("));
  tokenQueue.push(Token(TokenType::ExclamationMark, "!"));
  tokenQueue.push(Token(TokenType::OpenParenthesis, "("));
  tokenQueue.push(Token(TokenType::Identifier, "x"));
  tokenQueue.push(Token(TokenType::Equal, "=="));
  tokenQueue.push(Token(TokenType::Constant, "0"));
  tokenQueue.push(Token(TokenType::CloseParenthesis, ")"));
  tokenQueue.push(Token(TokenType::CloseParenthesis, ")"));
  tokenQueue.push(Token(TokenType::Separator, "{"));
  tokenQueue.push(Token(TokenType::Read, "read"));
  tokenQueue.push(Token(TokenType::Identifier, "x"));
  tokenQueue.push(Token(TokenType::Separator, ";"));
  tokenQueue.push(Token(TokenType::Separator, "}"));
  tokenQueue.push(Token(TokenType::Separator, "}"));
  TNode *programTNode = parser.buildAst(tokenQueue);
  TNode *procedureTNode = programTNode->getChildren().front();
  TNode *stmtLstTNode = procedureTNode->getChildren().front();

  SECTION("procedureTNode of AST has one StatementList child") {
    REQUIRE(stmtLstTNode->getType() == TNodeType::StatementList);
  }

  TNode *whileTNode = stmtLstTNode->getChildren().front();
  SECTION("StatementList has one While child") {
    REQUIRE(whileTNode->getType() == TNodeType::While);
  }

  TNode *whileCondExprNotTNode = whileTNode->getChildren().front();
  TNode *whileStmtLstTNode = whileTNode->getChildren().back();
  SECTION("While has ConditionExpression and StatementList children") {
    REQUIRE(whileCondExprNotTNode->getType() == TNodeType::Not);
    REQUIRE(whileStmtLstTNode->getType() == TNodeType::StatementList);
  }

  SECTION("ConditionExpression has Variable and Constant children") {
    TNode *whileCondExprEqualTNode = whileCondExprNotTNode->getChildren().front();
    TNode *variableTNode = whileCondExprEqualTNode->getChildren().front();
    TNode *constantTNode = whileCondExprEqualTNode->getChildren().back();
    REQUIRE(whileCondExprEqualTNode->getType() == TNodeType::Equal);
    REQUIRE(variableTNode->getType() == TNodeType::Variable);
    REQUIRE(variableTNode->getName() == "x");
    REQUIRE(constantTNode->getType() == TNodeType::Constant);
    REQUIRE(constantTNode->getName() == "0");
  }

  SECTION("WhileStatementList has one Read child") {
    TNode *readTNode = whileStmtLstTNode->getChildren().front();
    TNode *variableTNode = readTNode->getChildren().front();
    REQUIRE(readTNode->getType() == TNodeType::Read);
    REQUIRE(variableTNode->getType() == TNodeType::Variable);
    REQUIRE(variableTNode->getName() == "x");
  }
}

SCENARIO("Condition Expressions: And") {
  Parser parser;
  std::queue<Token> tokenQueue;
  tokenQueue.push(Token(TokenType::Procedure, "procedure"));
  tokenQueue.push(Token(TokenType::Identifier, "Example"));
  tokenQueue.push(Token(TokenType::Separator, "{"));
  tokenQueue.push(Token(TokenType::While, "while"));
  tokenQueue.push(Token(TokenType::OpenParenthesis, "("));
  tokenQueue.push(Token(TokenType::OpenParenthesis, "("));
  tokenQueue.push(Token(TokenType::Identifier, "x"));
  tokenQueue.push(Token(TokenType::Equal, "=="));
  tokenQueue.push(Token(TokenType::Constant, "0"));
  tokenQueue.push(Token(TokenType::CloseParenthesis, ")"));
  tokenQueue.push(Token(TokenType::And, "&&"));
  tokenQueue.push(Token(TokenType::OpenParenthesis, "("));
  tokenQueue.push(Token(TokenType::Identifier, "y"));
  tokenQueue.push(Token(TokenType::Equal, "=="));
  tokenQueue.push(Token(TokenType::Constant, "0"));
  tokenQueue.push(Token(TokenType::CloseParenthesis, ")"));
  tokenQueue.push(Token(TokenType::CloseParenthesis, ")"));
  tokenQueue.push(Token(TokenType::Separator, "{"));
  tokenQueue.push(Token(TokenType::Read, "read"));
  tokenQueue.push(Token(TokenType::Identifier, "x"));
  tokenQueue.push(Token(TokenType::Separator, ";"));
  tokenQueue.push(Token(TokenType::Separator, "}"));
  tokenQueue.push(Token(TokenType::Separator, "}"));
  TNode *programTNode = parser.buildAst(tokenQueue);
  TNode *procedureTNode = programTNode->getChildren().front();
  TNode *stmtLstTNode = procedureTNode->getChildren().front();

  SECTION("procedureTNode of AST has one StatementList child") {
    REQUIRE(stmtLstTNode->getType() == TNodeType::StatementList);
  }

  TNode *whileTNode = stmtLstTNode->getChildren().front();
  SECTION("StatementList has one While child") {
    REQUIRE(whileTNode->getType() == TNodeType::While);
  }

  TNode *whileCondExprAndTNode = whileTNode->getChildren().front();
  TNode *whileStmtLstTNode = whileTNode->getChildren().back();
  SECTION("While has ConditionExpression and StatementList children") {
    REQUIRE(whileCondExprAndTNode->getType() == TNodeType::And);
    REQUIRE(whileStmtLstTNode->getType() == TNodeType::StatementList);
  }

  SECTION("ConditionExpression has two Equal children") {
    TNode *whileCondExprLeftEqualTNode = whileCondExprAndTNode->getChildren().front();
    TNode *whileCondExprRightEqualTNode = whileCondExprAndTNode->getChildren().back();
    TNode *leftCondExprVariableTNode = whileCondExprLeftEqualTNode->getChildren().front();
    TNode *leftCondExprConstantTNode = whileCondExprLeftEqualTNode->getChildren().back();
	TNode *rightCondExprVariableTNode = whileCondExprRightEqualTNode->getChildren().front();
	TNode *rightCondExprConstantTNode = whileCondExprRightEqualTNode->getChildren().back();
    REQUIRE(whileCondExprLeftEqualTNode->getType() == TNodeType::Equal);
    REQUIRE(leftCondExprVariableTNode->getType() == TNodeType::Variable);
    REQUIRE(leftCondExprVariableTNode->getName() == "x");
    REQUIRE(leftCondExprConstantTNode->getType() == TNodeType::Constant);
    REQUIRE(leftCondExprConstantTNode->getName() == "0");
    REQUIRE(whileCondExprRightEqualTNode->getType() == TNodeType::Equal);
    REQUIRE(rightCondExprVariableTNode->getType() == TNodeType::Variable);
    REQUIRE(rightCondExprVariableTNode->getName() == "y");
    REQUIRE(rightCondExprConstantTNode->getType() == TNodeType::Constant);
    REQUIRE(rightCondExprConstantTNode->getName() == "0");
  }

  SECTION("WhileStatementList has one Read child") {
    TNode *readTNode = whileStmtLstTNode->getChildren().front();
    TNode *variableTNode = readTNode->getChildren().front();
    REQUIRE(readTNode->getType() == TNodeType::Read);
    REQUIRE(variableTNode->getType() == TNodeType::Variable);
    REQUIRE(variableTNode->getName() == "x");
  }
}

SCENARIO("Condition Expressions: And and Or") {
	Parser parser;
	std::queue<Token> tokenQueue;
	tokenQueue.push(Token(TokenType::Procedure, "procedure"));
	tokenQueue.push(Token(TokenType::Identifier, "Example"));
	tokenQueue.push(Token(TokenType::Separator, "{"));
	tokenQueue.push(Token(TokenType::While, "while"));
	tokenQueue.push(Token(TokenType::OpenParenthesis, "("));
	tokenQueue.push(Token(TokenType::OpenParenthesis, "("));
	tokenQueue.push(Token(TokenType::Identifier, "x"));
	tokenQueue.push(Token(TokenType::Equal, "=="));
	tokenQueue.push(Token(TokenType::Constant, "0"));
	tokenQueue.push(Token(TokenType::CloseParenthesis, ")"));
	tokenQueue.push(Token(TokenType::And, "&&"));
	tokenQueue.push(Token(TokenType::OpenParenthesis, "("));
	tokenQueue.push(Token(TokenType::Identifier, "y"));
	tokenQueue.push(Token(TokenType::Equal, "=="));
	tokenQueue.push(Token(TokenType::Constant, "0"));
	tokenQueue.push(Token(TokenType::CloseParenthesis, ")"));
	tokenQueue.push(Token(TokenType::Or, "||"));
	tokenQueue.push(Token(TokenType::OpenParenthesis, "("));
	tokenQueue.push(Token(TokenType::Identifier, "z"));
	tokenQueue.push(Token(TokenType::Equal, "=="));
	tokenQueue.push(Token(TokenType::Constant, "0"));
	tokenQueue.push(Token(TokenType::CloseParenthesis, ")"));
	tokenQueue.push(Token(TokenType::CloseParenthesis, ")"));
	tokenQueue.push(Token(TokenType::Separator, "{"));
	tokenQueue.push(Token(TokenType::Read, "read"));
	tokenQueue.push(Token(TokenType::Identifier, "x"));
	tokenQueue.push(Token(TokenType::Separator, ";"));
	tokenQueue.push(Token(TokenType::Separator, "}"));
	tokenQueue.push(Token(TokenType::Separator, "}"));
	TNode *programTNode = parser.buildAst(tokenQueue);
	TNode *procedureTNode = programTNode->getChildren().front();
	TNode *stmtLstTNode = procedureTNode->getChildren().front();

	SECTION("procedureTNode of AST has one StatementList child") {
		REQUIRE(stmtLstTNode->getType() == TNodeType::StatementList);
	}

	TNode *whileTNode = stmtLstTNode->getChildren().front();
	SECTION("StatementList has one While child") {
		REQUIRE(whileTNode->getType() == TNodeType::While);
	}

	TNode *whileCondExprOrTNode = whileTNode->getChildren().front();
	TNode *whileStmtLstTNode = whileTNode->getChildren().back();
	SECTION("While has ConditionExpression and StatementList children") {
		REQUIRE(whileCondExprOrTNode->getType() == TNodeType::Or);
		REQUIRE(whileStmtLstTNode->getType() == TNodeType::StatementList);
	}

	TNode *whileCondExprAndTNode = whileCondExprOrTNode->getChildren().front();
	TNode *whileCondExprEqualTNode = whileCondExprOrTNode->getChildren().back();
	SECTION("ConditionExpression has And and Equal children") {
		REQUIRE(whileCondExprAndTNode->getType() == TNodeType::And);
		REQUIRE(whileCondExprEqualTNode->getType() == TNodeType::Equal);
	}

	SECTION("Right subtree of ConditionExpression of Or") {
		TNode *condExprVariableTNode = whileCondExprEqualTNode->getChildren().front();
		TNode *condExprConstantTNode = whileCondExprEqualTNode->getChildren().back();
		REQUIRE(condExprVariableTNode->getType() == TNodeType::Variable);
		REQUIRE(condExprVariableTNode->getName() == "z");
		REQUIRE(condExprConstantTNode->getType() == TNodeType::Constant);
		REQUIRE(condExprConstantTNode->getName() == "0");
	}

	SECTION("Left subtree of ConditionExpression of Or") {
		TNode *leftCondExprTNode = whileCondExprAndTNode->getChildren().front();
		TNode *rightCondExprTNode = whileCondExprAndTNode->getChildren().back();
		TNode *leftCondExprVariableTNode = leftCondExprTNode->getChildren().front();
		TNode *leftCondExprConstantTNode = leftCondExprTNode->getChildren().back();
		TNode *rightCondExprVariableTNode = rightCondExprTNode->getChildren().front();
		TNode *rightCondExprConstantTNode = rightCondExprTNode->getChildren().back();
		REQUIRE(leftCondExprTNode->getType() == TNodeType::Equal);
		REQUIRE(leftCondExprVariableTNode->getType() == TNodeType::Variable);
		REQUIRE(leftCondExprVariableTNode->getName() == "x");
		REQUIRE(leftCondExprConstantTNode->getType() == TNodeType::Constant);
		REQUIRE(leftCondExprConstantTNode->getName() == "0");
		REQUIRE(rightCondExprTNode->getType() == TNodeType::Equal);
		REQUIRE(rightCondExprVariableTNode->getType() == TNodeType::Variable);
		REQUIRE(rightCondExprVariableTNode->getName() == "y");
		REQUIRE(rightCondExprConstantTNode->getType() == TNodeType::Constant);
		REQUIRE(rightCondExprConstantTNode->getName() == "0");
	}

	SECTION("WhileStatementList has one Read child") {
		TNode *readTNode = whileStmtLstTNode->getChildren().front();
		TNode *variableTNode = readTNode->getChildren().front();
		REQUIRE(readTNode->getType() == TNodeType::Read);
		REQUIRE(variableTNode->getType() == TNodeType::Variable);
		REQUIRE(variableTNode->getName() == "x");
	}
}

SCENARIO("Procedure with multiple statements") {
	Parser parser;
	std::queue<Token> tokenQueue;
	tokenQueue.push(Token(TokenType::Procedure, "procedure"));
	tokenQueue.push(Token(TokenType::Identifier, "Example"));
	tokenQueue.push(Token(TokenType::Separator, "{"));
	tokenQueue.push(Token(TokenType::Read, "read"));
	tokenQueue.push(Token(TokenType::Identifier, "x"));
	tokenQueue.push(Token(TokenType::Separator, ";"));
	tokenQueue.push(Token(TokenType::Print, "print"));
	tokenQueue.push(Token(TokenType::Identifier, "y"));
	tokenQueue.push(Token(TokenType::Separator, ";"));
	tokenQueue.push(Token(TokenType::While, "while"));
	tokenQueue.push(Token(TokenType::OpenParenthesis, "("));
	tokenQueue.push(Token(TokenType::OpenParenthesis, "("));
	tokenQueue.push(Token(TokenType::Identifier, "x"));
	tokenQueue.push(Token(TokenType::Equal, "=="));
	tokenQueue.push(Token(TokenType::Constant, "0"));
	tokenQueue.push(Token(TokenType::CloseParenthesis, ")"));
	tokenQueue.push(Token(TokenType::And, "&&"));
	tokenQueue.push(Token(TokenType::OpenParenthesis, "("));
	tokenQueue.push(Token(TokenType::Identifier, "y"));
	tokenQueue.push(Token(TokenType::Equal, "=="));
	tokenQueue.push(Token(TokenType::Constant, "0"));
	tokenQueue.push(Token(TokenType::CloseParenthesis, ")"));
	tokenQueue.push(Token(TokenType::Or, "||"));
	tokenQueue.push(Token(TokenType::OpenParenthesis, "("));
	tokenQueue.push(Token(TokenType::Identifier, "z"));
	tokenQueue.push(Token(TokenType::Equal, "=="));
	tokenQueue.push(Token(TokenType::Constant, "0"));
	tokenQueue.push(Token(TokenType::CloseParenthesis, ")"));
	tokenQueue.push(Token(TokenType::CloseParenthesis, ")"));
	tokenQueue.push(Token(TokenType::Separator, "{"));
	tokenQueue.push(Token(TokenType::Identifier, "x"));
	tokenQueue.push(Token(TokenType::Separator, "="));
	tokenQueue.push(Token(TokenType::Identifier, "x"));
	tokenQueue.push(Token(TokenType::Divide, "/"));
	tokenQueue.push(Token(TokenType::Identifier, "y"));
	tokenQueue.push(Token(TokenType::Separator, ";"));
	tokenQueue.push(Token(TokenType::Identifier, "y"));
	tokenQueue.push(Token(TokenType::Separator, "="));
	tokenQueue.push(Token(TokenType::Identifier, "x"));
	tokenQueue.push(Token(TokenType::Separator, ";"));
	tokenQueue.push(Token(TokenType::Separator, "}"));
	tokenQueue.push(Token(TokenType::Read, "read"));
	tokenQueue.push(Token(TokenType::Identifier, "z"));
	tokenQueue.push(Token(TokenType::Separator, ";"));
	tokenQueue.push(Token(TokenType::Separator, "}"));
	TNode *programTNode = parser.buildAst(tokenQueue);
	TNode *procedureTNode = programTNode->getChildren().front();
	TNode *stmtLstTNode = procedureTNode->getChildren().front();

	SECTION("procedureTNode of AST has one StatementList child") {
		REQUIRE(stmtLstTNode->getType() == TNodeType::StatementList);
	}

	TNode *readXTNode = stmtLstTNode->getChildren()[0];
	TNode *printTNode = stmtLstTNode->getChildren()[1];
	TNode *whileTNode = stmtLstTNode->getChildren()[2];
	TNode *readZTNode = stmtLstTNode->getChildren()[3];
	SECTION("StatementList has two Read, one While and one Print children") {
		REQUIRE(readXTNode->getType() == TNodeType::Read);
		REQUIRE(printTNode->getType() == TNodeType::Print);
		REQUIRE(whileTNode->getType() == TNodeType::While);
		REQUIRE(readZTNode->getType() == TNodeType::Read);
	}

	SECTION("Read has one Variable child") {
		TNode *variableXTNode = readXTNode->getChildren().front();
		TNode *variableZTNode = readZTNode->getChildren().front();
		REQUIRE(variableXTNode->getType() == TNodeType::Variable);
		REQUIRE(variableXTNode->getName() == "x");
		REQUIRE(variableZTNode->getType() == TNodeType::Variable);
		REQUIRE(variableZTNode->getName() == "z");
	}

	SECTION("Print has one Variable child") {
		TNode *variableTNode = printTNode->getChildren().front();
		REQUIRE(variableTNode->getType() == TNodeType::Variable);
		REQUIRE(variableTNode->getName() == "y");
	}

	TNode *whileCondExprOrTNode = whileTNode->getChildren().front();
	TNode *whileStmtLstTNode = whileTNode->getChildren().back();
	SECTION("While has ConditionExpression and StatementList children") {
		REQUIRE(whileCondExprOrTNode->getType() == TNodeType::Or);
		REQUIRE(whileStmtLstTNode->getType() == TNodeType::StatementList);
	}

	TNode *whileCondExprAndTNode = whileCondExprOrTNode->getChildren().front();
	TNode *whileCondExprEqualTNode = whileCondExprOrTNode->getChildren().back();
	SECTION("ConditionExpression has And and Equal children") {
		REQUIRE(whileCondExprAndTNode->getType() == TNodeType::And);
		REQUIRE(whileCondExprEqualTNode->getType() == TNodeType::Equal);
	}

	SECTION("Right subtree of ConditionExpression of Or") {
		TNode *condExprVariableTNode = whileCondExprEqualTNode->getChildren().front();
		TNode *condExprConstantTNode = whileCondExprEqualTNode->getChildren().back();
		REQUIRE(condExprVariableTNode->getType() == TNodeType::Variable);
		REQUIRE(condExprVariableTNode->getName() == "z");
		REQUIRE(condExprConstantTNode->getType() == TNodeType::Constant);
		REQUIRE(condExprConstantTNode->getName() == "0");
	}

	SECTION("Left subtree of ConditionExpression of Or") {
		TNode *leftCondExprTNode = whileCondExprAndTNode->getChildren().front();
		TNode *rightCondExprTNode = whileCondExprAndTNode->getChildren().back();
		TNode *leftCondExprVariableTNode = leftCondExprTNode->getChildren().front();
		TNode *leftCondExprConstantTNode = leftCondExprTNode->getChildren().back();
		TNode *rightCondExprVariableTNode = rightCondExprTNode->getChildren().front();
		TNode *rightCondExprConstantTNode = rightCondExprTNode->getChildren().back();
		REQUIRE(leftCondExprTNode->getType() == TNodeType::Equal);
		REQUIRE(leftCondExprVariableTNode->getType() == TNodeType::Variable);
		REQUIRE(leftCondExprVariableTNode->getName() == "x");
		REQUIRE(leftCondExprConstantTNode->getType() == TNodeType::Constant);
		REQUIRE(leftCondExprConstantTNode->getName() == "0");
		REQUIRE(rightCondExprTNode->getType() == TNodeType::Equal);
		REQUIRE(rightCondExprVariableTNode->getType() == TNodeType::Variable);
		REQUIRE(rightCondExprVariableTNode->getName() == "y");
		REQUIRE(rightCondExprConstantTNode->getType() == TNodeType::Constant);
		REQUIRE(rightCondExprConstantTNode->getName() == "0");
	}

	SECTION("WhileStatementList has two Assign children") {
		TNode *firstAssignTNode = whileStmtLstTNode->getChildren().front();
		TNode *firstVariableTNode = firstAssignTNode->getChildren().front();
		TNode *divideTNode = firstAssignTNode->getChildren().back();
		TNode *divideLeftTNode = divideTNode->getChildren().front();
		TNode *divideRightTNode = divideTNode->getChildren().back();
		TNode *secondAssignTNode = whileStmtLstTNode->getChildren().back();
		TNode *secondVariableTNode = secondAssignTNode->getChildren().front();
		TNode *variableTNode = secondAssignTNode->getChildren().back();
		REQUIRE(firstAssignTNode->getType() == TNodeType::Assign);
		REQUIRE(secondAssignTNode->getType() == TNodeType::Assign);
		REQUIRE(firstVariableTNode->getType() == TNodeType::Variable);
		REQUIRE(firstVariableTNode->getName() == "x");
		REQUIRE(divideTNode->getType() == TNodeType::Divide);
		REQUIRE(divideLeftTNode->getType() == TNodeType::Variable);
		REQUIRE(divideLeftTNode->getName() == "x");
		REQUIRE(divideRightTNode->getType() == TNodeType::Variable);
		REQUIRE(divideRightTNode->getName() == "y");
		REQUIRE(secondVariableTNode->getType() == TNodeType::Variable);
		REQUIRE(secondVariableTNode->getName() == "y");
		REQUIRE(variableTNode->getType() == TNodeType::Variable);
		REQUIRE(variableTNode->getName() == "x");
	}
}