#pragma once
#include <istream>
#include <list>

enum class TokenType { Identifier, Number, Operator, Delimiter, Whitespace };

struct Token {
  const TokenType type;
  const std::string value;
};

namespace Lexer {
std::list<Token> tokenize(std::istream &stream);
};
