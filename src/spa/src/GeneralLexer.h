#pragma once
#include <istream>
#include <list>

enum class TokenType { Identifier, Number, Operator, Delimiter, Whitespace };

struct Token {
  TokenType type;
  std::string value;
};

class GeneralLexer {
public:
  static std::list<Token> tokenize(std::istream &stream);
};
