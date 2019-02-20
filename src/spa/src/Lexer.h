#pragma once

#include "Token.h"
#include <queue>
#include <vector>

class Lexer {
public:
  Lexer();
  ~Lexer();
  std::queue<Token> tokenizeFile(std::istream &fileStream);
  std::vector<Token> tokenize(std::string);

private:
  std::vector<std::string> vectorize(std::string);
  std::string convertQueueToString(std::queue<std::string>);
  Token pushKeyword(std::string);
  Token pushConstant(std::string);
  Token pushIdentifier(std::string);
  Token pushSeparator(std::string);
  Token pushOperator(std::string);
  Token getToken(std::string);
  bool isIdentifier(std::string);
  bool isSeparator(std::string);
  bool isOperator(std::string);
  bool isKeyword(std::string);
  bool isConstant(std::string);
};
