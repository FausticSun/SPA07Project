#include "GeneralLexer.h"
#include <cctype>
#include <limits>

namespace {
Token recognizeIdentifier(std::istream &stream) {
  std::string value;
  while (std::isalnum(stream.peek())) {
    value += char(stream.get());
  }
  return {TokenType::Identifier, value};
}

Token recognizeNumber(std::istream &stream) {
  std::string value;
  while (std::isdigit(stream.peek())) {
    value += char(stream.get());
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
    value += char(stream.get());
    break;
  case '+':
  case '-':
  case '*':
  case '/':
  case '%':
  case '_':
    type = TokenType::Operator;
    value += char(stream.get());
    break;
  case '>':
  case '<':
  case '=':
  case '!':
    type = TokenType::Operator;
    value += char(stream.get());
    if (stream.peek() == '=') {
      value += char(stream.get());
    }
    break;
  case '&':
  case '|':
    type = TokenType::Operator;
    value += char(stream.get());
    if (stream.peek() == value[0]) {
      value += char(stream.get());
    } else {
      throw std::logic_error(
          "Error parsing relational operators, encountered: " +
          char(stream.peek()));
    }
    break;
  default:
    throw std::logic_error("Unknown symbol encountered: " +
                           char(stream.peek()));
  }
  return {type, value};
}

Token recognizeSpace(std::istream &stream) {
  Token t = {TokenType::Whitespace, " "};
  while (std::isspace(stream.peek())) {
    char(stream.get());
  }
  return t;
}
} // namespace

namespace Lexer {
std::list<Token> tokenize(std::istream &stream) {
  std::list<Token> tokens;
  while (stream.peek() != EOF) {
    // Check for and consume comments
    if (char(stream.get()) == '\\') {
      if (stream.peek() == '\\') {
        stream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (!stream.eof()) {
          stream.unget();
        }
      } else {
        throw std::logic_error("Expected \\ after \\, encountered: " +
                               char(stream.peek()));
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
      throw std::logic_error("Unknown character encountered: " +
                             char(stream.peek()));
    }
  }
  return tokens;
}
} // namespace Lexer
