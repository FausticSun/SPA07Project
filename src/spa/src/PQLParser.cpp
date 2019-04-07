#include "PQLParser.h"
#include <iterator>

using Lexer::Token;
using Lexer::TokenType;

namespace Parser {
Token PQLParser::expect(Token token) {
  if (tokens.empty()) {
    throw std::logic_error("Expected more tokens but ran out");
  }
  if ((token.type == TokenType::Identifier && token.value.empty() &&
       tokens.front().type == TokenType::Identifier) ||
      tokens.front() == token) {
    auto t = tokens.front();
    tokens.pop_front();
    return t;
  }
  throw std::logic_error("Expected " + token.value +
                         " but got: " + tokens.front().value);
  return token;
}

void PQLParser::parseQuery() {
  while (tokens.front().value != "Select") {
    parseDeclaration();
  }
  expect(PQLTokens::Select);
  parseResultCl();
  parseClauses();
}

void PQLParser::parseDeclaration() {
  auto designEntityToken = expect(PQLTokens::Identifier);
  if (designEntityToken.value == "prog") {
    expect(PQLTokens::Underscore);
    expect(PQLTokens::Line);
  }
  if (PQLTokens::tokenEntityMap.count(designEntityToken) == 0) {
    throw std::logic_error("Invalid declaration");
  }
  do {
    auto synonymToken = expect(PQLTokens::Identifier);
    declarations.emplace(std::make_pair(
        synonymToken.value, PQLTokens::tokenEntityMap.at(designEntityToken)));
    if (tokens.front().value == ",") {
      expect(PQLTokens::Comma);
    }
  } while (tokens.front() != PQLTokens::Semicolon);
  expect(PQLTokens::Semicolon);
}

void PQLParser::parseResultCL() {
  if (tokens.front() == PQLTokens::Boolean) {
    expect(PQLTokens::Boolean);
    query.selectors.push_back(QueryEntity(QueryEntityType::Boolean));
  } else {
    parseTuple();
  }
}

void PQLParser::parseTuple() {
  if (tokens.front().type == TokenType::Identifier) {
    parseElem();
  } else {
  }
}

void PQLParser::parseElem() {
  if (*std::next(tokens.begin()) == PQLTokens::Period) {
    parseAttrRef();
  } else {
    parseSynonym();
  }
}

PQLParser::PQLParser(std::list<Lexer::Token> tokens) : tokens(tokens) {
  parseQuery();
}

Query PQLParser::getQuery() { return query; }

Query parsePQL(std::list<Token> tokens) { return PQLParser(tokens).getQuery(); }
} // namespace Parser
