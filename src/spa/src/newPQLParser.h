#pragma once
#include "GeneralLexer.h"
#include <memory>
#include <vector>

class Query {};

namespace Parser {
namespace PQLTokens {
// Design Entity Keywords
const static Token Stmt{TokenType::Identifier, "stmt"};
const static Token Read{TokenType::Identifier, "read"};
const static Token Print{TokenType::Identifier, "print"};
const static Token Call{TokenType::Identifier, "call"};
const static Token While{TokenType::Identifier, "while"};
const static Token If{TokenType::Identifier, "if"};
const static Token Assign{TokenType::Identifier, "assign"};
const static Token Variable{TokenType::Identifier, "variable"};
const static Token Constant{TokenType::Identifier, "constant"};
const static Token Prog{TokenType::Identifier, "prog"};
const static Token Line{TokenType::Identifier, "line"};
const static Token Procedure{TokenType::Identifier, "procedure"};

// Clauses keywords
const static Token Such{TokenType::Identifier, "such"};
const static Token That{TokenType::Identifier, "that"};
const static Token Pattern{TokenType::Identifier, "pattern"};
const static Token With{TokenType::Identifier, "with"};
const static Token And{TokenType::Identifier, "and"};

// Such that keywords
const static Token Follows{TokenType::Identifier, "Follows"};
const static Token Parent{TokenType::Identifier, "Parent"};
const static Token Uses{TokenType::Identifier, "Uses"};
const static Token Modifies{TokenType::Identifier, "Modifies"};
const static Token Calls{TokenType::Identifier, "Calls"};
const static Token Next{TokenType::Identifier, "Next"};
const static Token Affects{TokenType::Identifier, "Affects"};

// Symbols
const static Token Underscore{TokenType::Delimiter, "_"};
const static Token Pound{TokenType::Delimiter, "#"};
const static Token Comma{TokenType::Delimiter, "."};
const static Token Period{TokenType::Delimiter, "."};
const static Token Equal{TokenType::Operator, "="};
const static Token LeftParentheses{TokenType::Delimiter, "("};
const static Token RightParentheses{TokenType::Delimiter, ")"};
const static Token LeftChevron{TokenType::Operator, "<"};
const static Token RightChevron{TokenType::Operator, ">"};
const static Token Semicolon{TokenType::Delimiter, ";"};

// General
const static Token Identifier{TokenType::Identifier, ""};
const static Token Number{TokenType::Number, ""};
}; // namespace PQLTokens

class PQLParser {
private:
  // Parsing
  Token expect(Token token);
  void parseQuery();

public:
  PQLParser(std::list<Token>);
  Query getQuery();
};

Query parsePQL(std::list<Token>);
} // namespace Parser
