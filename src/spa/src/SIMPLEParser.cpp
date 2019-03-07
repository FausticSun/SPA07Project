#include "SIMPLEParser.h"
#include "ExprParser.h"

namespace {
using namespace Parser;
Token expect(std::list<Token> &tokens, Token token);
void parseProgram(std::list<Token> &tokens, std::unique_ptr<PKB> &pkb,
                  std::list<std::string> &stack);
void parseProcedure(std::list<Token> &tokens, std::unique_ptr<PKB> &pkb,
                    std::list<std::string> &stack);
void parseStmtLst(std::list<Token> &tokens, std::unique_ptr<PKB> &pkb,
                  std::list<std::string> &stack);
void parseStmt(std::list<Token> &tokens, std::unique_ptr<PKB> &pkb,
               std::list<std::string> &stack);
void parseRead(std::list<Token> &tokens, std::unique_ptr<PKB> &pkb,
               std::list<std::string> &stack);
void parsePrint(std::list<Token> &tokens, std::unique_ptr<PKB> &pkb,
                std::list<std::string> &stack);
void parseCall(std::list<Token> &tokens, std::unique_ptr<PKB> &pkb,
               std::list<std::string> &stack);
void parseWhile(std::list<Token> &tokens, std::unique_ptr<PKB> &pkb,
                std::list<std::string> &stack);
void parseIf(std::list<Token> &tokens, std::unique_ptr<PKB> &pkb,
             std::list<std::string> &stack);
void parseAssign(std::list<Token> &tokens, std::unique_ptr<PKB> &pkb,
                 std::list<std::string> &stack);

Token expect(std::list<Token> &tokens, Token token) {
  if ((token.type == TokenType::Identifier && token.value == "" &&
       tokens.front().type == TokenType::Identifier) ||
      tokens.front() == token) {
    auto t = tokens.front();
    tokens.pop_front();
    return t;
  }
  throw std::logic_error("Expected " + token.value +
                         " but got: " + tokens.front().value);
  return token;
}

void parseProgram(std::list<Token> &tokens, std::unique_ptr<PKB> &pkb,
                  std::list<std::string> &stack) {
  do {
    if (tokens.empty()) {
      throw std::logic_error("Expected at least 1 procedure");
    }
    parseProcedure(tokens, pkb, stack);
  } while (!tokens.empty());
}

void parseProcedure(std::list<Token> &tokens, std::unique_ptr<PKB> &pkb,
                    std::list<std::string> &stack) {
  expect(tokens, SIMPLETokens::Procedure);
  expect(tokens, SIMPLETokens::Identifier);
  expect(tokens, SIMPLETokens::LeftBrace);
  parseStmtLst(tokens, pkb, stack);
  expect(tokens, SIMPLETokens::RightBrace);
}

void parseStmtLst(std::list<Token> &tokens, std::unique_ptr<PKB> &pkb,
                  std::list<std::string> &stack) {
  do {
    parseStmt(tokens, pkb, stack);
  } while (tokens.front() != SIMPLETokens::RightBrace);
}

void parseStmt(std::list<Token> &tokens, std::unique_ptr<PKB> &pkb,
               std::list<std::string> &stack) {
  if (tokens.front() == SIMPLETokens::Read) {
    parseRead(tokens, pkb, stack);
  } else if (tokens.front() == SIMPLETokens::Print) {
    parsePrint(tokens, pkb, stack);
  } else if (tokens.front() == SIMPLETokens::Call) {
    parseCall(tokens, pkb, stack);
  } else if (tokens.front() == SIMPLETokens::While) {
    parseWhile(tokens, pkb, stack);
  } else if (tokens.front() == SIMPLETokens::If) {
    parseIf(tokens, pkb, stack);
  } else if (tokens.front() == SIMPLETokens::Identifier) {
    parseAssign(tokens, pkb, stack);
  } else {
    throw std::logic_error("Error parsing statement");
  }
}

void parseRead(std::list<Token> &tokens, std::unique_ptr<PKB> &pkb,
               std::list<std::string> &stack) {
  expect(tokens, SIMPLETokens::Read);
  expect(tokens, SIMPLETokens::Identifier);
  expect(tokens, SIMPLETokens::Semicolon);
}
void parsePrint(std::list<Token> &tokens, std::unique_ptr<PKB> &pkb,
                std::list<std::string> &stack) {
  expect(tokens, SIMPLETokens::Print);
  expect(tokens, SIMPLETokens::Identifier);
  expect(tokens, SIMPLETokens::Semicolon);
}
void parseCall(std::list<Token> &tokens, std::unique_ptr<PKB> &pkb,
               std::list<std::string> &stack) {
  expect(tokens, SIMPLETokens::Call);
  expect(tokens, SIMPLETokens::Identifier);
  expect(tokens, SIMPLETokens::Semicolon);
}
void parseWhile(std::list<Token> &tokens, std::unique_ptr<PKB> &pkb,
                std::list<std::string> &stack) {
  expect(tokens, SIMPLETokens::While);
  expect(tokens, SIMPLETokens::LeftBrace);
  parseExpr(tokens);
  expect(tokens, SIMPLETokens::RightBrace);
  expect(tokens, SIMPLETokens::LeftParentheses);
  parseStmtLst(tokens, pkb, stack);
  expect(tokens, SIMPLETokens::RightParentheses);
}
void parseIf(std::list<Token> &tokens, std::unique_ptr<PKB> &pkb,
             std::list<std::string> &stack) {
  expect(tokens, SIMPLETokens::If);
  expect(tokens, SIMPLETokens::LeftBrace);
  parseExpr(tokens);
  expect(tokens, SIMPLETokens::RightBrace);
  expect(tokens, SIMPLETokens::Then);
  expect(tokens, SIMPLETokens::LeftParentheses);
  parseStmtLst(tokens, pkb, stack);
  expect(tokens, SIMPLETokens::RightParentheses);
  expect(tokens, SIMPLETokens::Else);
  expect(tokens, SIMPLETokens::LeftParentheses);
  parseStmtLst(tokens, pkb, stack);
  expect(tokens, SIMPLETokens::RightParentheses);
}
void parseAssign(std::list<Token> &tokens, std::unique_ptr<PKB> &pkb,
                 std::list<std::string> &stack) {
  expect(tokens, SIMPLETokens::Identifier);
  expect(tokens, SIMPLETokens::Assign);
  parseExpr(tokens);
  expect(tokens, SIMPLETokens::Semicolon);
}

} // namespace

namespace Parser {
std::unique_ptr<PKB> parseSIMPLE(std::list<Token> tokens) {
  auto pkb = std::unique_ptr<PKB>(new PKB());
  std::list<std::string> stack;
  parseProgram(tokens, pkb, stack);
  return pkb;
}
} // namespace Parser
