#pragma once

#include <Token.h>
#include <queue>
#include <string>
#include <utility>

using namespace std;

class Lexer {

public:
  Lexer();
  ~Lexer();
  queue<Token> tokenizeFile(string filePath);
  vector<Token> tokenize(string);

private:
  vector<string> vectorize(string);
  string convertQueueToString(queue<string>);
  Token pushKeyword(string);
  Token pushConstant(string);
  Token pushIdentifier(string);
  Token pushSeparator(string);
  Token pushOperator(string);
  Token getToken(string);
  bool isIdentifier(string);
  bool isSeparator(string);
  bool isOperator(string);
  bool isKeyword(string);
  bool isConstant(string);
};
