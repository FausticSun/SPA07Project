#include "SIMPLEParser.h"
#include "ExprParser.h"

using Lexer::Token;
using Lexer::TokenType;

namespace Parser {
Token SIMPLEParser::expect(Token token) {
  if (tokens.empty()) {
    throw std::logic_error("Expected more tokens but ran out");
  }
  if ((token.type == TokenType::Identifier && token.value.empty() &&
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

// program: procedure+
void SIMPLEParser::parseProgram() {
  do {
    if (tokens.empty()) {
      throw std::logic_error("Expected at least 1 procedure");
    }
    parseProcedure();
  } while (!tokens.empty());
}

// procedure: 'procedure' proc_name '{' stmtLst '}'
// Sets the procedure and the exiting statements
// of said procedure
void SIMPLEParser::parseProcedure() {
  int firstStmtNo = stmtCounter + 1;
  expect(SIMPLETokens::Procedure);
  currentProc = expect(SIMPLETokens::Identifier).value;
  expect(SIMPLETokens::LeftBrace);
  ExitStmtLst procExitStmts = parseStmtLst(PROCEDURE);
  expect(SIMPLETokens::RightBrace);
  int lastStmtNo = stmtCounter + 1;
  pkb->setProc(currentProc, firstStmtNo, lastStmtNo);
  pkb->setProcExitStmt(currentProc, procExitStmts);
}

// stmtLst: stmt+
// Sets the follows, parent and next relations
// Returns the exiting statements of this stmtLst
SIMPLEParser::ExitStmtLst SIMPLEParser::parseStmtLst(int parent) {
  // Parse stmts
  std::vector<StmtInfo> stmtInfoLst;
  do {
    auto stmtInfo = parseStmt();
    stmtInfoLst.emplace_back(stmtInfo);
  } while (tokens.front() != SIMPLETokens::RightBrace);
  // Extract Follows
  if (stmtInfoLst.size() > 1) {
    for (auto it = stmtInfoLst.begin(); it != std::prev(stmtInfoLst.end());
         ++it) {
      pkb->setFollows((*it).first, (*std::next(it)).first);
    }
  }
  // Extract Parent
  if (parent != PROCEDURE) {
    for (auto stmtInfo : stmtInfoLst) {
      pkb->setParent(parent, stmtInfo.first);
    }
  }
  // Extract Next
  if (parent != -1) {
    pkb->setNext(parent, stmtInfoLst.front().first);
  }
  if (stmtInfoLst.size() > 1) {
    for (auto it = stmtInfoLst.begin(); it != std::prev(stmtInfoLst.end());
         ++it) {
      for (int prev : (*it).second) {
        pkb->setNext(prev, (*std::next(it)).first);
      }
    }
  }
  // Return exit stmts
  return stmtInfoLst.back().second;
}

// stmt: read | print | call | while | if | assign
// Returns information about this stmt
SIMPLEParser::StmtInfo SIMPLEParser::parseStmt() {
  int stmtNo = getStmtNo();
  ExitStmtLst exitStmtLst;
  if (tokens.front() == SIMPLETokens::Read) {
    exitStmtLst = parseRead(stmtNo);
  } else if (tokens.front() == SIMPLETokens::Print) {
    exitStmtLst = parsePrint(stmtNo);
  } else if (tokens.front() == SIMPLETokens::Call) {
    exitStmtLst = parseCall(stmtNo);
  } else if (tokens.front() == SIMPLETokens::While) {
    exitStmtLst = parseWhile(stmtNo);
  } else if (tokens.front() == SIMPLETokens::If) {
    exitStmtLst = parseIf(stmtNo);
  } else if (tokens.front().type == TokenType::Identifier) {
    exitStmtLst = parseAssign(stmtNo);
  } else {
    throw std::logic_error("Error parsing statement");
  }
  return StmtInfo(stmtNo, exitStmtLst);
}

// read: 'read' var_name ';'
// Sets the modifies relation for read stmts
SIMPLEParser::ExitStmtLst SIMPLEParser::parseRead(int stmtNo) {
  expect(SIMPLETokens::Read);
  auto var = expect(SIMPLETokens::Identifier).value;
  expect(SIMPLETokens::Semicolon);

  pkb->setStmtType(stmtNo, StatementType::Read);
  pkb->setModifies(currentProc, var);
  pkb->setModifies(stmtNo, var);
  return {stmtNo};
}

// print: 'print' var_name ';'
// Sets the uses relation for print stmts
SIMPLEParser::ExitStmtLst SIMPLEParser::parsePrint(int stmtNo) {
  expect(SIMPLETokens::Print);
  auto var = expect(SIMPLETokens::Identifier).value;
  expect(SIMPLETokens::Semicolon);

  pkb->setStmtType(stmtNo, StatementType::Print);
  pkb->setUses(currentProc, var);
  pkb->setUses(stmtNo, var);
  return {stmtNo};
}

// call: 'call' proc_name ';'
// Sets the calls relation for call stmts
SIMPLEParser::ExitStmtLst SIMPLEParser::parseCall(int stmtNo) {
  expect(SIMPLETokens::Call);
  auto proc = expect(SIMPLETokens::Identifier).value;
  expect(SIMPLETokens::Semicolon);

  pkb->setStmtType(stmtNo, StatementType::Call);
  pkb->setCalls(currentProc, proc);
  pkb->setCallProcName(stmtNo, proc);
  return {stmtNo};
}

// while: 'while' '(' cond_expr ')' '{' stmtLst '}'
// Sets uses (for variables in the cond_expr),
// next relation and while pattern
SIMPLEParser::ExitStmtLst SIMPLEParser::parseWhile(int stmtNo) {
  expect(SIMPLETokens::While);
  expect(SIMPLETokens::LeftParentheses);
  auto postfix = parseWhileCondExpr();
  expect(SIMPLETokens::RightParentheses);
  expect(SIMPLETokens::LeftBrace);
  auto exitStmtLst = parseStmtLst(stmtNo);
  expect(SIMPLETokens::RightBrace);

  pkb->setStmtType(stmtNo, StatementType::While);
  // Extract uses
  setUsesExpr(stmtNo, postfix);
  // Extract while pattern
  for (auto t : postfix) {
    if (t.type == TokenType::Identifier) {
      pkb->setWhile(stmtNo, t.value);
    }
  }
  // Extract next
  // Connect exit stmts to this while stmt
  for (int i : exitStmtLst) {
    pkb->setNext(i, stmtNo);
  }
  return {stmtNo};
}

// if: 'if' '(' cond_expr ')' 'then' '{' stmtLst '}' 'else' '{' stmtLst '}'
// Sets uses (for variables in the cond_expr),
// next relation and if pattern
SIMPLEParser::ExitStmtLst SIMPLEParser::parseIf(int stmtNo) {
  expect(SIMPLETokens::If);
  expect(SIMPLETokens::LeftParentheses);
  auto postfix = parseIfCondExpr();
  expect(SIMPLETokens::RightParentheses);
  expect(SIMPLETokens::Then);
  expect(SIMPLETokens::LeftBrace);
  auto thenExitStmtLst = parseStmtLst(stmtNo);
  expect(SIMPLETokens::RightBrace);
  expect(SIMPLETokens::Else);
  expect(SIMPLETokens::LeftBrace);
  auto elseExitStmtLst = parseStmtLst(stmtNo);
  expect(SIMPLETokens::RightBrace);

  pkb->setStmtType(stmtNo, StatementType::If);
  // Extract uses
  setUsesExpr(stmtNo, postfix);
  // Extract if pattern
  for (auto t : postfix) {
    if (t.type == TokenType::Identifier) {
      pkb->setIf(stmtNo, t.value);
    }
  }

  ExitStmtLst exitStmtLst(thenExitStmtLst);
  exitStmtLst.insert(exitStmtLst.end(), elseExitStmtLst.begin(),
                     elseExitStmtLst.end());
  return exitStmtLst;
}

// assign: var_name '=' expr ';'
// Set uses, modifies relation and assign pattern
SIMPLEParser::ExitStmtLst SIMPLEParser::parseAssign(int stmtNo) {
  auto var = expect(SIMPLETokens::Identifier).value;
  expect(SIMPLETokens::Assign);
  auto postfix = parseAssignExpr();
  expect(SIMPLETokens::Semicolon);

  pkb->setStmtType(stmtNo, StatementType::Assign);
  pkb->setModifies(currentProc, var);
  pkb->setModifies(stmtNo, var);
  setUsesExpr(stmtNo, postfix);
  auto postfixString = tokensToString(postfix);
  pkb->setAssign(stmtNo, var, postfixString);
  return {stmtNo};
}

// Extract out the tokens in while cond_expr
// and delegate parsing of cond_expr to ExprParser
std::list<Token> SIMPLEParser::parseWhileCondExpr() {
  std::list<Token> exprTokens;
  while (!(*(tokens.begin()) == SIMPLETokens::RightParentheses &&
           *(std::next(tokens.begin())) == SIMPLETokens::LeftBrace)) {
    exprTokens.emplace_back(tokens.front());
    tokens.pop_front();
  }
  if (tokens.empty()) {
    throw std::logic_error("Condition expression cannot be empty");
  }
  return parseExpr(exprTokens);
}

// Extract out the tokens in if cond_expr
// and delegate parsing of cond_expr to ExprParser
std::list<Token> SIMPLEParser::parseIfCondExpr() {
  std::list<Token> exprTokens;
  while (!(*(tokens.begin()) == SIMPLETokens::RightParentheses &&
           *(std::next(tokens.begin())) == SIMPLETokens::Then)) {
    exprTokens.emplace_back(tokens.front());
    tokens.pop_front();
  }
  if (tokens.empty()) {
    throw std::logic_error("Condition expression cannot be empty");
  }
  return parseExpr(exprTokens);
}

// Extract out the tokens in assign expr and delegate parsing of expr to
// ExprParser Also validates expression to make sure it does not contain
// relational tokens
std::list<Token> SIMPLEParser::parseAssignExpr() {
  std::list<Token> exprTokens;
  while (*(tokens.begin()) != SIMPLETokens::Semicolon) {
    exprTokens.emplace_back(tokens.front());
    tokens.pop_front();
  }
  if (tokens.empty()) {
    throw std::logic_error("Expression cannot be empty");
  }
  auto postfix = parseExpr(exprTokens);
  for (auto token : postfix) {
    if (ExprTokens::isRelationalOp(token)) {
      throw std::logic_error(
          "Non-arithmetic operators found in assignment expression");
    }
  }
  return postfix;
}

// Helper method to extract out the variables in a postfix expr
// and set uses for specified stmt
void SIMPLEParser::setUsesExpr(int stmtNo, std::list<Token> postfix) {
  for (auto token : postfix) {
    switch (token.type) {
    case TokenType::Number:
      pkb->setConst(std::stoi(token.value));
      break;
    case TokenType::Identifier:
      pkb->setUses(currentProc, token.value);
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
