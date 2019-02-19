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
  vector<Token> tokenizeProcedure(vector<string>);
  vector<Token> tokenizeAssignment(vector<string>);
  vector<Token> tokenizeRead(vector<string>);
  vector<Token> tokenizePrint(vector<string>);
  Token pushIdentifier(string);
  Token pushSeparator(string);
  Token pushOperator(string);
  Token getToken(string);
  bool isIdentifier(string);
  bool isSeparator(string);
  bool isOperator(string);
  bool isConstant(string);
  bool onlyContainDigits(string);
};
