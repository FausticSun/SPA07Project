#pragma once
#include "GeneralLexer.h"

namespace Parser {
namespace ExprTokens {
// Arithmetic Operators
const static Token Plus{TokenType::Operator, "+"};
const static Token Minus{TokenType::Operator, "-"};
const static Token Multiply{TokenType::Operator, "*"};
const static Token Divide{TokenType::Operator, "/"};
const static Token Modulo{TokenType::Operator, "%"};
const static std::set<Token> ArithmeticOperators({Plus, Minus, Multiply, Divide,
                                                  Modulo});
// Relational Operators
const static Token GreaterThan{TokenType::Operator, ">"};
const static Token GreaterThanEqual{TokenType::Operator, ">="};
const static Token LessThan{TokenType::Operator, "<"};
const static Token LessThanEqual{TokenType::Operator, "<="};
const static Token Equal{TokenType::Operator, "=="};
const static Token NotEqual{TokenType::Operator, "!="};
const static Token Not{TokenType::Operator, "!"};
const static Token And{TokenType::Operator, "&&"};
const static Token Or{TokenType::Operator, "||"};
const static std::set<Token>
    RelationalOperators({GreaterThan, GreaterThanEqual, LessThan, LessThanEqual,
                         Equal, NotEqual, Not, And, Or});

// Delimiters
const static Token LeftParentheses{TokenType::Delimiter, "("};
const static Token RightParentheses{TokenType::Delimiter, ")"};

// Identifier
const static Token Identifier{TokenType::Identifier, ""};

// Number
const static Token Identifier{TokenType::Number, ""};

} // namespace ExprTokens

std::list<Token> parseExpr(std::list<Token> &);
bool isArithmeticOp(Token);
bool isRelationalOp(Token);
std::string tokensToString(const std::list<Token> &);
} // namespace Parser
