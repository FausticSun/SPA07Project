#pragma once

#include <string>

using std::string;

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
  string name;
  int statementNumber;
  Token() : type(TokenType::Assign), name(""), statementNumber(0){};
  Token(const TokenType type, const string &name, const int statementNumber)
      : type(type), name(name), statementNumber(statementNumber) {}
};
