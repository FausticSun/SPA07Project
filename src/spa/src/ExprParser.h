#pragma once
#include "GeneralLexer.h"

namespace Parser {
namespace ExprTokens {
// Keywords
const static Token Procedure{TokenType::Identifier, "procedure"};
const static Token Read{TokenType::Identifier, "read"};
const static Token Print{TokenType::Identifier, "print"};
const static Token Call{TokenType::Identifier, "call"};
const static Token While{TokenType::Identifier, "while"};
const static Token If{TokenType::Identifier, "if"};

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
} // namespace ExprTokens

std::list<Token> parseExpr(std::list<Token> &);
std::string tokensToString(std::list<Token> &);
} // namespace Parser
