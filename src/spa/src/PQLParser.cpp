#include "PQLParser.h"
#include <iterator>

using Lexer::Token;
using Lexer::TokenType;

namespace Parser {
Lexer::Token PQLParser::expect(Lexer::Token token) {
  if (tokens.empty()) {
    throw std::logic_error("Expected more tokens but ran out");
  }
  if ((token.type == TokenType::Identifier && token.value.empty() &&
       tokens.front().type == TokenType::Identifier) ||
      (token.type == TokenType::Number && token.value.empty() &&
       tokens.front().type == TokenType::Number) ||
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
  if (!tokens.empty()) {
    parseClauses();
  }
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

void PQLParser::parseClauses() {
  if (tokens.front() == PQLTokens::With) {
    parseWithCl();
  } else if (tokens.front() == PQLTokens::Pattern) {
    parsePatternCl();
  } else if (tokens.front() == PQLTokens::Such) {
    parseSuchThatCl();
  } else {
    throw std::logic_error("Unexpected clause: " + tokens.front().value);
  }
}

void PQLParser::parseResultCl() {
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
    expect(PQLTokens::LeftChevron);
    parseElem();
    while (tokens.front() != PQLTokens::RightChevron) {
      expect(PQLTokens::Comma);
      parseElem();
    }
    expect(PQLTokens::RightChevron);
  }
}

void PQLParser::parseWithCl() {
  expect(PQLTokens::With);
  parseAttrCond();
}

void PQLParser::parseAttrCond() {
  parseAttrCompare();
  while (tokens.front() == PQLTokens::And) {
    expect(PQLTokens::And);
    parseAttrCompare();
  }
}

void PQLParser::parseAttrCompare() {
  parseRef();
  expect(PQLTokens::Equals);
  parseRef();
}

void PQLParser::parseAttrRef() {
  expect(PQLTokens::Identifier);
  expect(PQLTokens::Period);
  expect(PQLTokens::Identifier);
}

void PQLParser::parseSuchThatCl() {
  expect(PQLTokens::Such);
  expect(PQLTokens::That);
  parseRelCond();
}

void PQLParser::parseRelCond() {
  parseRelRef();
  while (tokens.front() == PQLTokens::And) {
    expect(PQLTokens::And);
    parseRelRef();
  }
}

void PQLParser::parseRelRef() {
  bool isTransitive = false;
  auto relWord = expect(PQLTokens::Identifier);
  if (PQLTokens::transitiveRel.count(relWord) &&
      tokens.front() == PQLTokens::Star) {
    expect(PQLTokens::Star);
    isTransitive = true;
  }
  expect(PQLTokens::LeftParentheses);
  parseGenRef();
  expect(PQLTokens::Comma);
  parseGenRef();
  expect(PQLTokens::RightParentheses);
}

void PQLParser::parsePatternCl() {
  expect(PQLTokens::Pattern);
  parsePatternCond();
}

void PQLParser::parsePatternCond() {
  parsePattern();
  while (tokens.front() == PQLTokens::And) {
    expect(PQLTokens::And);
    parsePattern();
  }
}

void PQLParser::parsePattern() {
  // TODO: Check syn type and switch to various parse cases
  auto syn = parseSynonym();
  parseAssign();
  parseWhile();
  parseIf();
}

void PQLParser::parseAssign() {
  expect(PQLTokens::LeftParentheses);
  parseEntRef();
  expect(PQLTokens::Comma);
  parseExprSpec();
  expect(PQLTokens::RightParentheses);
}

void PQLParser::parseWhile() {
  expect(PQLTokens::LeftParentheses);
  parseEntRef();
  expect(PQLTokens::Comma);
  expect(PQLTokens::Underscore);
  expect(PQLTokens::RightParentheses);
}

void PQLParser::parseIf() {
  expect(PQLTokens::LeftParentheses);
  parseEntRef();
  expect(PQLTokens::Comma);
  expect(PQLTokens::Underscore);
  expect(PQLTokens::Comma);
  expect(PQLTokens::Underscore);
  expect(PQLTokens::RightParentheses);
}

void PQLParser::parseExprSpec() {
  std::list<Token> exprTokens;
  if (tokens.front() == PQLTokens::Underscore) {
    expect(PQLTokens::Underscore);
    if (tokens.front() == PQLTokens::Quote) {
      parseQuotedExpr;
      expect(PQLTokens::Underscore);
    }
  } else {
    parseQuotedExpr();
  }
}

std::string PQLParser::parseQuotedExpr() {
  std::list<Token> exprTokens;
  std::string expr = "";
  expect(PQLTokens::Quote);
  if (tokens.front() == PQLTokens::Quote) {
    return expr;
  } else {
    while (tokens.front() != PQLTokens::Quote) {
      exprTokens.push_back(tokens.front());
      tokens.pop_front();
    }
    expr = Parser::tokensToString(Parser::parseExpr(exprTokens));
  }
  return expr;
}

void PQLParser::parseRef() {
  if (tokens.front() == PQLTokens::Quote) {
    expect(PQLTokens::Quote);
    expect(PQLTokens::Identifier);
    expect(PQLTokens::Quote);
  } else if (tokens.front().type == TokenType::Number) {
    expect(PQLTokens::Number);
  } else if (*std::next(tokens.begin()) == PQLTokens::Period) {
    parseAttrRef();
  } else {
    parseSynonym();
  }
}

void PQLParser::parseGenRef() {
  if (tokens.front() == PQLTokens::Underscore) {
    expect(PQLTokens::Underscore);
  } else if (tokens.front() == PQLTokens::Quote) {
    expect(PQLTokens::Quote);
    expect(PQLTokens::Identifier);
    expect(PQLTokens::Quote);
  } else if (tokens.front() == PQLTokens::Number) {
    expect(PQLTokens::Number);
  } else {
    parseSynonym();
  }
}

void PQLParser::parseEntRef() {
  if (tokens.front() == PQLTokens::Underscore) {
    expect(PQLTokens::Underscore);
  } else if (tokens.front() == PQLTokens::Quote) {
    expect(PQLTokens::Quote);
    expect(PQLTokens::Identifier);
    expect(PQLTokens::Quote);
  } else {
    parseSynonym();
  }
}

void PQLParser::parseStmtRef() {
  if (tokens.front() == PQLTokens::Underscore) {
    expect(PQLTokens::Underscore);
  } else if (tokens.front() == PQLTokens::Number) {
    expect(PQLTokens::Number);
  } else {
    parseSynonym();
  }
}

void PQLParser::parseLineRef() {
  if (tokens.front() == PQLTokens::Underscore) {
    expect(PQLTokens::Underscore);
  } else if (tokens.front() == PQLTokens::Number) {
    expect(PQLTokens::Number);
  } else {
    parseSynonym();
  }
}

void PQLParser::parseElem() {
  if (*std::next(tokens.begin()) == PQLTokens::Period) {
    parseAttrRef();
  } else {
    parseSynonym();
  }
}

void PQLParser::parseSynonym() { expect(PQLTokens::Identifier); }

PQLParser::PQLParser(std::list<Lexer::Token> tokens) : tokens(tokens) {
  parseQuery();
}

Query PQLParser::getQuery() { return query; }

Query parsePQL(std::list<Token> tokens) { return PQLParser(tokens).getQuery(); }
} // namespace Parser
