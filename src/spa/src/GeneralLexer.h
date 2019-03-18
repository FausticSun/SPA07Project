#pragma once
#include <istream>
#include <list>

namespace Lexer {
enum class TokenType { Identifier, Number, Operator, Delimiter, Whitespace };

struct Token {
  const TokenType type;
  const std::string value;
  friend bool operator<(const Token &lhs, const Token &rhs) {
    return lhs.type != rhs.type ? lhs.type < rhs.type : lhs.value < rhs.value;
  }
  friend bool operator>(const Token &lhs, const Token &rhs) {
    return rhs < lhs;
  }
  friend bool operator<=(const Token &lhs, const Token &rhs) {
    return !(lhs > rhs);
  }
  friend bool operator>=(const Token &lhs, const Token &rhs) {
    return !(lhs < rhs);
  }
  friend bool operator==(const Token &lhs, const Token &rhs) {
    return lhs.type == rhs.type && lhs.value == rhs.value;
  }
  friend bool operator!=(const Token &lhs, const Token &rhs) {
    return !(lhs == rhs);
  }
};
std::list<Token> tokenize(std::istream &stream);
}; // namespace Lexer
