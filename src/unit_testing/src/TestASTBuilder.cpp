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
  const std::unique_ptr<TNode> &programTNode = parser.buildAst(tokenQueue);
  const std::unique_ptr<TNode> &procedureTNode = programTNode->children.front();
  const std::unique_ptr<TNode> &stmtLstTNode = procedureTNode->children.front();

  SECTION("procedureTNode of AST has one StatementList child") {
    REQUIRE(stmtLstTNode->type == TNodeType::StatementList);
  }

  const std::unique_ptr<TNode> &assignTNode = stmtLstTNode->children.front();
  SECTION("StatementList has one Assign child") {
    REQUIRE(assignTNode->type == TNodeType::Assign);
  }

  SECTION("Assign has Variable and Constant children") {
    const std::unique_ptr<TNode> &variableTNode = assignTNode->children.front();
    const std::unique_ptr<TNode> &constantTNode = assignTNode->children.back();
    REQUIRE(variableTNode->type == TNodeType::Variable);
    REQUIRE(variableTNode->name == "x");
    REQUIRE(constantTNode->type == TNodeType::Constant);
    REQUIRE(constantTNode->name == "0");
  }

  SECTION("Assign is assigned statement number 1") {
    REQUIRE(assignTNode->statementNumber == 1);
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
  const std::unique_ptr<TNode> &programTNode = parser.buildAst(tokenQueue);
  const std::unique_ptr<TNode> &procedureTNode = programTNode->children.front();
  const std::unique_ptr<TNode> &stmtLstTNode = procedureTNode->children.front();

  SECTION("procedureTNode of AST has one StatementList child") {
    REQUIRE(stmtLstTNode->type == TNodeType::StatementList);
  }

  const std::unique_ptr<TNode> &readTNode = stmtLstTNode->children.front();
  SECTION("StatementList has one Read child") {
    REQUIRE(readTNode->type == TNodeType::Read);
  }

  SECTION("Read has one Variable child") {
    const std::unique_ptr<TNode> &variableTNode = readTNode->children.front();
    REQUIRE(variableTNode->type == TNodeType::Variable);
    REQUIRE(variableTNode->name == "x");
  }

  SECTION("Read is assigned statement number 1") {
    REQUIRE(readTNode->statementNumber == 1);
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
  const std::unique_ptr<TNode> &programTNode = parser.buildAst(tokenQueue);
  const std::unique_ptr<TNode> &procedureTNode = programTNode->children.front();
  const std::unique_ptr<TNode> &stmtLstTNode = procedureTNode->children.front();

  SECTION("procedureTNode of AST has one StatementList child") {
    REQUIRE(stmtLstTNode->type == TNodeType::StatementList);
  }

  const std::unique_ptr<TNode> &printTNode = stmtLstTNode->children.front();
  SECTION("StatementList has one Print child") {
    REQUIRE(printTNode->type == TNodeType::Print);
  }

  SECTION("Print has one Variable child") {
    const std::unique_ptr<TNode> &variableTNode = printTNode->children.front();
    REQUIRE(variableTNode->type == TNodeType::Variable);
    REQUIRE(variableTNode->name == "x");
  }

  SECTION("Print is assigned statement number 1") {
    REQUIRE(printTNode->statementNumber == 1);
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
  const std::unique_ptr<TNode> &programTNode = parser.buildAst(tokenQueue);
  const std::unique_ptr<TNode> &procedureTNode = programTNode->children.front();
  const std::unique_ptr<TNode> &stmtLstTNode = procedureTNode->children.front();

  SECTION("procedureTNode of AST has one StatementList child") {
    REQUIRE(stmtLstTNode->type == TNodeType::StatementList);
  }

  const std::unique_ptr<TNode> &whileTNode = stmtLstTNode->children.front();
  SECTION("StatementList has one While child") {
    REQUIRE(whileTNode->type == TNodeType::While);
  }

  const std::unique_ptr<TNode> &whileCondExprTNode =
      whileTNode->children.front();
  const std::unique_ptr<TNode> &whileStmtLstTNode = whileTNode->children.back();
  SECTION("While has ConditionExpression and StatementList children") {
    REQUIRE(whileCondExprTNode->type == TNodeType::Equal);
    REQUIRE(whileStmtLstTNode->type == TNodeType::StatementList);
  }

  SECTION("ConditionExpression has Variable and Constant children") {
    const std::unique_ptr<TNode> &variableTNode =
        whileCondExprTNode->children.front();
    const std::unique_ptr<TNode> &constantTNode =
        whileCondExprTNode->children.back();
    REQUIRE(variableTNode->type == TNodeType::Variable);
    REQUIRE(variableTNode->name == "x");
    REQUIRE(constantTNode->type == TNodeType::Constant);
    REQUIRE(constantTNode->name == "0");
  }

  const std::unique_ptr<TNode> &readTNode = whileStmtLstTNode->children.front();
  SECTION("WhileStatementList has one Read child") {
    const std::unique_ptr<TNode> &variableTNode = readTNode->children.front();
    REQUIRE(readTNode->type == TNodeType::Read);
    REQUIRE(variableTNode->type == TNodeType::Variable);
    REQUIRE(variableTNode->name == "x");
  }

  SECTION("Statement numbers are assigned accordingly") {
    REQUIRE(whileTNode->statementNumber == 1);
    REQUIRE(readTNode->statementNumber == 2);
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
  const std::unique_ptr<TNode> &programTNode = parser.buildAst(tokenQueue);
  const std::unique_ptr<TNode> &procedureTNode = programTNode->children.front();
  const std::unique_ptr<TNode> &stmtLstTNode = procedureTNode->children.front();

  SECTION("procedureTNode of AST has one StatementList child") {
    REQUIRE(stmtLstTNode->type == TNodeType::StatementList);
  }

  const std::unique_ptr<TNode> &ifTNode = stmtLstTNode->children.front();
  SECTION("StatementList has one If child") {
    REQUIRE(ifTNode->type == TNodeType::If);
  }

  const std::unique_ptr<TNode> &ifCondExprTNode = ifTNode->children[0];
  const std::unique_ptr<TNode> &thenStmtLstTNode = ifTNode->children[1];
  const std::unique_ptr<TNode> &elseStmtLstTNode = ifTNode->children[2];
  SECTION("If has ConditionExpression, ThenStatementList, ElseStatementList "
          "children") {
    REQUIRE(ifCondExprTNode->type == TNodeType::Equal);
    REQUIRE(thenStmtLstTNode->type == TNodeType::StatementList);
    REQUIRE(elseStmtLstTNode->type == TNodeType::StatementList);
  }

  SECTION("ConditionExpression has Variable and Constant children") {
    const std::unique_ptr<TNode> &variableTNode =
        ifCondExprTNode->children.front();
    const std::unique_ptr<TNode> &constantTNode =
        ifCondExprTNode->children.back();
    REQUIRE(variableTNode->type == TNodeType::Variable);
    REQUIRE(variableTNode->name == "x");
    REQUIRE(constantTNode->type == TNodeType::Constant);
    REQUIRE(constantTNode->name == "0");
  }

  const std::unique_ptr<TNode> &readXTNode = thenStmtLstTNode->children.front();
  SECTION("ThenStatementList has one Read child") {
    const std::unique_ptr<TNode> &variableTNode = readXTNode->children.front();
    REQUIRE(readXTNode->type == TNodeType::Read);
    REQUIRE(variableTNode->type == TNodeType::Variable);
    REQUIRE(variableTNode->name == "x");
  }

  const std::unique_ptr<TNode> &readYTNode = elseStmtLstTNode->children.front();
  SECTION("ElseStatementList has one Read child") {
    const std::unique_ptr<TNode> &variableTNode = readYTNode->children.front();
    REQUIRE(readYTNode->type == TNodeType::Read);
    REQUIRE(variableTNode->type == TNodeType::Variable);
    REQUIRE(variableTNode->name == "y");
  }

  SECTION("Statement numbers are assigned accordingly") {
    REQUIRE(ifTNode->statementNumber == 1);
    REQUIRE(readXTNode->statementNumber == 2);
    REQUIRE(readYTNode->statementNumber == 3);
  }
}

SCENARIO("Procedure with one if statement nested in one while statement") {
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
  tokenQueue.push(Token(TokenType::If, "if"));
  tokenQueue.push(Token(TokenType::OpenParenthesis, "("));
  tokenQueue.push(Token(TokenType::Identifier, "y"));
  tokenQueue.push(Token(TokenType::Equal, "=="));
  tokenQueue.push(Token(TokenType::Constant, "0"));
  tokenQueue.push(Token(TokenType::CloseParenthesis, ")"));
  tokenQueue.push(Token(TokenType::Then, "then"));
  tokenQueue.push(Token(TokenType::Separator, "{"));
  tokenQueue.push(Token(TokenType::Read, "read"));
  tokenQueue.push(Token(TokenType::Identifier, "z"));
  tokenQueue.push(Token(TokenType::Separator, ";"));
  tokenQueue.push(Token(TokenType::Separator, "}"));
  tokenQueue.push(Token(TokenType::Else, "else"));
  tokenQueue.push(Token(TokenType::Separator, "{"));
  tokenQueue.push(Token(TokenType::Print, "print"));
  tokenQueue.push(Token(TokenType::Identifier, "a"));
  tokenQueue.push(Token(TokenType::Separator, ";"));
  tokenQueue.push(Token(TokenType::Separator, "}"));
  tokenQueue.push(Token(TokenType::Separator, "}"));
  tokenQueue.push(Token(TokenType::Separator, "}"));
  const std::unique_ptr<TNode> &programTNode = parser.buildAst(tokenQueue);
  const std::unique_ptr<TNode> &procedureTNode = programTNode->children.front();
  const std::unique_ptr<TNode> &stmtLstTNode = procedureTNode->children.front();

  SECTION("procedureTNode of AST has one StatementList child") {
    REQUIRE(stmtLstTNode->type == TNodeType::StatementList);
  }

  const std::unique_ptr<TNode> &whileTNode = stmtLstTNode->children.front();
  SECTION("StatementList has one While child") {
    REQUIRE(whileTNode->type == TNodeType::While);
  }

  const std::unique_ptr<TNode> &whileCondExprTNode =
      whileTNode->children.front();
  const std::unique_ptr<TNode> &whileStmtLstTNode = whileTNode->children.back();
  SECTION("While has ConditionExpression and StatementList children") {
    REQUIRE(whileCondExprTNode->type == TNodeType::Equal);
    REQUIRE(whileStmtLstTNode->type == TNodeType::StatementList);
  }

  SECTION("WhileConditionExpression has Variable and Constant children") {
    const std::unique_ptr<TNode> &variableTNode =
        whileCondExprTNode->children.front();
    const std::unique_ptr<TNode> &constantTNode =
        whileCondExprTNode->children.back();
    REQUIRE(variableTNode->type == TNodeType::Variable);
    REQUIRE(variableTNode->name == "x");
    REQUIRE(constantTNode->type == TNodeType::Constant);
    REQUIRE(constantTNode->name == "0");
  }

  const std::unique_ptr<TNode> &ifTNode = whileStmtLstTNode->children.front();
  SECTION("WhileStatementList has one If child") {
    REQUIRE(ifTNode->type == TNodeType::If);
  }

  const std::unique_ptr<TNode> &ifCondExprTNode = ifTNode->children[0];
  const std::unique_ptr<TNode> &thenStmtLstTNode = ifTNode->children[1];
  const std::unique_ptr<TNode> &elseStmtLstTNode = ifTNode->children[2];
  SECTION("If has ConditionExpression, ThenStatementList, ElseStatementList "
          "children") {
    REQUIRE(ifCondExprTNode->type == TNodeType::Equal);
    REQUIRE(thenStmtLstTNode->type == TNodeType::StatementList);
    REQUIRE(elseStmtLstTNode->type == TNodeType::StatementList);
  }

  SECTION("IfConditionExpression has Variable and Constant children") {
    const std::unique_ptr<TNode> &variableTNode =
        ifCondExprTNode->children.front();
    const std::unique_ptr<TNode> &constantTNode =
        ifCondExprTNode->children.back();
    REQUIRE(variableTNode->type == TNodeType::Variable);
    REQUIRE(variableTNode->name == "y");
    REQUIRE(constantTNode->type == TNodeType::Constant);
    REQUIRE(constantTNode->name == "0");
  }

  const std::unique_ptr<TNode> &readZTNode = thenStmtLstTNode->children.front();
  SECTION("ThenStatementList has one Read child") {
    const std::unique_ptr<TNode> &variableTNode = readZTNode->children.front();
    REQUIRE(readZTNode->type == TNodeType::Read);
    REQUIRE(variableTNode->type == TNodeType::Variable);
    REQUIRE(variableTNode->name == "z");
  }

  const std::unique_ptr<TNode> &printATNode =
      elseStmtLstTNode->children.front();
  SECTION("ElseStatementList has one Print child") {
    const std::unique_ptr<TNode> &variableTNode = printATNode->children.front();
    REQUIRE(printATNode->type == TNodeType::Print);
    REQUIRE(variableTNode->type == TNodeType::Variable);
    REQUIRE(variableTNode->name == "a");
  }

  SECTION("Statement numbers are assigned accordingly") {
    REQUIRE(whileTNode->statementNumber == 1);
    REQUIRE(ifTNode->statementNumber == 2);
    REQUIRE(readZTNode->statementNumber == 3);
    REQUIRE(printATNode->statementNumber == 4);
  }
}

SCENARIO("Procedure with one while statement nested in one if statement") {
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
  tokenQueue.push(Token(TokenType::While, "while"));
  tokenQueue.push(Token(TokenType::OpenParenthesis, "("));
  tokenQueue.push(Token(TokenType::Identifier, "y"));
  tokenQueue.push(Token(TokenType::Equal, "=="));
  tokenQueue.push(Token(TokenType::Constant, "0"));
  tokenQueue.push(Token(TokenType::CloseParenthesis, ")"));
  tokenQueue.push(Token(TokenType::Separator, "{"));
  tokenQueue.push(Token(TokenType::Read, "read"));
  tokenQueue.push(Token(TokenType::Identifier, "z"));
  tokenQueue.push(Token(TokenType::Separator, ";"));
  tokenQueue.push(Token(TokenType::Separator, "}"));
  tokenQueue.push(Token(TokenType::Separator, "}"));
  tokenQueue.push(Token(TokenType::Else, "else"));
  tokenQueue.push(Token(TokenType::Separator, "{"));
  tokenQueue.push(Token(TokenType::Print, "print"));
  tokenQueue.push(Token(TokenType::Identifier, "a"));
  tokenQueue.push(Token(TokenType::Separator, ";"));
  tokenQueue.push(Token(TokenType::Separator, "}"));
  tokenQueue.push(Token(TokenType::Separator, "}"));
  const std::unique_ptr<TNode> &programTNode = parser.buildAst(tokenQueue);
  const std::unique_ptr<TNode> &procedureTNode = programTNode->children.front();
  const std::unique_ptr<TNode> &stmtLstTNode = procedureTNode->children.front();

  SECTION("procedureTNode of AST has one StatementList child") {
    REQUIRE(stmtLstTNode->type == TNodeType::StatementList);
  }

  const std::unique_ptr<TNode> &ifTNode = stmtLstTNode->children.front();
  SECTION("StatementList has one If child") {
    REQUIRE(ifTNode->type == TNodeType::If);
  }

  const std::unique_ptr<TNode> &ifCondExprTNode = ifTNode->children[0];
  const std::unique_ptr<TNode> &thenStmtLstTNode = ifTNode->children[1];
  const std::unique_ptr<TNode> &elseStmtLstTNode = ifTNode->children[2];
  SECTION("If has ConditionExpression, ThenStatementList, ElseStatementList "
          "children") {
    REQUIRE(ifCondExprTNode->type == TNodeType::Equal);
    REQUIRE(thenStmtLstTNode->type == TNodeType::StatementList);
    REQUIRE(elseStmtLstTNode->type == TNodeType::StatementList);
  }

  SECTION("IfConditionExpression has Variable and Constant children") {
    const std::unique_ptr<TNode> &variableTNode =
        ifCondExprTNode->children.front();
    const std::unique_ptr<TNode> &constantTNode =
        ifCondExprTNode->children.back();
    REQUIRE(variableTNode->type == TNodeType::Variable);
    REQUIRE(variableTNode->name == "x");
    REQUIRE(constantTNode->type == TNodeType::Constant);
    REQUIRE(constantTNode->name == "0");
  }

  const std::unique_ptr<TNode> &whileTNode = thenStmtLstTNode->children.front();
  SECTION("ThenStatementList has one While child") {
    REQUIRE(whileTNode->type == TNodeType::While);
  }

  const std::unique_ptr<TNode> &whileCondExprTNode =
      whileTNode->children.front();
  const std::unique_ptr<TNode> &whileStmtLstTNode = whileTNode->children.back();
  SECTION("While has ConditionExpression and StatementList children") {
    REQUIRE(whileCondExprTNode->type == TNodeType::Equal);
    REQUIRE(whileStmtLstTNode->type == TNodeType::StatementList);
  }

  SECTION("WhileConditionExpression has Variable and Constant children") {
    const std::unique_ptr<TNode> &variableTNode =
        whileCondExprTNode->children.front();
    const std::unique_ptr<TNode> &constantTNode =
        whileCondExprTNode->children.back();
    REQUIRE(variableTNode->type == TNodeType::Variable);
    REQUIRE(variableTNode->name == "y");
    REQUIRE(constantTNode->type == TNodeType::Constant);
    REQUIRE(constantTNode->name == "0");
  }

  const std::unique_ptr<TNode> &readTNode = whileStmtLstTNode->children.front();
  SECTION("WhileStatementList has one Read child") {
    const std::unique_ptr<TNode> &variableTNode = readTNode->children.front();
    REQUIRE(readTNode->type == TNodeType::Read);
    REQUIRE(variableTNode->type == TNodeType::Variable);
    REQUIRE(variableTNode->name == "z");
  }

  const std::unique_ptr<TNode> &printTNode = elseStmtLstTNode->children.front();
  SECTION("ElseStatementList has one Read child") {
    const std::unique_ptr<TNode> &variableTNode = printTNode->children.front();
    REQUIRE(printTNode->type == TNodeType::Print);
    REQUIRE(variableTNode->type == TNodeType::Variable);
    REQUIRE(variableTNode->name == "a");
  }

  SECTION("Statement numbers are assigned accordingly") {
    REQUIRE(ifTNode->statementNumber == 1);
    REQUIRE(whileTNode->statementNumber == 2);
    REQUIRE(readTNode->statementNumber == 3);
    REQUIRE(printTNode->statementNumber == 4);
  }
}

SCENARIO(
    "Procedure with one while statement nested in another while statement") {
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
  tokenQueue.push(Token(TokenType::While, "while"));
  tokenQueue.push(Token(TokenType::OpenParenthesis, "("));
  tokenQueue.push(Token(TokenType::Identifier, "y"));
  tokenQueue.push(Token(TokenType::Equal, "=="));
  tokenQueue.push(Token(TokenType::Constant, "0"));
  tokenQueue.push(Token(TokenType::CloseParenthesis, ")"));
  tokenQueue.push(Token(TokenType::Separator, "{"));
  tokenQueue.push(Token(TokenType::Read, "read"));
  tokenQueue.push(Token(TokenType::Identifier, "z"));
  tokenQueue.push(Token(TokenType::Separator, ";"));
  tokenQueue.push(Token(TokenType::Separator, "}"));
  tokenQueue.push(Token(TokenType::Print, "print"));
  tokenQueue.push(Token(TokenType::Identifier, "a"));
  tokenQueue.push(Token(TokenType::Separator, ";"));
  tokenQueue.push(Token(TokenType::Separator, "}"));
  tokenQueue.push(Token(TokenType::Separator, "}"));
  const std::unique_ptr<TNode> &programTNode = parser.buildAst(tokenQueue);
  const std::unique_ptr<TNode> &procedureTNode = programTNode->children.front();
  const std::unique_ptr<TNode> &stmtLstTNode = procedureTNode->children.front();

  SECTION("procedureTNode of AST has one StatementList child") {
    REQUIRE(stmtLstTNode->type == TNodeType::StatementList);
  }

  const std::unique_ptr<TNode> &whileTNode = stmtLstTNode->children.front();
  SECTION("StatementList has one While child") {
    REQUIRE(whileTNode->type == TNodeType::While);
  }

  const std::unique_ptr<TNode> &whileCondExprTNode =
      whileTNode->children.front();
  const std::unique_ptr<TNode> &whileStmtLstTNode = whileTNode->children.back();
  SECTION("OuterWhile has ConditionExpression and StatementList children") {
    REQUIRE(whileCondExprTNode->type == TNodeType::Equal);
    REQUIRE(whileStmtLstTNode->type == TNodeType::StatementList);
  }

  SECTION("WhileConditionExpression has Variable and Constant children") {
    const std::unique_ptr<TNode> &variableTNode =
        whileCondExprTNode->children.front();
    const std::unique_ptr<TNode> &constantTNode =
        whileCondExprTNode->children.back();
    REQUIRE(variableTNode->type == TNodeType::Variable);
    REQUIRE(variableTNode->name == "x");
    REQUIRE(constantTNode->type == TNodeType::Constant);
    REQUIRE(constantTNode->name == "0");
  }

  const std::unique_ptr<TNode> &innerWhileTNode =
      whileStmtLstTNode->children.front();
  SECTION("WhileStatementList has one While child") {
    REQUIRE(innerWhileTNode->type == TNodeType::While);
  }

  const std::unique_ptr<TNode> &innerWhileCondExprTNode =
      innerWhileTNode->children.front();
  const std::unique_ptr<TNode> &innerWhileStmtLstTNode =
      innerWhileTNode->children.back();
  SECTION("InnerWhileConditionExpression has Variable and Constant children") {
    const std::unique_ptr<TNode> &variableTNode =
        innerWhileCondExprTNode->children.front();
    const std::unique_ptr<TNode> &constantTNode =
        innerWhileCondExprTNode->children.back();
    REQUIRE(variableTNode->type == TNodeType::Variable);
    REQUIRE(variableTNode->name == "y");
    REQUIRE(constantTNode->type == TNodeType::Constant);
    REQUIRE(constantTNode->name == "0");
  }

  const std::unique_ptr<TNode> &readTNode =
      innerWhileStmtLstTNode->children.front();
  SECTION("InnerWhileStatementList has one Read child") {
    const std::unique_ptr<TNode> &variableTNode = readTNode->children.front();
    REQUIRE(readTNode->type == TNodeType::Read);
    REQUIRE(variableTNode->type == TNodeType::Variable);
    REQUIRE(variableTNode->name == "z");
  }

  const std::unique_ptr<TNode> &printTNode = whileStmtLstTNode->children.back();
  SECTION("WhileStatementList has one Print child") {
    const std::unique_ptr<TNode> &variableTNode = printTNode->children.front();
    REQUIRE(printTNode->type == TNodeType::Print);
    REQUIRE(variableTNode->type == TNodeType::Variable);
    REQUIRE(variableTNode->name == "a");
  }

  SECTION("Statement numbers are assigned accordingly") {
    REQUIRE(whileTNode->statementNumber == 1);
    REQUIRE(innerWhileTNode->statementNumber == 2);
    REQUIRE(readTNode->statementNumber == 3);
    REQUIRE(printTNode->statementNumber == 4);
  }
}

SCENARIO("Procedure with one if statement nested in another if statement") {
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
  tokenQueue.push(Token(TokenType::If, "if"));
  tokenQueue.push(Token(TokenType::OpenParenthesis, "("));
  tokenQueue.push(Token(TokenType::Identifier, "y"));
  tokenQueue.push(Token(TokenType::Equal, "=="));
  tokenQueue.push(Token(TokenType::Constant, "0"));
  tokenQueue.push(Token(TokenType::CloseParenthesis, ")"));
  tokenQueue.push(Token(TokenType::Then, "then"));
  tokenQueue.push(Token(TokenType::Separator, "{"));
  tokenQueue.push(Token(TokenType::Read, "read"));
  tokenQueue.push(Token(TokenType::Identifier, "z"));
  tokenQueue.push(Token(TokenType::Separator, ";"));
  tokenQueue.push(Token(TokenType::Separator, "}"));
  tokenQueue.push(Token(TokenType::Else, "else"));
  tokenQueue.push(Token(TokenType::Separator, "{"));
  tokenQueue.push(Token(TokenType::Print, "print"));
  tokenQueue.push(Token(TokenType::Identifier, "a"));
  tokenQueue.push(Token(TokenType::Separator, ";"));
  tokenQueue.push(Token(TokenType::Separator, "}"));
  tokenQueue.push(Token(TokenType::Separator, "}"));
  tokenQueue.push(Token(TokenType::Else, "else"));
  tokenQueue.push(Token(TokenType::Separator, "{"));
  tokenQueue.push(Token(TokenType::Print, "print"));
  tokenQueue.push(Token(TokenType::Identifier, "b"));
  tokenQueue.push(Token(TokenType::Separator, ";"));
  tokenQueue.push(Token(TokenType::Separator, "}"));
  tokenQueue.push(Token(TokenType::Separator, "}"));
  const std::unique_ptr<TNode> &programTNode = parser.buildAst(tokenQueue);
  const std::unique_ptr<TNode> &procedureTNode = programTNode->children.front();
  const std::unique_ptr<TNode> &stmtLstTNode = procedureTNode->children.front();

  SECTION("procedureTNode of AST has one StatementList child") {
    REQUIRE(stmtLstTNode->type == TNodeType::StatementList);
  }

  const std::unique_ptr<TNode> &ifTNode = stmtLstTNode->children.front();
  SECTION("StatementList has one If child") {
    REQUIRE(ifTNode->type == TNodeType::If);
  }

  const std::unique_ptr<TNode> &ifCondExprTNode = ifTNode->children[0];
  const std::unique_ptr<TNode> &thenStmtLstTNode = ifTNode->children[1];
  const std::unique_ptr<TNode> &elseStmtLstTNode = ifTNode->children[2];
  SECTION("If has ConditionExpression, ThenStatementList, ElseStatementList "
          "children") {
    REQUIRE(ifCondExprTNode->type == TNodeType::Equal);
    REQUIRE(thenStmtLstTNode->type == TNodeType::StatementList);
    REQUIRE(elseStmtLstTNode->type == TNodeType::StatementList);
  }

  SECTION("IfConditionExpression has Variable and Constant children") {
    const std::unique_ptr<TNode> &variableTNode =
        ifCondExprTNode->children.front();
    const std::unique_ptr<TNode> &constantTNode =
        ifCondExprTNode->children.back();
    REQUIRE(variableTNode->type == TNodeType::Variable);
    REQUIRE(variableTNode->name == "x");
    REQUIRE(constantTNode->type == TNodeType::Constant);
    REQUIRE(constantTNode->name == "0");
  }

  const std::unique_ptr<TNode> &innerIfTNode =
      thenStmtLstTNode->children.front();
  SECTION("ThenStatementList has one If child") {
    REQUIRE(innerIfTNode->type == TNodeType::If);
  }

  const std::unique_ptr<TNode> &innerIfCondExprTNode =
      innerIfTNode->children[0];
  const std::unique_ptr<TNode> &innerThenStmtLstTNode =
      innerIfTNode->children[1];
  const std::unique_ptr<TNode> &innerElseStmtLstTNode =
      innerIfTNode->children[2];
  SECTION(
      "InnerIf has ConditionExpression, ThenStatementList, ElseStatementList "
      "children") {
    REQUIRE(innerIfCondExprTNode->type == TNodeType::Equal);
    REQUIRE(innerThenStmtLstTNode->type == TNodeType::StatementList);
    REQUIRE(innerElseStmtLstTNode->type == TNodeType::StatementList);
  }

  SECTION("InnerIfConditionExpression has Variable and Constant children") {
    const std::unique_ptr<TNode> &variableTNode =
        innerIfCondExprTNode->children.front();
    const std::unique_ptr<TNode> &constantTNode =
        innerIfCondExprTNode->children.back();
    REQUIRE(variableTNode->type == TNodeType::Variable);
    REQUIRE(variableTNode->name == "y");
    REQUIRE(constantTNode->type == TNodeType::Constant);
    REQUIRE(constantTNode->name == "0");
  }

  const std::unique_ptr<TNode> &readTNode =
      innerThenStmtLstTNode->children.front();
  SECTION("InnerThenStatementList has one Read child") {
    const std::unique_ptr<TNode> &variableTNode = readTNode->children.front();
    REQUIRE(readTNode->type == TNodeType::Read);
    REQUIRE(variableTNode->type == TNodeType::Variable);
    REQUIRE(variableTNode->name == "z");
  }

  const std::unique_ptr<TNode> &printATNode =
      innerElseStmtLstTNode->children.front();
  SECTION("InnerElseStatementList has one Print child") {
    const std::unique_ptr<TNode> &variableTNode = printATNode->children.front();
    REQUIRE(printATNode->type == TNodeType::Print);
    REQUIRE(variableTNode->type == TNodeType::Variable);
    REQUIRE(variableTNode->name == "a");
  }

  const std::unique_ptr<TNode> &printBTNode =
      elseStmtLstTNode->children.front();
  SECTION("ElseStatementList has one Print child") {
    const std::unique_ptr<TNode> &variableTNode = printBTNode->children.front();
    REQUIRE(printBTNode->type == TNodeType::Print);
    REQUIRE(variableTNode->type == TNodeType::Variable);
    REQUIRE(variableTNode->name == "b");
  }

  SECTION("Statement numbers are assigned accordingly") {
    REQUIRE(ifTNode->statementNumber == 1);
    REQUIRE(innerIfTNode->statementNumber == 2);
    REQUIRE(readTNode->statementNumber == 3);
    REQUIRE(printATNode->statementNumber == 4);
    REQUIRE(printBTNode->statementNumber == 5);
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
  const std::unique_ptr<TNode> &programTNode = parser.buildAst(tokenQueue);
  const std::unique_ptr<TNode> &procedureTNode = programTNode->children.front();
  const std::unique_ptr<TNode> &stmtLstTNode = procedureTNode->children.front();

  SECTION("procedureTNode of AST has one StatementList child") {
    REQUIRE(stmtLstTNode->type == TNodeType::StatementList);
  }

  const std::unique_ptr<TNode> &assignTNode = stmtLstTNode->children.front();
  SECTION("StatementList has one Assign child") {
    REQUIRE(assignTNode->type == TNodeType::Assign);
  }

  const std::unique_ptr<TNode> &assignVariableTNode =
      assignTNode->children.front();
  const std::unique_ptr<TNode> &assignPlusTNode = assignTNode->children.back();
  SECTION("Assign has Variable and Plus children") {
    REQUIRE(assignVariableTNode->type == TNodeType::Variable);
    REQUIRE(assignVariableTNode->name == "x");
    REQUIRE(assignPlusTNode->type == TNodeType::Plus);
  }

  const std::unique_ptr<TNode> &plusTNode = assignPlusTNode->children.front();
  const std::unique_ptr<TNode> &variableATNode = plusTNode->children.front();
  const std::unique_ptr<TNode> &variableBTNode = plusTNode->children.back();
  SECTION("Left subtree of Plus") {
    REQUIRE(plusTNode->type == TNodeType::Plus);
    REQUIRE(variableATNode->type == TNodeType::Variable);
    REQUIRE(variableATNode->name == "a");
    REQUIRE(variableBTNode->type == TNodeType::Variable);
    REQUIRE(variableBTNode->name == "b");
  }

  const std::unique_ptr<TNode> &variableCTNode =
      assignPlusTNode->children.back();
  SECTION("Right subtree of Plus") {
    REQUIRE(variableCTNode->type == TNodeType::Variable);
    REQUIRE(variableCTNode->name == "c");
  }

  SECTION("Assign is assigned statement number 1") {
    REQUIRE(assignTNode->statementNumber == 1);
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
  const std::unique_ptr<TNode> &programTNode = parser.buildAst(tokenQueue);
  const std::unique_ptr<TNode> &procedureTNode = programTNode->children.front();
  const std::unique_ptr<TNode> &stmtLstTNode = procedureTNode->children.front();

  SECTION("procedureTNode of AST has one StatementList child") {
    REQUIRE(stmtLstTNode->type == TNodeType::StatementList);
  }

  const std::unique_ptr<TNode> &assignTNode = stmtLstTNode->children.front();
  SECTION("StatementList has one Assign child") {
    REQUIRE(assignTNode->type == TNodeType::Assign);
  }

  const std::unique_ptr<TNode> &assignVariableTNode =
      assignTNode->children.front();
  const std::unique_ptr<TNode> &assignPlusTNode = assignTNode->children.back();
  SECTION("Assign has Variable and Plus children") {
    REQUIRE(assignVariableTNode->type == TNodeType::Variable);
    REQUIRE(assignVariableTNode->name == "x");
    REQUIRE(assignPlusTNode->type == TNodeType::Plus);
  }

  const std::unique_ptr<TNode> &multiplyTNode =
      assignPlusTNode->children.back();
  const std::unique_ptr<TNode> &variableBTNode =
      multiplyTNode->children.front();
  const std::unique_ptr<TNode> &variableCTNode = multiplyTNode->children.back();
  SECTION("Right subtree of Plus") {
    REQUIRE(multiplyTNode->type == TNodeType::Multiply);
    REQUIRE(variableBTNode->type == TNodeType::Variable);
    REQUIRE(variableBTNode->name == "b");
    REQUIRE(variableCTNode->type == TNodeType::Variable);
    REQUIRE(variableCTNode->name == "c");
  }

  const std::unique_ptr<TNode> &variableATNode =
      assignPlusTNode->children.front();
  SECTION("Left subtree of Plus") {
    REQUIRE(variableATNode->type == TNodeType::Variable);
    REQUIRE(variableATNode->name == "a");
  }

  SECTION("Assign is assigned statement number 1") {
    REQUIRE(assignTNode->statementNumber == 1);
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
  const std::unique_ptr<TNode> &programTNode = parser.buildAst(tokenQueue);
  const std::unique_ptr<TNode> &procedureTNode = programTNode->children.front();
  const std::unique_ptr<TNode> &stmtLstTNode = procedureTNode->children.front();

  SECTION("procedureTNode of AST has one StatementList child") {
    REQUIRE(stmtLstTNode->type == TNodeType::StatementList);
  }

  const std::unique_ptr<TNode> &assignTNode = stmtLstTNode->children.front();
  SECTION("StatementList has one Assign child") {
    REQUIRE(assignTNode->type == TNodeType::Assign);
  }

  const std::unique_ptr<TNode> &assignVariableTNode =
      assignTNode->children.front();
  const std::unique_ptr<TNode> &assignMultiplyTNode =
      assignTNode->children.back();
  SECTION("Assign has Variable and Plus children") {
    REQUIRE(assignVariableTNode->type == TNodeType::Variable);
    REQUIRE(assignVariableTNode->name == "x");
    REQUIRE(assignMultiplyTNode->type == TNodeType::Multiply);
  }

  const std::unique_ptr<TNode> &plusTNode =
      assignMultiplyTNode->children.front();
  const std::unique_ptr<TNode> &variableATNode = plusTNode->children.front();
  const std::unique_ptr<TNode> &variableBTNode = plusTNode->children.back();
  SECTION("Left subtree of Multiply") {
    REQUIRE(plusTNode->type == TNodeType::Plus);
    REQUIRE(variableATNode->type == TNodeType::Variable);
    REQUIRE(variableATNode->name == "a");
    REQUIRE(variableBTNode->type == TNodeType::Variable);
    REQUIRE(variableBTNode->name == "b");
  }

  const std::unique_ptr<TNode> &variableCTNode =
      assignMultiplyTNode->children.back();
  SECTION("Right subtree of Multiply") {
    REQUIRE(variableCTNode->type == TNodeType::Variable);
    REQUIRE(variableCTNode->name == "c");
  }

  SECTION("Assign is assigned statement number 1") {
    REQUIRE(assignTNode->statementNumber == 1);
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
  const std::unique_ptr<TNode> &programTNode = parser.buildAst(tokenQueue);
  const std::unique_ptr<TNode> &procedureTNode = programTNode->children.front();
  const std::unique_ptr<TNode> &stmtLstTNode = procedureTNode->children.front();

  SECTION("procedureTNode of AST has one StatementList child") {
    REQUIRE(stmtLstTNode->type == TNodeType::StatementList);
  }

  const std::unique_ptr<TNode> &assignTNode = stmtLstTNode->children.front();
  SECTION("StatementList has one Assign child") {
    REQUIRE(assignTNode->type == TNodeType::Assign);
  }

  const std::unique_ptr<TNode> &assignVariableTNode =
      assignTNode->children.front();
  const std::unique_ptr<TNode> &assignPlusTNode = assignTNode->children.back();
  SECTION("Assign has Variable and Plus children") {
    REQUIRE(assignVariableTNode->type == TNodeType::Variable);
    REQUIRE(assignVariableTNode->name == "x");
    REQUIRE(assignPlusTNode->type == TNodeType::Plus);
  }

  const std::unique_ptr<TNode> &multiplyTNode =
      assignPlusTNode->children.front();
  const std::unique_ptr<TNode> &variableATNode =
      multiplyTNode->children.front();
  const std::unique_ptr<TNode> &variableBTNode = multiplyTNode->children.back();
  SECTION("Left subtree of Plus") {
    REQUIRE(multiplyTNode->type == TNodeType::Multiply);
    REQUIRE(variableATNode->type == TNodeType::Variable);
    REQUIRE(variableATNode->name == "a");
    REQUIRE(variableBTNode->type == TNodeType::Variable);
    REQUIRE(variableBTNode->name == "b");
  }

  const std::unique_ptr<TNode> &variableCTNode =
      assignPlusTNode->children.back();
  SECTION("Right subtree of Plus") {
    REQUIRE(variableCTNode->type == TNodeType::Variable);
    REQUIRE(variableCTNode->name == "c");
  }

  SECTION("Assign is assigned statement number 1") {
    REQUIRE(assignTNode->statementNumber == 1);
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
  const std::unique_ptr<TNode> &programTNode = parser.buildAst(tokenQueue);
  const std::unique_ptr<TNode> &procedureTNode = programTNode->children.front();
  const std::unique_ptr<TNode> &stmtLstTNode = procedureTNode->children.front();

  SECTION("procedureTNode of AST has one StatementList child") {
    REQUIRE(stmtLstTNode->type == TNodeType::StatementList);
  }

  const std::unique_ptr<TNode> &assignTNode = stmtLstTNode->children.front();
  SECTION("StatementList has one Assign child") {
    REQUIRE(assignTNode->type == TNodeType::Assign);
  }

  const std::unique_ptr<TNode> &assignVariableTNode =
      assignTNode->children.front();
  const std::unique_ptr<TNode> &assignMultiplyTNode =
      assignTNode->children.back();
  SECTION("Assign has Variable and Plus children") {
    REQUIRE(assignVariableTNode->type == TNodeType::Variable);
    REQUIRE(assignVariableTNode->name == "x");
    REQUIRE(assignMultiplyTNode->type == TNodeType::Multiply);
  }

  const std::unique_ptr<TNode> &plusTNode =
      assignMultiplyTNode->children.back();
  const std::unique_ptr<TNode> &variableBTNode = plusTNode->children.front();
  const std::unique_ptr<TNode> &variableCTNode = plusTNode->children.back();
  SECTION("Right subtree of Plus") {
    REQUIRE(plusTNode->type == TNodeType::Plus);
    REQUIRE(variableBTNode->type == TNodeType::Variable);
    REQUIRE(variableBTNode->name == "b");
    REQUIRE(variableCTNode->type == TNodeType::Variable);
    REQUIRE(variableCTNode->name == "c");
  }

  const std::unique_ptr<TNode> &variableATNode =
      assignMultiplyTNode->children.front();
  SECTION("Left subtree of Plus") {
    REQUIRE(variableATNode->type == TNodeType::Variable);
    REQUIRE(variableATNode->name == "a");
  }

  SECTION("Assign is assigned statement number 1") {
    REQUIRE(assignTNode->statementNumber == 1);
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
  const std::unique_ptr<TNode> &programTNode = parser.buildAst(tokenQueue);
  const std::unique_ptr<TNode> &procedureTNode = programTNode->children.front();
  const std::unique_ptr<TNode> &stmtLstTNode = procedureTNode->children.front();

  SECTION("procedureTNode of AST has one StatementList child") {
    REQUIRE(stmtLstTNode->type == TNodeType::StatementList);
  }

  const std::unique_ptr<TNode> &assignTNode = stmtLstTNode->children.front();
  SECTION("StatementList has one Assign child") {
    REQUIRE(assignTNode->type == TNodeType::Assign);
  }

  const std::unique_ptr<TNode> &assignVariableTNode =
      assignTNode->children.front();
  const std::unique_ptr<TNode> &assignPlusTNode = assignTNode->children.back();
  SECTION("Assign has Variable and Plus children") {
    REQUIRE(assignVariableTNode->type == TNodeType::Variable);
    REQUIRE(assignVariableTNode->name == "x");
    REQUIRE(assignPlusTNode->type == TNodeType::Plus);
  }

  const std::unique_ptr<TNode> &plusTNode = assignPlusTNode->children.front();
  const std::unique_ptr<TNode> &multiplyTNode = plusTNode->children.front();
  const std::unique_ptr<TNode> &variableCTNode = plusTNode->children.back();
  SECTION("Left subtree of Plus") {
    REQUIRE(plusTNode->type == TNodeType::Plus);
    REQUIRE(multiplyTNode->type == TNodeType::Multiply);
    REQUIRE(variableCTNode->type == TNodeType::Variable);
    REQUIRE(variableCTNode->name == "c");
  }

  const std::unique_ptr<TNode> &variableATNode =
      multiplyTNode->children.front();
  const std::unique_ptr<TNode> &variableBTNode = multiplyTNode->children.back();
  SECTION("Left of left subtree of Plus") {
    REQUIRE(variableATNode->type == TNodeType::Variable);
    REQUIRE(variableATNode->name == "a");
    REQUIRE(variableBTNode->type == TNodeType::Variable);
    REQUIRE(variableBTNode->name == "b");
  }

  const std::unique_ptr<TNode> &variableDTNode =
      assignPlusTNode->children.back();
  SECTION("Right subtree of Plus") {
    REQUIRE(variableDTNode->type == TNodeType::Variable);
    REQUIRE(variableDTNode->name == "d");
  }

  SECTION("Assign is assigned statement number 1") {
    REQUIRE(assignTNode->statementNumber == 1);
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
  const std::unique_ptr<TNode> &programTNode = parser.buildAst(tokenQueue);
  const std::unique_ptr<TNode> &procedureTNode = programTNode->children.front();
  const std::unique_ptr<TNode> &stmtLstTNode = procedureTNode->children.front();

  SECTION("procedureTNode of AST has one StatementList child") {
    REQUIRE(stmtLstTNode->type == TNodeType::StatementList);
  }

  const std::unique_ptr<TNode> &assignTNode = stmtLstTNode->children.front();
  SECTION("StatementList has one Assign child") {
    REQUIRE(assignTNode->type == TNodeType::Assign);
  }

  const std::unique_ptr<TNode> &assignVariableTNode =
      assignTNode->children.front();
  const std::unique_ptr<TNode> &assignPlusTNode = assignTNode->children.back();
  SECTION("Assign has Variable and Plus children") {
    REQUIRE(assignVariableTNode->type == TNodeType::Variable);
    REQUIRE(assignVariableTNode->name == "x");
    REQUIRE(assignPlusTNode->type == TNodeType::Plus);
  }

  const std::unique_ptr<TNode> &plusTNode = assignPlusTNode->children.front();
  const std::unique_ptr<TNode> &multiplyTNode = plusTNode->children.back();
  const std::unique_ptr<TNode> &variableATNode = plusTNode->children.front();
  SECTION("Left subtree of Plus") {
    REQUIRE(plusTNode->type == TNodeType::Plus);
    REQUIRE(multiplyTNode->type == TNodeType::Multiply);
    REQUIRE(variableATNode->type == TNodeType::Variable);
    REQUIRE(variableATNode->name == "a");
  }

  const std::unique_ptr<TNode> &variableBTNode =
      multiplyTNode->children.front();
  const std::unique_ptr<TNode> &variableCTNode = multiplyTNode->children.back();
  SECTION("Right of left subtree of Plus") {
    REQUIRE(variableBTNode->type == TNodeType::Variable);
    REQUIRE(variableBTNode->name == "b");
    REQUIRE(variableCTNode->type == TNodeType::Variable);
    REQUIRE(variableCTNode->name == "c");
  }

  const std::unique_ptr<TNode> &variableDTNode =
      assignPlusTNode->children.back();
  SECTION("Right subtree of Plus") {
    REQUIRE(variableDTNode->type == TNodeType::Variable);
    REQUIRE(variableDTNode->name == "d");
  }

  SECTION("Assign is assigned statement number 1") {
    REQUIRE(assignTNode->statementNumber == 1);
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
  const std::unique_ptr<TNode> &programTNode = parser.buildAst(tokenQueue);
  const std::unique_ptr<TNode> &procedureTNode = programTNode->children.front();
  const std::unique_ptr<TNode> &stmtLstTNode = procedureTNode->children.front();

  SECTION("procedureTNode of AST has one StatementList child") {
    REQUIRE(stmtLstTNode->type == TNodeType::StatementList);
  }

  const std::unique_ptr<TNode> &assignTNode = stmtLstTNode->children.front();
  SECTION("StatementList has one Assign child") {
    REQUIRE(assignTNode->type == TNodeType::Assign);
  }

  const std::unique_ptr<TNode> &assignVariableTNode =
      assignTNode->children.front();
  const std::unique_ptr<TNode> &assignPlusTNode = assignTNode->children.back();
  SECTION("Assign has Variable and Plus children") {
    REQUIRE(assignVariableTNode->type == TNodeType::Variable);
    REQUIRE(assignVariableTNode->name == "x");
    REQUIRE(assignPlusTNode->type == TNodeType::Plus);
  }

  const std::unique_ptr<TNode> &plusTNode = assignPlusTNode->children.front();
  const std::unique_ptr<TNode> &variableATNode = plusTNode->children.front();
  const std::unique_ptr<TNode> &variableBTNode = plusTNode->children.back();
  SECTION("Left subtree of Plus") {
    REQUIRE(plusTNode->type == TNodeType::Plus);
    REQUIRE(variableATNode->type == TNodeType::Variable);
    REQUIRE(variableATNode->name == "a");
    REQUIRE(variableBTNode->type == TNodeType::Variable);
    REQUIRE(variableBTNode->name == "b");
  }

  const std::unique_ptr<TNode> &multiplyTNode =
      assignPlusTNode->children.back();
  const std::unique_ptr<TNode> &variableCTNode =
      multiplyTNode->children.front();
  const std::unique_ptr<TNode> &variableDTNode = multiplyTNode->children.back();
  SECTION("Right subtree of Plus") {
    REQUIRE(multiplyTNode->type == TNodeType::Multiply);
    REQUIRE(variableCTNode->type == TNodeType::Variable);
    REQUIRE(variableCTNode->name == "c");
    REQUIRE(variableDTNode->type == TNodeType::Variable);
    REQUIRE(variableDTNode->name == "d");
  }

  SECTION("Assign is assigned statement number 1") {
    REQUIRE(assignTNode->statementNumber == 1);
  }
}

SCENARIO("Assignment statement: x = (a + (b + c)) + (d + (e + f)) + g") {
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
  tokenQueue.push(Token(TokenType::OpenParenthesis, "("));
  tokenQueue.push(Token(TokenType::Identifier, "b"));
  tokenQueue.push(Token(TokenType::Plus, "+"));
  tokenQueue.push(Token(TokenType::Identifier, "c"));
  tokenQueue.push(Token(TokenType::CloseParenthesis, ")"));
  tokenQueue.push(Token(TokenType::CloseParenthesis, ")"));
  tokenQueue.push(Token(TokenType::Plus, "+"));
  tokenQueue.push(Token(TokenType::OpenParenthesis, "("));
  tokenQueue.push(Token(TokenType::Identifier, "d"));
  tokenQueue.push(Token(TokenType::Plus, "+"));
  tokenQueue.push(Token(TokenType::OpenParenthesis, "("));
  tokenQueue.push(Token(TokenType::Identifier, "e"));
  tokenQueue.push(Token(TokenType::Plus, "+"));
  tokenQueue.push(Token(TokenType::Identifier, "f"));
  tokenQueue.push(Token(TokenType::CloseParenthesis, ")"));
  tokenQueue.push(Token(TokenType::CloseParenthesis, ")"));
  tokenQueue.push(Token(TokenType::Plus, "+"));
  tokenQueue.push(Token(TokenType::Identifier, "g"));
  tokenQueue.push(Token(TokenType::Separator, ";"));
  tokenQueue.push(Token(TokenType::Separator, "}"));
  const std::unique_ptr<TNode> &programTNode = parser.buildAst(tokenQueue);
  const std::unique_ptr<TNode> &procedureTNode = programTNode->children.front();
  const std::unique_ptr<TNode> &stmtLstTNode = procedureTNode->children.front();

  SECTION("procedureTNode of AST has one StatementList child") {
    REQUIRE(stmtLstTNode->type == TNodeType::StatementList);
  }

  const std::unique_ptr<TNode> &assignTNode = stmtLstTNode->children.front();
  SECTION("StatementList has one Assign child") {
    REQUIRE(assignTNode->type == TNodeType::Assign);
  }

  const std::unique_ptr<TNode> &assignVariableTNode =
      assignTNode->children.front();
  const std::unique_ptr<TNode> &assignPlusTNode = assignTNode->children.back();
  SECTION("Assign has Variable and Plus children") {
    REQUIRE(assignVariableTNode->type == TNodeType::Variable);
    REQUIRE(assignVariableTNode->name == "x");
    REQUIRE(assignPlusTNode->type == TNodeType::Plus);
  }

  const std::unique_ptr<TNode> &plusTNode1 = assignPlusTNode->children.front();
  const std::unique_ptr<TNode> &variableGTNode =
      assignPlusTNode->children.back();
  SECTION("Children of Assign->Plus") {
    REQUIRE(plusTNode1->type == TNodeType::Plus);
    REQUIRE(variableGTNode->type == TNodeType::Variable);
    REQUIRE(variableGTNode->name == "g");
  }

  const std::unique_ptr<TNode> &plusTNode2 = plusTNode1->children.front();
  const std::unique_ptr<TNode> &plusTNode3 = plusTNode1->children.back();
  SECTION("Children of Assign->Plus->Plus (Left)") {
    REQUIRE(plusTNode2->type == TNodeType::Plus);
    REQUIRE(plusTNode3->type == TNodeType::Plus);
  }

  const std::unique_ptr<TNode> &variableATNode = plusTNode2->children.front();
  const std::unique_ptr<TNode> &plusTNode4 = plusTNode2->children.back();
  SECTION("Children of Assign->Plus->Plus->Plus (Left)") {
    REQUIRE(plusTNode4->type == TNodeType::Plus);
    REQUIRE(variableATNode->type == TNodeType::Variable);
    REQUIRE(variableATNode->name == "a");
  }

  const std::unique_ptr<TNode> &variableBTNode = plusTNode4->children.front();
  const std::unique_ptr<TNode> &variableCTNode = plusTNode4->children.back();
  SECTION("Children of Assign->Plus->Plus->Plus->Plus (Left)") {
    REQUIRE(variableBTNode->type == TNodeType::Variable);
    REQUIRE(variableBTNode->name == "b");
    REQUIRE(variableCTNode->type == TNodeType::Variable);
    REQUIRE(variableCTNode->name == "c");
  }

  const std::unique_ptr<TNode> &variableDTNode = plusTNode3->children.front();
  const std::unique_ptr<TNode> &plusTNode5 = plusTNode3->children.back();
  SECTION("Children of Assign->Plus->Plus (Right)") {
    REQUIRE(plusTNode5->type == TNodeType::Plus);
    REQUIRE(variableDTNode->type == TNodeType::Variable);
    REQUIRE(variableDTNode->name == "d");
  }

  const std::unique_ptr<TNode> &variableETNode = plusTNode5->children.front();
  const std::unique_ptr<TNode> &variableFTNode = plusTNode5->children.back();
  SECTION("Children of Assign->Plus->Plus->Plus->Plus (Right)") {
    REQUIRE(variableETNode->type == TNodeType::Variable);
    REQUIRE(variableETNode->name == "e");
    REQUIRE(variableFTNode->type == TNodeType::Variable);
    REQUIRE(variableFTNode->name == "f");
  }

  SECTION("Assign is assigned statement number 1") {
    REQUIRE(assignTNode->statementNumber == 1);
  }
}

SCENARIO("Assignment statement: x = (a + b * c d * (e + f)) * g") {
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
  tokenQueue.push(Token(TokenType::Multiply, "*"));
  tokenQueue.push(Token(TokenType::Identifier, "c"));
  tokenQueue.push(Token(TokenType::Plus, "+"));
  tokenQueue.push(Token(TokenType::Identifier, "d"));
  tokenQueue.push(Token(TokenType::Multiply, "*"));
  tokenQueue.push(Token(TokenType::OpenParenthesis, "("));
  tokenQueue.push(Token(TokenType::Identifier, "e"));
  tokenQueue.push(Token(TokenType::Plus, "+"));
  tokenQueue.push(Token(TokenType::Identifier, "f"));
  tokenQueue.push(Token(TokenType::CloseParenthesis, ")"));
  tokenQueue.push(Token(TokenType::CloseParenthesis, ")"));
  tokenQueue.push(Token(TokenType::Multiply, "*"));
  tokenQueue.push(Token(TokenType::Identifier, "g"));
  tokenQueue.push(Token(TokenType::Separator, ";"));
  tokenQueue.push(Token(TokenType::Separator, "}"));
  const std::unique_ptr<TNode> &programTNode = parser.buildAst(tokenQueue);
  const std::unique_ptr<TNode> &procedureTNode = programTNode->children.front();
  const std::unique_ptr<TNode> &stmtLstTNode = procedureTNode->children.front();

  SECTION("procedureTNode of AST has one StatementList child") {
    REQUIRE(stmtLstTNode->type == TNodeType::StatementList);
  }

  const std::unique_ptr<TNode> &assignTNode = stmtLstTNode->children.front();
  SECTION("StatementList has one Assign child") {
    REQUIRE(assignTNode->type == TNodeType::Assign);
  }

  const std::unique_ptr<TNode> &assignVariableTNode =
      assignTNode->children.front();
  const std::unique_ptr<TNode> &assignMultiplyTNode =
      assignTNode->children.back();
  SECTION("Assign has Variable and Mutliply children") {
    REQUIRE(assignVariableTNode->type == TNodeType::Variable);
    REQUIRE(assignVariableTNode->name == "x");
    REQUIRE(assignMultiplyTNode->type == TNodeType::Multiply);
  }

  const std::unique_ptr<TNode> &plusTNode1 =
      assignMultiplyTNode->children.front();
  const std::unique_ptr<TNode> &variableGTNode =
      assignMultiplyTNode->children.back();
  SECTION("Children of Assign->Multiply") {
    REQUIRE(plusTNode1->type == TNodeType::Plus);
    REQUIRE(variableGTNode->type == TNodeType::Variable);
    REQUIRE(variableGTNode->name == "g");
  }

  const std::unique_ptr<TNode> &plusTNode2 = plusTNode1->children.front();
  const std::unique_ptr<TNode> &multiplyTNode1 = plusTNode1->children.back();
  SECTION("Children of Assign->Multiply->Plus") {
    REQUIRE(plusTNode2->type == TNodeType::Plus);
    REQUIRE(multiplyTNode1->type == TNodeType::Multiply);
  }

  const std::unique_ptr<TNode> &variableATNode = plusTNode2->children.front();
  const std::unique_ptr<TNode> &multiplyTNode2 = plusTNode2->children.back();
  SECTION("Children of Assign->Multiply->Plus->Plus") {
    REQUIRE(multiplyTNode2->type == TNodeType::Multiply);
    REQUIRE(variableATNode->type == TNodeType::Variable);
    REQUIRE(variableATNode->name == "a");
  }

  const std::unique_ptr<TNode> &variableBTNode =
      multiplyTNode2->children.front();
  const std::unique_ptr<TNode> &variableCTNode =
      multiplyTNode2->children.back();
  SECTION("Children of Assign->Multiply->Plus->Plus->Multiply") {
    REQUIRE(variableBTNode->type == TNodeType::Variable);
    REQUIRE(variableBTNode->name == "b");
    REQUIRE(variableCTNode->type == TNodeType::Variable);
    REQUIRE(variableCTNode->name == "c");
  }

  const std::unique_ptr<TNode> &variableDTNode =
      multiplyTNode1->children.front();
  const std::unique_ptr<TNode> &plusTNode3 = multiplyTNode1->children.back();
  SECTION("Children of Assign->Plus->Multiply") {
    REQUIRE(plusTNode3->type == TNodeType::Plus);
    REQUIRE(variableDTNode->type == TNodeType::Variable);
    REQUIRE(variableDTNode->name == "d");
  }

  const std::unique_ptr<TNode> &variableETNode = plusTNode3->children.front();
  const std::unique_ptr<TNode> &variableFTNode = plusTNode3->children.back();
  SECTION("Children of Assign->Multiply->Plus->Multiply->Plus") {
    REQUIRE(variableETNode->type == TNodeType::Variable);
    REQUIRE(variableETNode->name == "e");
    REQUIRE(variableFTNode->type == TNodeType::Variable);
    REQUIRE(variableFTNode->name == "f");
  }

  SECTION("Assign is assigned statement number 1") {
    REQUIRE(assignTNode->statementNumber == 1);
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
  const std::unique_ptr<TNode> &programTNode = parser.buildAst(tokenQueue);
  const std::unique_ptr<TNode> &procedureTNode = programTNode->children.front();
  const std::unique_ptr<TNode> &stmtLstTNode = procedureTNode->children.front();

  SECTION("procedureTNode of AST has one StatementList child") {
    REQUIRE(stmtLstTNode->type == TNodeType::StatementList);
  }

  const std::unique_ptr<TNode> &whileTNode = stmtLstTNode->children.front();
  SECTION("StatementList has one While child") {
    REQUIRE(whileTNode->type == TNodeType::While);
  }

  const std::unique_ptr<TNode> &whileCondExprNotTNode =
      whileTNode->children.front();
  const std::unique_ptr<TNode> &whileStmtLstTNode = whileTNode->children.back();
  SECTION("While has ConditionExpression and StatementList children") {
    REQUIRE(whileCondExprNotTNode->type == TNodeType::Not);
    REQUIRE(whileStmtLstTNode->type == TNodeType::StatementList);
  }

  SECTION("ConditionExpression has Variable and Constant children") {
    const std::unique_ptr<TNode> &whileCondExprEqualTNode =
        whileCondExprNotTNode->children.front();
    const std::unique_ptr<TNode> &variableTNode =
        whileCondExprEqualTNode->children.front();
    const std::unique_ptr<TNode> &constantTNode =
        whileCondExprEqualTNode->children.back();
    REQUIRE(whileCondExprEqualTNode->type == TNodeType::Equal);
    REQUIRE(variableTNode->type == TNodeType::Variable);
    REQUIRE(variableTNode->name == "x");
    REQUIRE(constantTNode->type == TNodeType::Constant);
    REQUIRE(constantTNode->name == "0");
  }

  const std::unique_ptr<TNode> &readTNode = whileStmtLstTNode->children.front();
  SECTION("WhileStatementList has one Read child") {
    const std::unique_ptr<TNode> &variableTNode = readTNode->children.front();
    REQUIRE(readTNode->type == TNodeType::Read);
    REQUIRE(variableTNode->type == TNodeType::Variable);
    REQUIRE(variableTNode->name == "x");
  }

  SECTION("Statement numbers are assigned accordingly") {
    REQUIRE(whileTNode->statementNumber == 1);
    REQUIRE(readTNode->statementNumber == 2);
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
  const std::unique_ptr<TNode> &programTNode = parser.buildAst(tokenQueue);
  const std::unique_ptr<TNode> &procedureTNode = programTNode->children.front();
  const std::unique_ptr<TNode> &stmtLstTNode = procedureTNode->children.front();

  SECTION("procedureTNode of AST has one StatementList child") {
    REQUIRE(stmtLstTNode->type == TNodeType::StatementList);
  }

  const std::unique_ptr<TNode> &whileTNode = stmtLstTNode->children.front();
  SECTION("StatementList has one While child") {
    REQUIRE(whileTNode->type == TNodeType::While);
  }

  const std::unique_ptr<TNode> &whileCondExprAndTNode =
      whileTNode->children.front();
  const std::unique_ptr<TNode> &whileStmtLstTNode = whileTNode->children.back();
  SECTION("While has ConditionExpression and StatementList children") {
    REQUIRE(whileCondExprAndTNode->type == TNodeType::And);
    REQUIRE(whileStmtLstTNode->type == TNodeType::StatementList);
  }

  SECTION("ConditionExpression has two Equal children") {
    const std::unique_ptr<TNode> &whileCondExprLeftEqualTNode =
        whileCondExprAndTNode->children.front();
    const std::unique_ptr<TNode> &whileCondExprRightEqualTNode =
        whileCondExprAndTNode->children.back();
    const std::unique_ptr<TNode> &leftCondExprVariableTNode =
        whileCondExprLeftEqualTNode->children.front();
    const std::unique_ptr<TNode> &leftCondExprConstantTNode =
        whileCondExprLeftEqualTNode->children.back();
    const std::unique_ptr<TNode> &rightCondExprVariableTNode =
        whileCondExprRightEqualTNode->children.front();
    const std::unique_ptr<TNode> &rightCondExprConstantTNode =
        whileCondExprRightEqualTNode->children.back();
    REQUIRE(whileCondExprLeftEqualTNode->type == TNodeType::Equal);
    REQUIRE(leftCondExprVariableTNode->type == TNodeType::Variable);
    REQUIRE(leftCondExprVariableTNode->name == "x");
    REQUIRE(leftCondExprConstantTNode->type == TNodeType::Constant);
    REQUIRE(leftCondExprConstantTNode->name == "0");
    REQUIRE(whileCondExprRightEqualTNode->type == TNodeType::Equal);
    REQUIRE(rightCondExprVariableTNode->type == TNodeType::Variable);
    REQUIRE(rightCondExprVariableTNode->name == "y");
    REQUIRE(rightCondExprConstantTNode->type == TNodeType::Constant);
    REQUIRE(rightCondExprConstantTNode->name == "0");
  }

  const std::unique_ptr<TNode> &readTNode = whileStmtLstTNode->children.front();
  SECTION("WhileStatementList has one Read child") {
    const std::unique_ptr<TNode> &variableTNode = readTNode->children.front();
    REQUIRE(readTNode->type == TNodeType::Read);
    REQUIRE(variableTNode->type == TNodeType::Variable);
    REQUIRE(variableTNode->name == "x");
  }

  SECTION("Statement numbers are assigned accordingly") {
    REQUIRE(whileTNode->statementNumber == 1);
    REQUIRE(readTNode->statementNumber == 2);
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
  const std::unique_ptr<TNode> &programTNode = parser.buildAst(tokenQueue);
  const std::unique_ptr<TNode> &procedureTNode = programTNode->children.front();
  const std::unique_ptr<TNode> &stmtLstTNode = procedureTNode->children.front();

  SECTION("procedureTNode of AST has one StatementList child") {
    REQUIRE(stmtLstTNode->type == TNodeType::StatementList);
  }

  const std::unique_ptr<TNode> &whileTNode = stmtLstTNode->children.front();
  SECTION("StatementList has one While child") {
    REQUIRE(whileTNode->type == TNodeType::While);
  }

  const std::unique_ptr<TNode> &whileCondExprOrTNode =
      whileTNode->children.front();
  const std::unique_ptr<TNode> &whileStmtLstTNode = whileTNode->children.back();
  SECTION("While has ConditionExpression and StatementList children") {
    REQUIRE(whileCondExprOrTNode->type == TNodeType::Or);
    REQUIRE(whileStmtLstTNode->type == TNodeType::StatementList);
  }

  const std::unique_ptr<TNode> &whileCondExprAndTNode =
      whileCondExprOrTNode->children.front();
  const std::unique_ptr<TNode> &whileCondExprEqualTNode =
      whileCondExprOrTNode->children.back();
  SECTION("ConditionExpression has And and Equal children") {
    REQUIRE(whileCondExprAndTNode->type == TNodeType::And);
    REQUIRE(whileCondExprEqualTNode->type == TNodeType::Equal);
  }

  SECTION("Right subtree of ConditionExpression of Or") {
    const std::unique_ptr<TNode> &condExprVariableTNode =
        whileCondExprEqualTNode->children.front();
    const std::unique_ptr<TNode> &condExprConstantTNode =
        whileCondExprEqualTNode->children.back();
    REQUIRE(condExprVariableTNode->type == TNodeType::Variable);
    REQUIRE(condExprVariableTNode->name == "z");
    REQUIRE(condExprConstantTNode->type == TNodeType::Constant);
    REQUIRE(condExprConstantTNode->name == "0");
  }

  SECTION("Left subtree of ConditionExpression of Or") {
    const std::unique_ptr<TNode> &leftCondExprTNode =
        whileCondExprAndTNode->children.front();
    const std::unique_ptr<TNode> &rightCondExprTNode =
        whileCondExprAndTNode->children.back();
    const std::unique_ptr<TNode> &leftCondExprVariableTNode =
        leftCondExprTNode->children.front();
    const std::unique_ptr<TNode> &leftCondExprConstantTNode =
        leftCondExprTNode->children.back();
    const std::unique_ptr<TNode> &rightCondExprVariableTNode =
        rightCondExprTNode->children.front();
    const std::unique_ptr<TNode> &rightCondExprConstantTNode =
        rightCondExprTNode->children.back();
    REQUIRE(leftCondExprTNode->type == TNodeType::Equal);
    REQUIRE(leftCondExprVariableTNode->type == TNodeType::Variable);
    REQUIRE(leftCondExprVariableTNode->name == "x");
    REQUIRE(leftCondExprConstantTNode->type == TNodeType::Constant);
    REQUIRE(leftCondExprConstantTNode->name == "0");
    REQUIRE(rightCondExprTNode->type == TNodeType::Equal);
    REQUIRE(rightCondExprVariableTNode->type == TNodeType::Variable);
    REQUIRE(rightCondExprVariableTNode->name == "y");
    REQUIRE(rightCondExprConstantTNode->type == TNodeType::Constant);
    REQUIRE(rightCondExprConstantTNode->name == "0");
  }

  const std::unique_ptr<TNode> &readTNode = whileStmtLstTNode->children.front();
  SECTION("WhileStatementList has one Read child") {
    const std::unique_ptr<TNode> &variableTNode = readTNode->children.front();
    REQUIRE(readTNode->type == TNodeType::Read);
    REQUIRE(variableTNode->type == TNodeType::Variable);
    REQUIRE(variableTNode->name == "x");
  }

  SECTION("Statement numbers are assigned accordingly") {
    REQUIRE(whileTNode->statementNumber == 1);
    REQUIRE(readTNode->statementNumber == 2);
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
  const std::unique_ptr<TNode> &programTNode = parser.buildAst(tokenQueue);
  const std::unique_ptr<TNode> &procedureTNode = programTNode->children.front();
  const std::unique_ptr<TNode> &stmtLstTNode = procedureTNode->children.front();

  SECTION("procedureTNode of AST has one StatementList child") {
    REQUIRE(stmtLstTNode->type == TNodeType::StatementList);
  }

  const std::unique_ptr<TNode> &readXTNode = stmtLstTNode->children[0];
  const std::unique_ptr<TNode> &printTNode = stmtLstTNode->children[1];
  const std::unique_ptr<TNode> &whileTNode = stmtLstTNode->children[2];
  const std::unique_ptr<TNode> &readZTNode = stmtLstTNode->children[3];
  SECTION("StatementList has two Read, one While and one Print children") {
    REQUIRE(readXTNode->type == TNodeType::Read);
    REQUIRE(printTNode->type == TNodeType::Print);
    REQUIRE(whileTNode->type == TNodeType::While);
    REQUIRE(readZTNode->type == TNodeType::Read);
  }

  SECTION("Read has one Variable child") {
    const std::unique_ptr<TNode> &variableXTNode = readXTNode->children.front();
    const std::unique_ptr<TNode> &variableZTNode = readZTNode->children.front();
    REQUIRE(variableXTNode->type == TNodeType::Variable);
    REQUIRE(variableXTNode->name == "x");
    REQUIRE(variableZTNode->type == TNodeType::Variable);
    REQUIRE(variableZTNode->name == "z");
  }

  SECTION("Print has one Variable child") {
    const std::unique_ptr<TNode> &variableTNode = printTNode->children.front();
    REQUIRE(variableTNode->type == TNodeType::Variable);
    REQUIRE(variableTNode->name == "y");
  }

  const std::unique_ptr<TNode> &whileCondExprOrTNode =
      whileTNode->children.front();
  const std::unique_ptr<TNode> &whileStmtLstTNode = whileTNode->children.back();
  SECTION("While has ConditionExpression and StatementList children") {
    REQUIRE(whileCondExprOrTNode->type == TNodeType::Or);
    REQUIRE(whileStmtLstTNode->type == TNodeType::StatementList);
  }

  const std::unique_ptr<TNode> &whileCondExprAndTNode =
      whileCondExprOrTNode->children.front();
  const std::unique_ptr<TNode> &whileCondExprEqualTNode =
      whileCondExprOrTNode->children.back();
  SECTION("ConditionExpression has And and Equal children") {
    REQUIRE(whileCondExprAndTNode->type == TNodeType::And);
    REQUIRE(whileCondExprEqualTNode->type == TNodeType::Equal);
  }

  SECTION("Right subtree of ConditionExpression of Or") {
    const std::unique_ptr<TNode> &condExprVariableTNode =
        whileCondExprEqualTNode->children.front();
    const std::unique_ptr<TNode> &condExprConstantTNode =
        whileCondExprEqualTNode->children.back();
    REQUIRE(condExprVariableTNode->type == TNodeType::Variable);
    REQUIRE(condExprVariableTNode->name == "z");
    REQUIRE(condExprConstantTNode->type == TNodeType::Constant);
    REQUIRE(condExprConstantTNode->name == "0");
  }

  SECTION("Left subtree of ConditionExpression of Or") {
    const std::unique_ptr<TNode> &leftCondExprTNode =
        whileCondExprAndTNode->children.front();
    const std::unique_ptr<TNode> &rightCondExprTNode =
        whileCondExprAndTNode->children.back();
    const std::unique_ptr<TNode> &leftCondExprVariableTNode =
        leftCondExprTNode->children.front();
    const std::unique_ptr<TNode> &leftCondExprConstantTNode =
        leftCondExprTNode->children.back();
    const std::unique_ptr<TNode> &rightCondExprVariableTNode =
        rightCondExprTNode->children.front();
    const std::unique_ptr<TNode> &rightCondExprConstantTNode =
        rightCondExprTNode->children.back();
    REQUIRE(leftCondExprTNode->type == TNodeType::Equal);
    REQUIRE(leftCondExprVariableTNode->type == TNodeType::Variable);
    REQUIRE(leftCondExprVariableTNode->name == "x");
    REQUIRE(leftCondExprConstantTNode->type == TNodeType::Constant);
    REQUIRE(leftCondExprConstantTNode->name == "0");
    REQUIRE(rightCondExprTNode->type == TNodeType::Equal);
    REQUIRE(rightCondExprVariableTNode->type == TNodeType::Variable);
    REQUIRE(rightCondExprVariableTNode->name == "y");
    REQUIRE(rightCondExprConstantTNode->type == TNodeType::Constant);
    REQUIRE(rightCondExprConstantTNode->name == "0");
  }

  const std::unique_ptr<TNode> &firstAssignTNode =
      whileStmtLstTNode->children.front();
  const std::unique_ptr<TNode> &secondAssignTNode =
      whileStmtLstTNode->children.back();
  SECTION("WhileStatementList has two Assign children") {
    const std::unique_ptr<TNode> &firstVariableTNode =
        firstAssignTNode->children.front();
    const std::unique_ptr<TNode> &divideTNode =
        firstAssignTNode->children.back();
    const std::unique_ptr<TNode> &divideLeftTNode =
        divideTNode->children.front();
    const std::unique_ptr<TNode> &divideRightTNode =
        divideTNode->children.back();
    const std::unique_ptr<TNode> &secondVariableTNode =
        secondAssignTNode->children.front();
    const std::unique_ptr<TNode> &variableTNode =
        secondAssignTNode->children.back();
    REQUIRE(firstAssignTNode->type == TNodeType::Assign);
    REQUIRE(secondAssignTNode->type == TNodeType::Assign);
    REQUIRE(firstVariableTNode->type == TNodeType::Variable);
    REQUIRE(firstVariableTNode->name == "x");
    REQUIRE(divideTNode->type == TNodeType::Divide);
    REQUIRE(divideLeftTNode->type == TNodeType::Variable);
    REQUIRE(divideLeftTNode->name == "x");
    REQUIRE(divideRightTNode->type == TNodeType::Variable);
    REQUIRE(divideRightTNode->name == "y");
    REQUIRE(secondVariableTNode->type == TNodeType::Variable);
    REQUIRE(secondVariableTNode->name == "y");
    REQUIRE(variableTNode->type == TNodeType::Variable);
    REQUIRE(variableTNode->name == "x");
  }

  SECTION("Statement numbers are assigned accordingly") {
    REQUIRE(readXTNode->statementNumber == 1);
    REQUIRE(printTNode->statementNumber == 2);
    REQUIRE(whileTNode->statementNumber == 3);
    REQUIRE(firstAssignTNode->statementNumber == 4);
    REQUIRE(secondAssignTNode->statementNumber == 5);
    REQUIRE(readZTNode->statementNumber == 6);
  }
}
