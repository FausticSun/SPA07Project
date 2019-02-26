#include "GeneralLexer.h"
#include <cctype>
#include <limits>

Token recognizeIdentifier(std::istream &stream) {
  Token t = {TokenType::Identifier, ""};
  while (std::isalnum(stream.peek())) {
    t.value += stream.get();
  }
  return t;
}

Token recognizeNumber(std::istream &stream) {
  Token t = {TokenType::Number, ""};
  while (std::isdigit(stream.peek())) {
    t.value += stream.get();
  }
  if (std::isalpha(stream.peek())) {
    throw std::logic_error("Unexpected letter while parsing number");
  }
  return t;
}

Token recognizePunctuation(std::istream &stream) {
  Token t;
  switch (stream.peek()) {
  case '"':
  case '{':
  case '}':
  case '(':
  case ')':
  case ';':
  case ',':
  case '.':
  case '#':
    t = {TokenType::Delimiter, ""};
    t.value += stream.get();
    break;
  case '+':
  case '-':
  case '*':
  case '/':
  case '%':
  case '_':
    t = {TokenType::Operator, ""};
    t.value += stream.get();
    break;
  case '>':
  case '<':
  case '=':
  case '!':
    t = {TokenType::Operator, ""};
    t.value += stream.get();
    if (stream.peek() == '=') {
      t.value += stream.get();
    }
    break;
  case '&':
  case '|':
    t = {TokenType::Operator, ""};
    t.value += stream.get();
    if (stream.peek() == t.value[0]) {
      t.value += stream.get();
    }
    break;
  default:
    throw std::logic_error("Unknown symbol encountered: " + stream.peek());
  }
  return t;
}

Token recognizeSpace(std::istream &stream) {
  Token t = {TokenType::Whitespace, " "};
  while (std::isspace(stream.peek())) {
    stream.get();
  }
  return t;
}

std::list<Token> GeneralLexer::tokenize(std::istream &stream) {
  std::list<Token> tokens;
  while (stream.peek() != EOF) {
    // Check for and consume comments
    if (stream.get() == '/' && stream.peek() == '/') {
      stream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      if (!stream.eof()) {
        stream.unget();
      }
    } else {
      stream.unget();
    }
    // Lex everything else
    if (std::isalpha(stream.peek())) {
      tokens.push_back(recognizeIdentifier(stream));
    } else if (std::isdigit(stream.peek())) {
      tokens.push_back(recognizeNumber(stream));
    } else if (std::ispunct(stream.peek())) {
      tokens.push_back(recognizePunctuation(stream));
    } else if (std::isspace(stream.peek())) {
      tokens.push_back(recognizeSpace(stream));
    } else if (stream.peek() == EOF) {
      break;
    } else {
      throw std::logic_error("Unknown character encountered: " + stream.peek());
    }
  }
  return tokens;
}
