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

// select-cl: declaration* 'Select' result-cl clauses
void PQLParser::parseQuery() {
  while (!tokens.empty() && tokens.front().value != "Select") {
    parseDeclaration();
  }
  expect(PQLTokens::Select);
  parseResultCl();
  while (!tokens.empty()) {
    parseClauses();
  }
}

// declaration: design-entity IDENT ( ',' IDENT )* ';'
// design-entity : 'stmt' | 'read' | 'print' | 'call' | 'while' | 'if' |
// 'assign' | 'variable' | 'constant' | 'prog_line' | 'procedure'
// Validates design-entity and repeated declarations of synonyms
void PQLParser::parseDeclaration() {
  auto designEntityToken = expect(PQLTokens::Identifier);
  if (designEntityToken.value == "prog") {
    expect(PQLTokens::Underscore);
    expect(PQLTokens::Line);
  }
  if (PQLTokens::tokenEntityMap.count(designEntityToken) == 0) {
    throw std::logic_error("Invalid declaration");
  }
  auto synonymToken = expect(PQLTokens::Identifier);
  if (declarations.count(synonymToken.value)) {
    throwSemanticError("Synonym already declared");
  }
  declarations.emplace(std::make_pair(
      synonymToken.value, PQLTokens::tokenEntityMap.at(designEntityToken)));
  while (tokens.front() != PQLTokens::Semicolon) {
    expect(PQLTokens::Comma);
    auto synonymToken = expect(PQLTokens::Identifier);
    declarations.emplace(std::make_pair(
        synonymToken.value, PQLTokens::tokenEntityMap.at(designEntityToken)));
  }
  expect(PQLTokens::Semicolon);
}

// clauses: ( suchthat-cl | with-cl | pattern-cl )*
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

// result-cl: tuple | 'BOOLEAN'
// Sets the target in Query if it is 'BOOLEAN'
void PQLParser::parseResultCl() {
  if (tokens.front() == PQLTokens::Boolean) {
    expect(PQLTokens::Boolean);
    query.target.emplace_back(QueryEntity(QueryEntityType::Boolean));
  } else {
    parseTuple();
  }
}

// tuple: elem | '<' elem ( ',' elem )* '>'
// Sets the targets in Query
void PQLParser::parseTuple() {
  if (tokens.front().type == TokenType::Identifier) {
    query.target.emplace_back(parseElem());
  } else {
    expect(PQLTokens::LeftChevron);
    query.target.emplace_back(parseElem());
    while (tokens.front() != PQLTokens::RightChevron) {
      expect(PQLTokens::Comma);
      query.target.emplace_back(parseElem());
    }
    expect(PQLTokens::RightChevron);
  }
}

// with-cl: 'with' attrCond
void PQLParser::parseWithCl() {
  expect(PQLTokens::With);
  parseAttrCond();
}

// attrCond: attrCompare ( 'and' attrCompare )*
void PQLParser::parseAttrCond() {
  parseAttrCompare();
  while (!tokens.empty() && tokens.front() == PQLTokens::And) {
    expect(PQLTokens::And);
    parseAttrCompare();
  }
}

// attrCompare: ref '=' ref
// Validates the refs to be of the same type
// Sets the with clause in the query
void PQLParser::parseAttrCompare() {
  auto attrRef1 = parseRef();
  expect(PQLTokens::Equals);
  auto attrRef2 = parseRef();

  if (getAttRefType(attrRef1) != getAttRefType(attrRef2)) {
    throwSemanticError("Attr refs must be of same type");
  }

  Clause withCl(ClauseType::With, {attrRef1, attrRef2});
  query.clauses.emplace_back(withCl);
}

// attrRef: synonym '.' attrName
// Validates attrName and ensure its valid for synonym
PQLTokens::AttrRefType PQLParser::getAttRefType(QueryEntity attrRef) {
  switch (attrRef.type) {
  case QueryEntityType::Name:
    return PQLTokens::AttrRefType::Name;
  case QueryEntityType::Line:
  case QueryEntityType::Progline:
    return PQLTokens::AttrRefType::Integer;
  case QueryEntityType::Attrref:
    if (!PQLTokens::attrTypeMap.count(std::make_pair(
            attrRef.attrRefSynonymType, getAttrValue(attrRef.name)))) {
      throwSemanticError("Attribute value invalid for specificed synonym");
      break;
    }
    return PQLTokens::attrTypeMap.at(
        std::make_pair(attrRef.attrRefSynonymType, getAttrValue(attrRef.name)));
  default:
    throwSemanticError("Invalid attr");
  }
}

// Helper to get attrName in a attrRef string
std::string PQLParser::getAttrValue(std::string attr) {
  return attr.substr(attr.find(".") + 1);
}

// suchthat-cl: 'such that' relCond
void PQLParser::parseSuchThatCl() {
  expect(PQLTokens::Such);
  expect(PQLTokens::That);
  parseRelCond();
}

// relCond: relRef ( 'and' relRef )*
void PQLParser::parseRelCond() {
  parseRelRef();
  while (!tokens.empty() && tokens.front() == PQLTokens::And) {
    expect(PQLTokens::And);
    parseRelRef();
  }
}

// relRef: relation '(' genRef ',' genRef ')'
// relation: 'Modifies' | 'Uses' | 'Calls' | 'Calls*' | 'Parent' | 'Parent*' |
// 'Follows' | 'Follows*' | 'Next' | 'Next*' | 'Affects' | 'Affects*'
// Validates relation and ensure its valid (i.e. one of the above)
// Also calls validation on the parsed relRef
void PQLParser::parseRelRef() {
  // Alt type determines if the clause is of an "alternate type"
  // For Parent, Follows, Call, Next, Affects, the alt type is transitive
  // closure
  // Irrelevant for Uses and Modifies
  bool altType = false;

  auto relWord = expect(PQLTokens::Identifier);
  // Determine relation and alt type for transitive closure
  if (PQLTokens::transitiveRel.count(relWord) &&
      tokens.front() == PQLTokens::Star) {
    expect(PQLTokens::Star);
    altType = true;
  }
  expect(PQLTokens::LeftParentheses);
  auto ref1 = parseGenRef();
  expect(PQLTokens::Comma);
  auto ref2 = parseGenRef();
  expect(PQLTokens::RightParentheses);

  if (!PQLTokens::relClauseMap.count(std::make_pair(relWord, altType))) {
    throw std::logic_error("Invalid relation specified");
  }

  ClauseType clType =
      PQLTokens::relClauseMap.at(std::make_pair(relWord, altType));
  Clause suchThatCl(clType, {ref1, ref2});
  validateSuchThatCl(suchThatCl);
  query.clauses.emplace_back(suchThatCl);
}

// Validates relRef ensuring the 2 genRefs are of valid type for the given
// relation
void PQLParser::validateSuchThatCl(Clause suchThatCl) {
  auto allowedRefTypes =
      PQLTokens::allowedRefTypesMap.at(suchThatCl.clauseType);
  validateRef(suchThatCl.parameters[0], allowedRefTypes.first);
  validateRef(suchThatCl.parameters[1], allowedRefTypes.second);
  if ((suchThatCl.clauseType == ClauseType::UsesS ||
       suchThatCl.clauseType == ClauseType::ModifiesS) &&
      (suchThatCl.parameters[0].type == QueryEntityType::Underscore)) {
    throwSemanticError("First param of uses/modifies cannot be _");
  }
}

// Validate the given ref against the refType (whether its stmtRef, lineRef or
// either)
void PQLParser::validateRef(QueryEntity ref, PQLTokens::RefType refType) {
  if (!PQLTokens::refTypeSpecificTypesMap.at(refType).count(ref.type)) {
    throwSemanticError("Invalid ref");
  }
}

// pattern-cl: 'pattern' patternCond
void PQLParser::parsePatternCl() {
  expect(PQLTokens::Pattern);
  parsePatternCond();
}

// patternCond: pattern ( 'and' pattern )*
void PQLParser::parsePatternCond() {
  parsePattern();
  while (!tokens.empty() && tokens.front() == PQLTokens::And) {
    expect(PQLTokens::And);
    parsePattern();
  }
}

// pattern: syn ( assign | while | if )
// Uses type of syn to decide what type of pattern to parse
// If type is invalid, parses anyway and throw error later
void PQLParser::parsePattern() {
  auto syn = parseSynonym();
  switch (syn.type) {
  case QueryEntityType::Assign:
    parseAssign(syn);
    break;
  case QueryEntityType::While:
    parseWhile(syn);
    break;
  case QueryEntityType::If:
    parseIf(syn);
    break;
  default:
    parseAssign(syn);
    throwSemanticError("Invalid pattern synonym");
  }
}

// assign: '(' entRef ',' expression-spec ')'
// Sets pattern clause in query
void PQLParser::parseAssign(QueryEntity syn) {
  expect(PQLTokens::LeftParentheses);
  auto entRef = parseEntRef();
  expect(PQLTokens::Comma);
  auto exprRef = parseExprSpec();
  expect(PQLTokens::RightParentheses);

  Clause patternCl(ClauseType::AssignPatt, {syn, entRef, exprRef});
  query.clauses.emplace_back(patternCl);
}

// while: '(' entRef ',' '_' ')'
void PQLParser::parseWhile(QueryEntity syn) {
  expect(PQLTokens::LeftParentheses);
  auto entRef = parseEntRef();
  expect(PQLTokens::Comma);
  expect(PQLTokens::Underscore);
  expect(PQLTokens::RightParentheses);

  Clause patternCl(ClauseType::WhilePatt, {syn, entRef});
  query.clauses.emplace_back(patternCl);
}

// if: '(' entRef ',' '_' ',' '_' ')'
void PQLParser::parseIf(QueryEntity syn) {
  expect(PQLTokens::LeftParentheses);
  auto entRef = parseEntRef();
  expect(PQLTokens::Comma);
  expect(PQLTokens::Underscore);
  expect(PQLTokens::Comma);
  expect(PQLTokens::Underscore);
  expect(PQLTokens::RightParentheses);

  Clause patternCl(ClauseType::IfPatt, {syn, entRef});
  query.clauses.emplace_back(patternCl);
}

// expression-spec: quoted-expr | '_' quoted-expr '_' | '_'
QueryEntity PQLParser::parseExprSpec() {
  QueryEntity ent;
  std::string expr = "";
  std::list<Token> exprTokens;
  if (tokens.front() == PQLTokens::Underscore) {
    expect(PQLTokens::Underscore);
    if (tokens.front() == PQLTokens::Quote) {
      expr = parseQuotedExpr();
      expect(PQLTokens::Underscore);
      expr = "_" + expr + "_";
    } else {
      ent.type = QueryEntityType::Underscore;
      ent.name = "_";
      return ent;
    }
  } else {
    expr = parseQuotedExpr();
  }
  ent.type = QueryEntityType::Expression;
  ent.name = expr;
  return ent;
}

// quoted-expr: '"' expr '"'
// Extracts out expr and delegates parsing to ExprParser
// Validates expr and ensure it does not contain relational operators
std::string PQLParser::parseQuotedExpr() {
  std::list<Token> exprTokens;
  std::string expr = "";
  expect(PQLTokens::Quote);
  if (tokens.front() == PQLTokens::Quote) {
    return expr;
  } else {
    while (tokens.front() != PQLTokens::Quote) {
      exprTokens.emplace_back(tokens.front());
      tokens.pop_front();
    }
    expect(PQLTokens::Quote);
    auto postfix = Parser::parseExpr(exprTokens);
    for (auto token : postfix) {
      if (ExprTokens::isRelationalOp(token)) {
        throw std::logic_error("Non-arithmetic operators found in expression");
      }
    }
    expr = Parser::tokensToString(postfix);
  }
  return expr;
}

// attrRef: synonym '.' attrName
// attrName: 'procName' | 'varName' | 'value' | 'stmt#'
// Validates attrName and ensure its one of the above strings
QueryEntity PQLParser::parseAttrRef() {
  auto ent = parseSynonym();
  expect(PQLTokens::Period);
  auto attrToken = expect(PQLTokens::Identifier);
  if (attrToken.value != "varName" && attrToken.value != "procName" &&
      attrToken.value != "stmt" && attrToken.value != "value") {
    throw std::logic_error("Unexpected attribute");
  }
  ent.name += "." + attrToken.value;
  if (attrToken == PQLTokens::Stmt) {
    expect(PQLTokens::Hash);
    ent.name += "#";
  }
  ent.attrRefSynonymType = ent.type;
  ent.type = QueryEntityType::Attrref;
  getAttRefType(ent);
  return ent;
}

// ref: '"' IDENT '"' | INTEGER | attrRef | synonym
// Validates synonym and ensure its of type prog_line
QueryEntity PQLParser::parseRef() {
  if (tokens.front() == PQLTokens::Quote) {
    return parseName();
  } else if (tokens.front().type == TokenType::Number) {
    return parseLineNo();
  } else if (tokens.size() > 1 &&
             *std::next(tokens.begin()) == PQLTokens::Period) {
    return parseAttrRef();
  } else {
    auto syn = parseSynonym();
    if (syn.type != QueryEntityType::Progline) {
      throwSemanticError("Synonym must be of type prog_line");
    }
    return syn;
  }
}

// genRef: '"' IDENT '"' | INTEGER | '_' | synonym
QueryEntity PQLParser::parseGenRef() {
  if (tokens.front() == PQLTokens::Underscore) {
    return parseUnderscore();
  } else if (tokens.front() == PQLTokens::Quote) {
    return parseName();
  } else if (tokens.front().type == TokenType::Number) {
    return parseLineNo();
  } else {
    return parseSynonym();
  }
}

// entRef: '"' IDENT '"' | '_' | synonym
QueryEntity PQLParser::parseEntRef() {
  if (tokens.front() == PQLTokens::Underscore) {
    return parseUnderscore();
  } else if (tokens.front() == PQLTokens::Quote) {
    return parseName();
  } else {
    return parseSynonym();
  }
}

// entRef: INTEGER | '_' | synonym
QueryEntity PQLParser::parseStmtRef() {
  if (tokens.front() == PQLTokens::Underscore) {
    return parseUnderscore();
  } else if (tokens.front().type == TokenType::Number) {
    return parseLineNo();
  } else {
    return parseSynonym();
  }
}

// lineRef: INTEGER | '_' | synonym
QueryEntity PQLParser::parseLineRef() {
  if (tokens.front() == PQLTokens::Underscore) {
    return parseUnderscore();
  } else if (tokens.front().type == TokenType::Number) {
    return parseLineNo();
  } else {
    return parseSynonym();
  }
}

// elem: synonym | attrRef
QueryEntity PQLParser::parseElem() {
  if (tokens.size() < 2 || !(*std::next(tokens.begin()) == PQLTokens::Period)) {
    return parseSynonym();
  } else {
    return parseAttrRef();
  }
}

// synonym: IDENT
// Validates synonym and ensure its declared
QueryEntity PQLParser::parseSynonym() {
  QueryEntity ent;
  auto synToken = expect(PQLTokens::Identifier);
  if (!(declarations.count(synToken.value))) {
    throwSemanticError(("Synonym " + synToken.value + " not declared").c_str());
    return ent;
  }
  ent.name = synToken.value;
  ent.type = declarations.at(synToken.value);
  return ent;
}

// Parse _ and convert it to an entity
QueryEntity PQLParser::parseUnderscore() {
  expect(PQLTokens::Underscore);
  QueryEntity ent;
  ent.name = "_";
  ent.type = QueryEntityType::Underscore;
  return ent;
}

// Parse INTEGER and convert it to an entity
QueryEntity PQLParser::parseLineNo() {
  auto noToken = expect(PQLTokens::Number);
  QueryEntity ent;
  ent.name = noToken.value;
  ent.type = QueryEntityType::Line;
  return ent;
}

// Parse '"' IDENT '"' and convert it to an entity
QueryEntity PQLParser::parseName() {
  expect(PQLTokens::Quote);
  auto nameToken = expect(PQLTokens::Identifier);
  expect(PQLTokens::Quote);
  QueryEntity ent;
  ent.name = nameToken.value;
  ent.type = QueryEntityType::Name;
  return ent;
}

void PQLParser::throwSemanticError(std::string msg) {
  hasSemanticError = true;
  errorMsg = msg;
}

PQLParser::PQLParser(std::list<Lexer::Token> tokens) : tokens(tokens) {
  parseQuery();
  if (hasSemanticError) {
    throw SemanticError(errorMsg.c_str());
  }
}

Query PQLParser::getQuery() { return query; }

Query parsePQL(std::list<Token> tokens) { return PQLParser(tokens).getQuery(); }
} // namespace Parser
