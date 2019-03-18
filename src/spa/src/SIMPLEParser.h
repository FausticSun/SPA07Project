#pragma once
#include "GeneralLexer.h"
#include "PKB.h"
#include <memory>

namespace Parser {
namespace SIMPLETokens {
// Keywords
const static Lexer::Token Procedure{Lexer::TokenType::Identifier, "procedure"};
const static Lexer::Token Read{Lexer::TokenType::Identifier, "read"};
const static Lexer::Token Print{Lexer::TokenType::Identifier, "print"};
const static Lexer::Token Call{Lexer::TokenType::Identifier, "call"};
const static Lexer::Token While{Lexer::TokenType::Identifier, "while"};
const static Lexer::Token If{Lexer::TokenType::Identifier, "if"};
const static Lexer::Token Then{Lexer::TokenType::Identifier, "then"};
const static Lexer::Token Else{Lexer::TokenType::Identifier, "else"};

// Operators
const static Lexer::Token Assign{Lexer::TokenType::Operator, "="};

// Delimiters
const static Lexer::Token LeftBrace{Lexer::TokenType::Delimiter, "{"};
const static Lexer::Token RightBrace{Lexer::TokenType::Delimiter, "}"};
const static Lexer::Token LeftParentheses{Lexer::TokenType::Delimiter, "("};
const static Lexer::Token RightParentheses{Lexer::TokenType::Delimiter, ")"};
const static Lexer::Token Semicolon{Lexer::TokenType::Delimiter, ";"};

// Identifier
const static Lexer::Token Identifier{Lexer::TokenType::Identifier, ""};
}; // namespace SIMPLETokens

class SIMPLEParser {
private:
  // StmtInfo consists the stmtNo and "exit" stmts
  typedef std::vector<int> ExitStmtLst;
  typedef std::pair<int, ExitStmtLst> StmtInfo;
  static const int PROCEDURE = -1;

  std::list<Lexer::Token> tokens;
  std::unique_ptr<PKB> pkb;
  std::list<std::string> nextStack;
  std::string currentProc;
  int prevTraversedStmt;
  int stmtCounter;

  // Parsing
  Lexer::Token expect(Lexer::Token tokens);
  void parseProgram();
  void parseProcedure();
  ExitStmtLst parseStmtLst(int parent);
  StmtInfo parseStmt();
  ExitStmtLst parseRead(int stmtNo);
  ExitStmtLst parsePrint(int stmtNo);
  ExitStmtLst parseCall(int stmtNo);
  ExitStmtLst parseWhile(int stmtNo);
  ExitStmtLst parseIf(int stmtNo);
  ExitStmtLst parseAssign(int stmtNo);
  std::list<Lexer::Token> parseWhileCondExpr();
  std::list<Lexer::Token> parseIfCondExpr();
  std::list<Lexer::Token> parseAssignExpr();

  // Utility
  int getStmtNo();
  void setUsesExpr(int, std::list<Lexer::Token>);

public:
  SIMPLEParser(std::list<Lexer::Token>);
  std::unique_ptr<PKB> getPKB();
};

std::unique_ptr<PKB> parseSIMPLE(std::list<Lexer::Token>);
} // namespace Parser
