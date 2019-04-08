#pragma once
#include "ExprParser.h"
#include "GeneralLexer.h"
#include "Query.h"
#include <map>
#include <memory>

namespace Parser {
enum class RefType { EntRef, StmtRef, LineRef };

namespace PQLTokens {
// Design Entities
const static Lexer::Token Stmt{Lexer::TokenType::Identifier, "stmt"};
const static Lexer::Token Read{Lexer::TokenType::Identifier, "read"};
const static Lexer::Token Print{Lexer::TokenType::Identifier, "print"};
const static Lexer::Token Call{Lexer::TokenType::Identifier, "call"};
const static Lexer::Token While{Lexer::TokenType::Identifier, "while"};
const static Lexer::Token If{Lexer::TokenType::Identifier, "if"};
const static Lexer::Token Assign{Lexer::TokenType::Identifier, "assign"};
const static Lexer::Token Variable{Lexer::TokenType::Identifier, "variable"};
const static Lexer::Token Constant{Lexer::TokenType::Identifier, "constant"};
const static Lexer::Token Prog{Lexer::TokenType::Identifier, "prog"};
const static Lexer::Token Line{Lexer::TokenType::Identifier, "line"};
const static Lexer::Token Procedure{Lexer::TokenType::Identifier, "procedure"};

// Selectors
const static Lexer::Token Select{Lexer::TokenType::Identifier, "Select"};
const static Lexer::Token Such{Lexer::TokenType::Identifier, "such"};
const static Lexer::Token That{Lexer::TokenType::Identifier, "that"};
const static Lexer::Token Pattern{Lexer::TokenType::Identifier, "pattern"};
const static Lexer::Token With{Lexer::TokenType::Identifier, "with"};

// Relations
const static Lexer::Token Modifies{Lexer::TokenType::Identifier, "Modifies"};
const static Lexer::Token Uses{Lexer::TokenType::Identifier, "Uses"};
const static Lexer::Token Calls{Lexer::TokenType::Identifier, "Calls"};
const static Lexer::Token Parent{Lexer::TokenType::Identifier, "Parent"};
const static Lexer::Token Follows{Lexer::TokenType::Identifier, "Follows"};
const static Lexer::Token Next{Lexer::TokenType::Identifier, "Next"};
const static Lexer::Token Affects{Lexer::TokenType::Identifier, "Affects"};
const static Lexer::Token NextBip{Lexer::TokenType::Identifier, "NextBip"};
const static Lexer::Token AffectsBip{Lexer::TokenType::Identifier,
                                     "AffectsBip"};

// Other keywords
const static Lexer::Token Boolean{Lexer::TokenType::Identifier, "BOOLEAN"};
const static Lexer::Token And{Lexer::TokenType::Identifier, "and"};

// Symbols
const static Lexer::Token Underscore{Lexer::TokenType::Delimiter, "_"};
const static Lexer::Token LeftParentheses{Lexer::TokenType::Delimiter, "("};
const static Lexer::Token RightParentheses{Lexer::TokenType::Delimiter, ")"};
const static Lexer::Token LeftChevron{Lexer::TokenType::Delimiter, "<"};
const static Lexer::Token RightChevron{Lexer::TokenType::Delimiter, ">"};
const static Lexer::Token Semicolon{Lexer::TokenType::Delimiter, ";"};
const static Lexer::Token Comma{Lexer::TokenType::Delimiter, ","};
const static Lexer::Token Period{Lexer::TokenType::Delimiter, "."};
const static Lexer::Token Equals{Lexer::TokenType::Operator, "="};
const static Lexer::Token Star{Lexer::TokenType::Operator, "*"};
const static Lexer::Token Quote{Lexer::TokenType::Delimiter, "\""};
const static Lexer::Token Hash{Lexer::TokenType::Delimiter, "#"};

// General
const static Lexer::Token Identifier{Lexer::TokenType::Identifier, ""};
const static Lexer::Token Number{Lexer::TokenType::Number, ""};

static std::map<Lexer::Token, QueryEntityType> tokenEntityMap = {
    std::make_pair(PQLTokens::Stmt, QueryEntityType::Stmt),
    std::make_pair(PQLTokens::Read, QueryEntityType::Read),
    std::make_pair(PQLTokens::Print, QueryEntityType::Print),
    std::make_pair(PQLTokens::Call, QueryEntityType::Call),
    std::make_pair(PQLTokens::While, QueryEntityType::While),
    std::make_pair(PQLTokens::If, QueryEntityType::If),
    std::make_pair(PQLTokens::Assign, QueryEntityType::Assign),
    std::make_pair(PQLTokens::Variable, QueryEntityType::Variable),
    std::make_pair(PQLTokens::Constant, QueryEntityType::Constant),
    std::make_pair(PQLTokens::Prog, QueryEntityType::Progline),
    std::make_pair(PQLTokens::Procedure, QueryEntityType::Procedure)};

static std::set<Lexer::Token> transitiveRel = {
    PQLTokens::Calls,     PQLTokens::Parent,  PQLTokens::Follows,
    PQLTokens::Next,      PQLTokens::Affects, PQLTokens::NextBip,
    PQLTokens::AffectsBip};

static std::map<std::pair<Lexer::Token, bool>, ClauseType> relClauseMap = {
    std::make_pair(std::make_pair(PQLTokens::Follows, false),
                   ClauseType::Follows),
    std::make_pair(std::make_pair(PQLTokens::Follows, true),
                   ClauseType::FollowsT),
    std::make_pair(std::make_pair(PQLTokens::Parent, false),
                   ClauseType::Parent),
    std::make_pair(std::make_pair(PQLTokens::Parent, true),
                   ClauseType::ParentT),
    std::make_pair(std::make_pair(PQLTokens::Calls, false), ClauseType::Calls),
    std::make_pair(std::make_pair(PQLTokens::Calls, true), ClauseType::CallsT),
    std::make_pair(std::make_pair(PQLTokens::Next, false), ClauseType::Next),
    std::make_pair(std::make_pair(PQLTokens::Next, true), ClauseType::NextT),
    std::make_pair(std::make_pair(PQLTokens::Affects, false),
                   ClauseType::Affects),
    std::make_pair(std::make_pair(PQLTokens::Affects, true),
                   ClauseType::AffectsT),
    std::make_pair(std::make_pair(PQLTokens::Uses, false), ClauseType::UsesS),
    std::make_pair(std::make_pair(PQLTokens::Modifies, false),
                   ClauseType::ModifiesS)};

enum class RefType { Ent, Stmt, Both };
static std::map<ClauseType, std::pair<RefType, RefType>> allowedRefTypesMap = {
    std::make_pair(ClauseType::Follows,
                   std::make_pair(RefType::Stmt, RefType::Stmt)),
    std::make_pair(ClauseType::FollowsT,
                   std::make_pair(RefType::Stmt, RefType::Stmt)),
    std::make_pair(ClauseType::Parent,
                   std::make_pair(RefType::Stmt, RefType::Stmt)),
    std::make_pair(ClauseType::ParentT,
                   std::make_pair(RefType::Stmt, RefType::Stmt)),
    std::make_pair(ClauseType::Next,
                   std::make_pair(RefType::Stmt, RefType::Stmt)),
    std::make_pair(ClauseType::NextT,
                   std::make_pair(RefType::Stmt, RefType::Stmt)),
    std::make_pair(ClauseType::Affects,
                   std::make_pair(RefType::Stmt, RefType::Stmt)),
    std::make_pair(ClauseType::AffectsT,
                   std::make_pair(RefType::Stmt, RefType::Stmt)),
    std::make_pair(ClauseType::Calls,
                   std::make_pair(RefType::Ent, RefType::Ent)),
    std::make_pair(ClauseType::CallsT,
                   std::make_pair(RefType::Ent, RefType::Ent)),
    std::make_pair(ClauseType::UsesS,
                   std::make_pair(RefType::Both, RefType::Stmt)),
    std::make_pair(ClauseType::ModifiesS,
                   std::make_pair(RefType::Both, RefType::Stmt)),

};
static std::map<RefType, std::set<QueryEntityType>> refTypeSpecificTypesMap = {
    std::make_pair<RefType, std::set<QueryEntityType>>(
        RefType::Ent, {QueryEntityType::Procedure, QueryEntityType::Variable,
                       QueryEntityType::Name, QueryEntityType::Underscore}),
    std::make_pair<RefType, std::set<QueryEntityType>>(
        RefType::Stmt,
        {QueryEntityType::Stmt, QueryEntityType::Read, QueryEntityType::Print,
         QueryEntityType::Call, QueryEntityType::While, QueryEntityType::If,
         QueryEntityType::Assign, QueryEntityType::Progline,
         QueryEntityType::Line, QueryEntityType::Underscore}),
    std::make_pair<RefType, std::set<QueryEntityType>>(
        RefType::Both,
        {QueryEntityType::Stmt, QueryEntityType::Read, QueryEntityType::Print,
         QueryEntityType::Call, QueryEntityType::While, QueryEntityType::If,
         QueryEntityType::Assign, QueryEntityType::Progline,
         QueryEntityType::Procedure, QueryEntityType::Variable,
         QueryEntityType::Line, QueryEntityType::Name,
         QueryEntityType::Underscore})};
}; // namespace PQLTokens

class PQLParser {
private:
  std::list<Lexer::Token> tokens;
  Query query;
  std::map<std::string, QueryEntityType> declarations;

  // Parsing
  Lexer::Token expect(Lexer::Token token);
  void parseQuery();
  void parseDeclaration();
  void parseClauses();

  // Such that
  void parseResultCl();
  void parseTuple();

  // With
  void parseWithCl();
  void parseAttrCond();
  void parseAttrCompare();

  // Such That
  void parseSuchThatCl();
  void parseRelCond();
  void parseRelRef();
  void validateSuchThatCl(Clause);
  void validateRef(QueryEntity, PQLTokens::RefType);

  // Pattern
  void parsePatternCl();
  void parsePatternCond();
  void parsePattern();
  void parseAssign();
  void parseWhile();
  void parseIf();
  QueryEntity parseExprSpec();
  std::string parseQuotedExpr();

  // General
  QueryEntity parseAttrRef();
  QueryEntity parseRef();
  QueryEntity parseGenRef();
  QueryEntity parseEntRef();
  QueryEntity parseStmtRef();
  QueryEntity parseLineRef();
  QueryEntity parseElem();
  QueryEntity parseSynonym();
  QueryEntity parseUnderscore();
  QueryEntity parseLineNo();
  QueryEntity parseName();

public:
  PQLParser(std::list<Lexer::Token>);
  Query getQuery();
};

Query parsePQL(std::list<Lexer::Token>);
} // namespace Parser
