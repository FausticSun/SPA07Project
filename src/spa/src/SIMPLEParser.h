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
  std::list<std::string> parentStack;
  std::list<std::string> followStack;
  std::list<std::string> nextStack;
  std::string currentProc;
  int stmtNo;

  Token expect(Token token);
  std::string getStmtNo();
  void parseProgram();
  void parseProcedure();
  void parseStmtLst();
  void parseStmt();
  void parseRead();
  void parsePrint();
  void parseCall();
  void parseWhile();
  void parseIf();
  void parseAssign();

public:
  SIMPLEParser(std::list<Token>);
  std::unique_ptr<PKB> getPKB();
};

std::unique_ptr<PKB> parseSIMPLE(std::list<Token>);
} // namespace Parser
