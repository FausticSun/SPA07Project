#include "GeneralLexer.h"
#include <cctype>
#include <limits>

Token recognizeIdentifier(std::istream &stream) {
  std::string value;
  while (std::isalnum(stream.peek())) {
    value += stream.get();
  }
  return {TokenType::Identifier, value};
}

Token recognizeNumber(std::istream &stream) {
  std::string value;
  while (std::isdigit(stream.peek())) {
    value += stream.get();
  }
  if (std::isalpha(stream.peek())) {
    throw std::logic_error("Unexpected letter while parsing number");
  }
  return {TokenType::Number, value};
}

Token recognizePunctuation(std::istream &stream) {
  TokenType type;
  std::string value;
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
    type = TokenType::Delimiter;
    value += stream.get();
    break;
  case '+':
  case '-':
  case '*':
  case '/':
  case '%':
  case '_':
    type = TokenType::Operator;
    value += stream.get();
    break;
  case '>':
  case '<':
  case '=':
  case '!':
    type = TokenType::Operator;
    value += stream.get();
    if (stream.peek() == '=') {
      value += stream.get();
    }
    break;
  case '&':
  case '|':
    type = TokenType::Operator;
    value += stream.get();
    if (stream.peek() == value[0]) {
      value += stream.get();
    } else {
      std::logic_error("Error parsing relational operators, encountered: " +
                       stream.peek());
    }
    break;
  default:
    throw std::logic_error("Unknown symbol encountered: " + stream.peek());
  }
  return {type, value};
}

Token recognizeSpace(std::istream &stream) {
  Token t = {TokenType::Whitespace, " "};
  while (std::isspace(stream.peek())) {
    stream.get();
  }
  return t;
}

std::list<Token> Lexer::tokenize(std::istream &stream) {
  std::list<Token> tokens;
  while (stream.peek() != EOF) {
    // Check for and consume comments
    if (stream.get() == '\\') {
      if (stream.peek() == '\\') {
        stream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (!stream.eof()) {
          stream.unget();
        }
      } else {
        throw std::logic_error("Expected \\ after \\, encountered: " +
                               stream.peek());
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
      recognizeSpace(stream);
    } else if (stream.peek() == EOF) {
      break;
    } else {
      throw std::logic_error("Unknown character encountered: " + stream.peek());
    }
  }
  return tokens;
}
