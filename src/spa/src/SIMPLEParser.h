#pragma once
#include "GeneralLexer.h"
#include "PKB.h"
#include <memory>

namespace Parser {
namespace SIMPLETokens {
// Keywords
const static Token Procedure{TokenType::Identifier, "procedure"};
const static Token Read{TokenType::Identifier, "read"};
const static Token Print{TokenType::Identifier, "print"};
const static Token Call{TokenType::Identifier, "call"};
const static Token While{TokenType::Identifier, "while"};
const static Token If{TokenType::Identifier, "if"};
const static Token Then{TokenType::Identifier, "then"};
const static Token Else{TokenType::Identifier, "else"};

// Operators
const static Token Assign{TokenType::Operator, "="};

// Delimiters
const static Token LeftBrace{TokenType::Delimiter, "{"};
const static Token RightBrace{TokenType::Delimiter, "}"};
const static Token LeftParentheses{TokenType::Delimiter, "("};
const static Token RightParentheses{TokenType::Delimiter, ")"};
const static Token Semicolon{TokenType::Delimiter, ";"};

// Identifier
const static Token Identifier{TokenType::Identifier, ""};
}; // namespace SIMPLETokens

class SIMPLEParser {
private:
  std::list<Token> tokens;
  std::unique_ptr<PKB> pkb;
  std::list<std::string> nextStack;
  std::string currentProc;
  int stmtCounter;

  // Parsing
  Token expect(Token token);
  void parseProgram();
  void parseProcedure();
  std::vector<int> parseStmtLst();
  int parseStmt();
  int parseRead();
  int parsePrint();
  int parseCall();
  int parseWhile();
  int parseIf();
  int parseAssign();
  std::list<Token> parseWhileCondExpr();
  std::list<Token> parseIfCondExpr();
  std::list<Token> parseAssignExpr();

  // Utility
  int getStmtNo();
  void setUsesExpr(int, std::list<Token>);

public:
  SIMPLEParser(std::list<Token>);
  std::unique_ptr<PKB> getPKB();
};

std::unique_ptr<PKB> parseSIMPLE(std::list<Token>);
} // namespace Parser
