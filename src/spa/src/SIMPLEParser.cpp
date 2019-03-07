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

int SIMPLEParser::getStmtNo() { return ++stmtCounter; }

void SIMPLEParser::parseProgram() {
  do {
    if (tokens.empty()) {
      throw std::logic_error("Expected at least 1 procedure");
    }
    parseProcedure();
  } while (!tokens.empty());
}

void SIMPLEParser::parseProcedure() {
  int firstStmtNo = stmtCounter + 1;
  currentProc = expect(SIMPLETokens::Procedure).value;
  expect(SIMPLETokens::Identifier);
  expect(SIMPLETokens::LeftBrace);
  parseStmtLst();
  expect(SIMPLETokens::RightBrace);
  int lastStmtNo = stmtCounter;
  pkb->setProc(currentProc, firstStmtNo, lastStmtNo);
}

std::vector<int> SIMPLEParser::parseStmtLst() {
  std::vector<int> stmtNoLst;
  do {
    int stmtNo = parseStmt();
    stmtNoLst.push_back(stmtNo);
  } while (tokens.front() != SIMPLETokens::RightBrace);

  if (stmtNoLst.size() > 1) {
    for (auto it = stmtNoLst.begin(); it != std::prev(stmtNoLst.end()); ++it) {
      pkb->setFollows(*it, *(std::next(it)));
    }
  }
}

int SIMPLEParser::parseStmt() {
  if (tokens.front() == SIMPLETokens::Read) {
    return parseRead();
  } else if (tokens.front() == SIMPLETokens::Print) {
    return parsePrint();
  } else if (tokens.front() == SIMPLETokens::Call) {
    return parseCall();
  } else if (tokens.front() == SIMPLETokens::While) {
    return parseWhile();
  } else if (tokens.front() == SIMPLETokens::If) {
    return parseIf();
  } else if (tokens.front() == SIMPLETokens::Identifier) {
    return parseAssign();
  } else {
    throw std::logic_error("Error parsing statement");
  }
}

int SIMPLEParser::parseRead() {
  expect(SIMPLETokens::Read);
  auto var = expect(SIMPLETokens::Identifier).value;
  expect(SIMPLETokens::Semicolon);

  int stmtNo = getStmtNo();
  pkb->setStmtType(stmtNo, StatementType::Read);
  pkb->setModifies(stmtNo, var);
  return stmtNo;
}

int SIMPLEParser::parsePrint() {
  expect(SIMPLETokens::Print);
  auto var = expect(SIMPLETokens::Identifier).value;
  expect(SIMPLETokens::Semicolon);

  int stmtNo = getStmtNo();
  pkb->setStmtType(stmtNo, StatementType::Print);
  pkb->setUses(stmtNo, var);
  return stmtNo;
}

int SIMPLEParser::parseCall() {
  expect(SIMPLETokens::Call);
  auto proc = expect(SIMPLETokens::Identifier).value;
  expect(SIMPLETokens::Semicolon);

  int stmtNo = getStmtNo();
  pkb->setStmtType(stmtNo, StatementType::Call);
  pkb->setCalls(currentProc, proc);
  return stmtNo;
}

int SIMPLEParser::parseWhile() {
  expect(SIMPLETokens::While);
  expect(SIMPLETokens::LeftBrace);
  auto postfix = parseWhileCondExpr();
  expect(SIMPLETokens::RightBrace);
  expect(SIMPLETokens::LeftParentheses);
  auto stmtNoLst = parseStmtLst();
  expect(SIMPLETokens::RightParentheses);

  auto stmtNo = getStmtNo();
  pkb->setStmtType(stmtNo, StatementType::While);
  for (int i : stmtNoLst) {
    pkb->setParent(stmtNo, i);
  }
  pkb->setNext(stmtNo, stmtNoLst.front());
  return stmtNo;
}

int SIMPLEParser::parseIf() {
  expect(SIMPLETokens::If);
  expect(SIMPLETokens::LeftBrace);
  auto postfix = parseIfCondExpr();
  expect(SIMPLETokens::RightBrace);
  expect(SIMPLETokens::Then);
  expect(SIMPLETokens::LeftParentheses);
  auto thenStmtNoLst = parseStmtLst();
  expect(SIMPLETokens::RightParentheses);
  expect(SIMPLETokens::Else);
  expect(SIMPLETokens::LeftParentheses);
  auto elseStmtNoLst = parseStmtLst();
  expect(SIMPLETokens::RightParentheses);

  auto stmtNo = getStmtNo();
  pkb->setStmtType(stmtNo, StatementType::While);
  for (int i : thenStmtNoLst) {
    pkb->setParent(stmtNo, i);
  }
  for (int i : elseStmtNoLst) {
    pkb->setParent(stmtNo, i);
  }
  pkb->setNext(stmtNo, thenStmtNoLst.front());
  pkb->setNext(stmtNo, elseStmtNoLst.front());
  return stmtNo;
}

int SIMPLEParser::parseAssign() {
  auto var = expect(SIMPLETokens::Identifier).value;
  expect(SIMPLETokens::Assign);
  auto postfix = parseAssignExpr();
  expect(SIMPLETokens::Semicolon);

  int stmtNo = getStmtNo();
  pkb->setStmtType(stmtNo, StatementType::Assign);
  pkb->setModifies(stmtNo, var);
  setUsesExpr(stmtNo, postfix);
  pkb->setAssigns(stmtNo, var, Parser::tokensToString(postfix));
  return stmtNo;
}

std::list<Token> SIMPLEParser::parseWhileCondExpr() {
  std::list<Token> exprTokens;
  while (*(tokens.begin()) != SIMPLETokens::RightParentheses &&
         *(std::next(tokens.begin())) != SIMPLETokens::LeftBrace) {
    exprTokens.push_back(tokens.front());
    tokens.pop_front();
  }
  if (tokens.empty()) {
    throw std::logic_error("Condition expression cannot be empty");
  }
  return parseExpr(tokens);
}

std::list<Token> SIMPLEParser::parseIfCondExpr() {
  std::list<Token> exprTokens;
  while (*(tokens.begin()) != SIMPLETokens::RightParentheses &&
         *(std::next(tokens.begin())) != SIMPLETokens::Then) {
    exprTokens.push_back(tokens.front());
    tokens.pop_front();
  }
  if (tokens.empty()) {
    throw std::logic_error("Condition expression cannot be empty");
  }
  return parseExpr(tokens);
}

std::list<Token> SIMPLEParser::parseAssignExpr() {
  std::list<Token> exprTokens;
  while (*(tokens.begin()) != SIMPLETokens::Semicolon) {
    exprTokens.push_back(tokens.front());
    tokens.pop_front();
  }
  if (tokens.empty()) {
    throw std::logic_error("Expression cannot be empty");
  }
  auto postfix = parseExpr(tokens);
  for (auto token : postfix) {
    if (token.type == TokenType::Operator && !(isArithmeticOperator(token))) {
      throw std::logic_error(
          "Non-arithmetic operators found in assignment expression")
    }
  }
  return postfix;
}

void SIMPLEParser::setUsesExpr(int stmtNo, std::list<Token> postfix) {
  for (auto token : postfix) {
    switch (token.type) {
    case TokenType::Number:
      pkb->setConst(token.value);
      break;
    case TokenType::Identifier:
      pkb->setUses(stmtNo, token.value);
      break;
    default:
      break;
    }
  }
}

SIMPLEParser::SIMPLEParser(std::list<Token> tkn)
    : pkb(new PKB()), tokens(tkn), stmtCounter(0) {
  parseProgram();
}

std::unique_ptr<PKB> SIMPLEParser::getPKB() { return std::move(pkb); }

std::unique_ptr<PKB> parseSIMPLE(std::list<Token> tokens) {
  return SIMPLEParser(tokens).getPKB();
}
} // namespace Parser
