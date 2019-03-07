#include "SIMPLEParser.h"
#include "ExprParser.h"

namespace Parser {
Token SIMPLEParser::expect(Token token) {
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

std::string SIMPLEParser::getStmtNo() { return std::to_string(++stmtNo); }

void SIMPLEParser::parseProgram() {
  do {
    if (tokens.empty()) {
      throw std::logic_error("Expected at least 1 procedure");
    }
    parseProcedure();
  } while (!tokens.empty());
}

void SIMPLEParser::parseProcedure() {
  currentProc = expect(SIMPLETokens::Procedure).value;
  expect(SIMPLETokens::Identifier);
  expect(SIMPLETokens::LeftBrace);
  parseStmtLst();
  expect(SIMPLETokens::RightBrace);
}

void SIMPLEParser::parseStmtLst() {
  do {
    parseStmt();
  } while (tokens.front() != SIMPLETokens::RightBrace);
}

void SIMPLEParser::parseStmt() {
  if (tokens.front() == SIMPLETokens::Read) {
    parseRead();
  } else if (tokens.front() == SIMPLETokens::Print) {
    parsePrint();
  } else if (tokens.front() == SIMPLETokens::Call) {
    parseCall();
  } else if (tokens.front() == SIMPLETokens::While) {
    parseWhile();
  } else if (tokens.front() == SIMPLETokens::If) {
    parseIf();
  } else if (tokens.front() == SIMPLETokens::Identifier) {
    parseAssign();
  } else {
    throw std::logic_error("Error parsing statement");
  }
}

void SIMPLEParser::parseRead() {
  expect(SIMPLETokens::Read);
  expect(SIMPLETokens::Identifier);
  expect(SIMPLETokens::Semicolon);
}

void SIMPLEParser::parsePrint() {
  expect(SIMPLETokens::Print);
  expect(SIMPLETokens::Identifier);
  expect(SIMPLETokens::Semicolon);
}

void SIMPLEParser::parseCall() {
  expect(SIMPLETokens::Call);
  expect(SIMPLETokens::Identifier);
  expect(SIMPLETokens::Semicolon);
}

void SIMPLEParser::parseWhile() {
  expect(SIMPLETokens::While);
  expect(SIMPLETokens::LeftBrace);
  parseExpr(tokens);
  expect(SIMPLETokens::RightBrace);
  expect(SIMPLETokens::LeftParentheses);
  parseStmtLst();
  expect(SIMPLETokens::RightParentheses);
}

void SIMPLEParser::parseIf() {
  expect(SIMPLETokens::If);
  expect(SIMPLETokens::LeftBrace);
  parseExpr(tokens);
  expect(SIMPLETokens::RightBrace);
  expect(SIMPLETokens::Then);
  expect(SIMPLETokens::LeftParentheses);
  parseStmtLst();
  expect(SIMPLETokens::RightParentheses);
  expect(SIMPLETokens::Else);
  expect(SIMPLETokens::LeftParentheses);
  parseStmtLst();
  expect(SIMPLETokens::RightParentheses);
}

void SIMPLEParser::parseAssign() {
  expect(SIMPLETokens::Identifier);
  expect(SIMPLETokens::Assign);
  parseExpr(tokens);
  expect(SIMPLETokens::Semicolon);
}

SIMPLEParser::SIMPLEParser(std::list<Token>) {}

std::unique_ptr<PKB> SIMPLEParser::getPKB() { return std::move(pkb); }

std::unique_ptr<PKB> parseSIMPLE(std::list<Token> tokens) {
  return SIMPLEParser(tokens).getPKB();
}
} // namespace Parser
