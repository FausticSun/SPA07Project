#pragma once

#include <string>

enum class TokenType {
  Assign,
  If,
  Then,
  Else,
  Print,
  Procedure,
  Read,
  While,
  Constant,
  Identifier,
  Separator,
  OpenParenthesis,
  CloseParenthesis,
  Plus,
  Minus,
  Multiply,
  Divide,
  Mod,
  Greater,
  GreaterThanOrEqual,
  Lesser,
  LesserThanOrEqual,
  Equal,
  NotEqual,
  ExclamationMark,
  And,
  Or
};

struct Token {
  TokenType type;
  std::string name;
  Token() : type(TokenType::Assign), name(""){};
  Token(const TokenType type, const std::string &name)
      : type(type), name(name) {}
};
