#include "PQLParser.h"
#include <algorithm>
#include <queue>
#include <regex>
#include <stack>
#include <string.h>
#include <string>
#include <vector>

using namespace std;
using namespace PQLLexerToken;

PQLParser::PQLParser() {}

std::queue<QueryToken> PQLParser::parse(std::string query) {
  PQLLexer pqlLexer = PQLLexer(query);
  queue<pair<TokenType, string>> allTokens = pqlLexer.getTokenQueue();
  if (allTokens.size() == 0) {
    throw std::logic_error("Invalid query!");
  }
  std::queue<QueryToken> tokens;
  while (!allTokens.empty()) {
    pair<TokenType, string> pair = allTokens.front();
    tokens.push(QueryToken(pair.first, pair.second));
    allTokens.pop();
  }
  return tokens;
}

Query PQLParser::getQuery() { return query; }

Query PQLParser::buildQuery(std::queue<QueryToken> &tokenQueue) {
  this->tokenQueue = tokenQueue;
  bool hasSelect = false;
  while (!this->tokenQueue.empty()) {
    getNextToken();
    if (token.name == "Select") {
      setQueryTarget();
      tokenizeSelect();
      hasSelect = true;
      break;
    }
    expectTokenIn(this->expectedEntityTokens);
    if (token.name == "variable") {
      insertQueryEntityVariable();
    } else if (token.name == "procedure") {
      insertQueryEntityProcedure();
    } else if (token.name == "read") {
      insertQueryEntityRead();
    } else if (token.name == "print") {
      insertQueryEntityPrint();
    } else if (token.name == "stmt") {
      insertQueryEntityStmt();
    } else if (token.name == "call") {
      insertQueryEntityCall();
    } else if (token.name == "assign") {
      insertQueryEntityAssign();
    } else if (token.name == "constant") {
      insertQueryEntityConstant();
    } else if (token.name == "if") {
      insertQueryEntityIf();
    } else if (token.name == "while") {
      insertQueryEntityWhile();
    } else {
      insertQueryEntityProgline();
    }
  }
  if (!hasSelect) {
    throw std::logic_error("Must has 'Select' keyword.");
  }
  return constructQuery();
}

void PQLParser::getNextToken() {
  if (tokenQueue.empty()) {
    throw std::logic_error("Token Queue is empty!");
  }
  QueryToken nextToken = tokenQueue.front();
  tokenQueue.pop();
  token = nextToken;
}

void PQLParser::expectToken(std::string expectedToken) {
  getNextToken();
  if (token.name != expectedToken) {
    throw std::logic_error("Expected '" + expectedToken + "' but got '" +
                           token.name + "'");
  }
}

void PQLParser::expectTokenIn(std::vector<string> expectedTokens) {
  bool isIn = false;
  for (int i = 0; i < expectedTokens.size(); i++) {
    if (token.name == expectedTokens[i]) {
      isIn = true;
      break;
    }
  }
  if (!isIn) {
    throw std::logic_error("Expected query entity type but got '" + token.name +
                           "'");
  }
}

void PQLParser::setQueryTarget() {
  getNextToken();
  if (token.name == "BOOLEAN") {
    this->target.push_back(QueryEntity(QueryEntityType::Boolean, ""));
    return;
  } else if (token.name == "<") {
    insertTarget();
  } else {
    std::map<std::string, QueryEntityType>::iterator it =
        entityMaps.find(token.name);
    if (it != entityMaps.end()) {
      string next = "";
      if (!tokenQueue.empty()) {
        next = tokenQueue.front().name;
      }
      if (next == ".") {
        getNextToken();
        getNextToken();
		checkAttrrefValidity(it->second, token.name);
        this->target.push_back(QueryEntity(QueryEntityType::Attrref,
                                           it->first + "." + token.name,
                                           it->second));
      } else {
        this->target.push_back(QueryEntity(it->second, token.name));
      }
    } else {
      throw std::logic_error("No matched synonym have been declared.");
    }
  }
}

void PQLParser::insertTarget() {
  getNextToken();
  std::map<std::string, QueryEntityType>::iterator it =
      entityMaps.find(token.name);
  if (it != entityMaps.end()) {
    string next = "";
    if (!tokenQueue.empty()) {
      next = tokenQueue.front().name;
    }
    if (next == ".") {
      getNextToken();
      getNextToken();
	  checkAttrrefValidity(it->second, token.name);
      this->target.push_back(QueryEntity(
          QueryEntityType::Attrref, it->first + "." + token.name, it->second));
    } else {
      this->target.push_back(QueryEntity(it->second, token.name));
    }
  } else {
    throw std::logic_error("No matched synonym have been declared.");
  }
  getNextToken();
  if (token.name == ",") {
    insertTarget();
  } else {
    if (token.name != ">") {
      throw std::logic_error("Expected '>' but got '" + token.name + "'");
    }
  }
}

void PQLParser::tokenizeSelect() {
  string previous = "";
  string current = "";
  while (!this->tokenQueue.empty()) {
    getNextToken();
    expectTokenIn(this->expectedIndicatorTokens);
    current = token.name;
    if (token.name == "and") {
      if (previous == "") {
        throw std::logic_error("Cannot have 'and' as the first clause");
      }
      current = previous;
    }
    if (current == "such that") {
      getNextToken();
      expectTokenIn(expectedClauseTokens);
      if (token.name == "Follows") {
        expectToken("(");
        insertClauseFollows();
        expectToken(")");
      }
      if (token.name == "Follows*") {
        expectToken("(");
        insertClauseFollowsT();
        expectToken(")");
      }
      if (token.name == "Parent") {
        expectToken("(");
        insertClauseParent();
        expectToken(")");
      }
      if (token.name == "Parent*") {
        expectToken("(");
        insertClauseParentT();
        expectToken(")");
      }
      if (token.name == "Modifies") {
        expectToken("(");
        insertClauseModifiesS();
        expectToken(")");
      }
      if (token.name == "Uses") {
        expectToken("(");
        insertClauseUseS();
        expectToken(")");
      }
      if (token.name == "Next") {
        expectToken("(");
        insertClauseNext();
        expectToken(")");
      }
      if (token.name == "Next*") {
        expectToken("(");
        insertClauseNextT();
        expectToken(")");
      }
      if (token.name == "Calls") {
        expectToken("(");
        insertClauseCalls();
        expectToken(")");
      }
      if (token.name == "Calls*") {
        expectToken("(");
        insertClauseCallsT();
        expectToken(")");
      }
    } else if (current == "pattern") {
      insertClausePattern();
    } else { // with
      insertClauseWith();
    }
    previous = current;
  }
}

void PQLParser::insertQueryEntityVariable() {
  getNextToken();
  if (isDefined(token.name)) {
    throw std::logic_error("'" + token.name + "'" + " has been declared.");
  }
  QueryEntity qe =
      QueryEntity(QueryEntityType::Variable, checkNameValidity(token.name));
  this->entityMaps.insert(make_pair(token.name, QueryEntityType::Variable));
  this->selectors.push_back(qe);
  getNextToken();
  if (token.name == ",") {
    insertQueryEntityVariable();
  } else {
    if (token.name != ";") {
      throw std::logic_error("Expected ';' but got '" + token.name + "'");
    }
  }
}

void PQLParser::insertQueryEntityProcedure() {
  getNextToken();
  if (isDefined(token.name)) {
    throw std::logic_error("'" + token.name + "'" + " has been declared.");
  }
  QueryEntity qe =
      QueryEntity(QueryEntityType::Procedure, checkNameValidity(token.name));
  this->entityMaps.insert(make_pair(token.name, QueryEntityType::Procedure));
  this->selectors.push_back(qe);
  getNextToken();
  if (token.name == ",") {
    insertQueryEntityProcedure();
  } else {
    if (token.name != ";") {
      throw std::logic_error("Expected ';' but got '" + token.name + "'");
    }
  }
}

void PQLParser::insertQueryEntityRead() {
  getNextToken();
  if (isDefined(token.name)) {
    throw std::logic_error("'" + token.name + "'" + " has been declared.");
  }
  QueryEntity qe =
      QueryEntity(QueryEntityType::Read, checkNameValidity(token.name));
  this->entityMaps.insert(make_pair(token.name, QueryEntityType::Read));
  this->selectors.push_back(qe);
  getNextToken();
  if (token.name == ",") {
    insertQueryEntityRead();
  } else {
    if (token.name != ";") {
      throw std::logic_error("Expected ';' but got '" + token.name + "'");
    }
  }
}

void PQLParser::insertQueryEntityPrint() {
  getNextToken();
  if (isDefined(token.name)) {
    throw std::logic_error("'" + token.name + "'" + " has been declared.");
  }
  QueryEntity qe =
      QueryEntity(QueryEntityType::Print, checkNameValidity(token.name));
  this->entityMaps.insert(make_pair(token.name, QueryEntityType::Print));
  this->selectors.push_back(qe);
  getNextToken();
  if (token.name == ",") {
    insertQueryEntityPrint();
  } else {
    if (token.name != ";") {
      throw std::logic_error("Expected ';' but got '" + token.name + "'");
    }
  }
}

void PQLParser::insertQueryEntityIf() {
  getNextToken();
  if (isDefined(token.name)) {
    throw std::logic_error("'" + token.name + "'" + " has been declared.");
  }
  QueryEntity qe =
      QueryEntity(QueryEntityType::If, checkNameValidity(token.name));
  this->entityMaps.insert(make_pair(token.name, QueryEntityType::If));
  this->selectors.push_back(qe);
  getNextToken();
  if (token.name == ",") {
    insertQueryEntityIf();
  } else {
    if (token.name != ";") {
      throw std::logic_error("Expected ';' but got '" + token.name + "'");
    }
  }
}

void PQLParser::insertQueryEntityWhile() {
  getNextToken();
  if (isDefined(token.name)) {
    throw std::logic_error("'" + token.name + "'" + " has been declared.");
  }
  QueryEntity qe =
      QueryEntity(QueryEntityType::While, checkNameValidity(token.name));
  this->entityMaps.insert(make_pair(token.name, QueryEntityType::While));
  this->selectors.push_back(qe);
  getNextToken();
  if (token.name == ",") {
    insertQueryEntityWhile();
  } else {
    if (token.name != ";") {
      throw std::logic_error("Expected ';' but got '" + token.name + "'");
    }
  }
}

void PQLParser::insertQueryEntityAssign() {
  getNextToken();
  if (isDefined(token.name)) {
    throw std::logic_error("'" + token.name + "'" + " has been declared.");
  }
  QueryEntity qe =
      QueryEntity(QueryEntityType::Assign, checkNameValidity(token.name));
  this->entityMaps.insert(make_pair(token.name, QueryEntityType::Assign));
  this->selectors.push_back(qe);
  getNextToken();
  if (token.name == ",") {
    insertQueryEntityAssign();
  } else {
    if (token.name != ";") {
      throw std::logic_error("Expected ';' but got '" + token.name + "'");
    }
  }
}

void PQLParser::insertQueryEntityCall() {
  getNextToken();
  if (isDefined(token.name)) {
    throw std::logic_error("'" + token.name + "'" + " has been declared.");
  }
  QueryEntity qe =
      QueryEntity(QueryEntityType::Call, checkNameValidity(token.name));
  this->entityMaps.insert(make_pair(token.name, QueryEntityType::Call));
  this->selectors.push_back(qe);
  getNextToken();
  if (token.name == ",") {
    insertQueryEntityCall();
  } else {
    if (token.name != ";") {
      throw std::logic_error("Expected ';' but got '" + token.name + "'");
    }
  }
}

void PQLParser::insertQueryEntityStmt() {
  getNextToken();
  if (isDefined(token.name)) {
    throw std::logic_error("'" + token.name + "'" + " has been declared.");
  }
  QueryEntity qe =
      QueryEntity(QueryEntityType::Stmt, checkNameValidity(token.name));
  this->entityMaps.insert(make_pair(token.name, QueryEntityType::Stmt));
  this->selectors.push_back(qe);
  getNextToken();
  if (token.name == ",") {
    insertQueryEntityStmt();
  } else {
    if (token.name != ";") {
      throw std::logic_error("Expected ';' but got '" + token.name + "'");
    }
  }
}

void PQLParser::insertQueryEntityConstant() {
  getNextToken();
  if (isDefined(token.name)) {
    throw std::logic_error("'" + token.name + "'" + " has been declared.");
  }
  QueryEntity qe =
      QueryEntity(QueryEntityType::Constant, checkNameValidity(token.name));
  this->entityMaps.insert(make_pair(token.name, QueryEntityType::Constant));
  this->selectors.push_back(qe);
  getNextToken();
  if (token.name == ",") {
    insertQueryEntityConstant();
  } else {
    if (token.name != ";") {
      throw std::logic_error("Expected ';' but got '" + token.name + "'");
    }
  }
}

void PQLParser::insertQueryEntityProgline() {
  getNextToken();
  if (isDefined(token.name)) {
    throw std::logic_error("'" + token.name + "'" + " has been declared.");
  }
  QueryEntity qe =
      QueryEntity(QueryEntityType::Progline, checkNameValidity(token.name));
  this->entityMaps.insert(make_pair(token.name, QueryEntityType::Progline));
  this->selectors.push_back(qe);
  getNextToken();
  if (token.name == ",") {
    insertQueryEntityProgline();
  } else {
    if (token.name != ";") {
      throw std::logic_error("Expected ';' but got '" + token.name + "'");
    }
  }
}

bool PQLParser::isDefined(string name) {
  std::map<std::string, QueryEntityType>::iterator it = entityMaps.find(name);
  if (it != entityMaps.end()) {
    return true;
  }
  return false;
}

bool PQLParser::isInt(string s) {
  if (s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+')))
    return false;

  char *p;
  strtol(s.c_str(), &p, 10);

  return (*p == 0);
}

string PQLParser::checkNameValidity(string s) {
  if (regex_match(s, regex("[a-zA-Z][a-zA-Z0-9]*"))) {
    return s;
  } else {
    throw std::invalid_argument("Invalid naming style for names");
  }
}

QueryEntity PQLParser::determineQueryEntity() {
  getNextToken();
  if (isInt(token.name)) {
    return QueryEntity(QueryEntityType::Line, token.name);
  } else if (token.name == "_") {
    return QueryEntity(QueryEntityType::Underscore, token.name);
  } else if (token.name == "\"") {
    getNextToken();
    QueryEntity qe = QueryEntity(QueryEntityType::Name, token.name);
    expectToken("\"");
    return qe;
  } else {
    std::map<std::string, QueryEntityType>::iterator it =
        entityMaps.find(token.name);
    if (it != entityMaps.end()) {
      QueryEntityType qet = it->second;
      if (qet == QueryEntityType::Constant) {
        throw std::invalid_argument(
            "invalid to have constant synonym as the argument for clauses");
      }
      return QueryEntity(qet, token.name);
    }
    throw std::logic_error("No matched synonym have been declared.");
  }
}

void PQLParser::checkFPNValidity(
    QueryEntity firstEntity,
    QueryEntity secondEntity) { // Stmt,Read,Print,Call,While,If,Assign,Line,
                                // Underscore are allowed
  QueryEntityType firstType = firstEntity.type;
  QueryEntityType secondType = secondEntity.type;
  if (std::find(validationTable["FPN12"].begin(),
                validationTable["FPN12"].end(),
                firstType) != validationTable["FPN12"].end() &&
      std::find(validationTable["FPN12"].begin(),
                validationTable["FPN12"].end(),
                secondType) != validationTable["FPN12"].end()) {
    return;
  }
  throw std::invalid_argument("invalid argument type for clauses");
}

void PQLParser::checkModifiesValidity(
    QueryEntity firstEntity,
    QueryEntity secondEntity) { // first: Stmt,Read,Call,While,If,Assign,
                                // Procedure, Line, Name are allowed second:
                                // Variable, Name, Underscore are allowed
  QueryEntityType firstType = firstEntity.type;
  QueryEntityType secondType = secondEntity.type;
  if (std::find(validationTable["M1"].begin(), validationTable["M1"].end(),
                firstType) != validationTable["M1"].end() &&
      std::find(validationTable["UMPAT2"].begin(),
                validationTable["UMPAT2"].end(),
                secondType) != validationTable["UMPAT2"].end()) {
    return;
  }
  throw std::invalid_argument("invalid argument type for clauses");
}

void PQLParser::checkUsesValidity(
    QueryEntity firstEntity,
    QueryEntity secondEntity) { // first: Stmt,Print,Call,While,If,Assign,
                                // Procedure, Line, Name are allowed second:
                                // Variable, Name, Underscore are allowed
  QueryEntityType firstType = firstEntity.type;
  QueryEntityType secondType = secondEntity.type;
  if (std::find(validationTable["U1"].begin(), validationTable["U1"].end(),
                firstType) != validationTable["U1"].end() &&
      std::find(validationTable["UMPAT2"].begin(),
                validationTable["UMPAT2"].end(),
                secondType) != validationTable["UMPAT2"].end()) {
    return;
  }
  throw std::invalid_argument("invalid argument type for clauses");
}

void PQLParser::checkPatValidity(QueryEntityType qet) {
  if (std::find(validationTable["PAT1"].begin(), validationTable["PAT1"].end(),
                qet) != validationTable["PAT1"].end()) {
    return;
  }
  throw std::invalid_argument("invalid argument type for clauses");
}

void PQLParser::checkWithValidity(
    QueryEntity firstEntity,
    QueryEntity secondEntity) { // Name, Line, Attrref, Progline
  QueryEntityType firstType = firstEntity.type;
  QueryEntityType secondType = secondEntity.type;
  if (std::find(validationTable["W12"].begin(), validationTable["W12"].end(),
                firstType) != validationTable["W12"].end() &&
      std::find(validationTable["W12"].begin(), validationTable["W12"].end(),
                secondType) != validationTable["W12"].end()) {
    return;
  }
  throw std::invalid_argument("invalid argument type for clauses");
}

void PQLParser::checkCallsValidity(
    QueryEntity firstEntity,
    QueryEntity secondEntity) { // Name, Procedure, Underscore
  QueryEntityType firstType = firstEntity.type;
  QueryEntityType secondType = secondEntity.type;
  if (std::find(validationTable["C12"].begin(), validationTable["C12"].end(),
                firstType) != validationTable["C12"].end() &&
      std::find(validationTable["C12"].begin(), validationTable["C12"].end(),
                secondType) != validationTable["C12"].end()) {
    return;
  }
  throw std::invalid_argument("invalid argument type for clauses");
}

void PQLParser::checkAttrrefValidity(
	QueryEntityType type,
	string attrName) {
	if (std::find(validationTable[attrName].begin(), validationTable[attrName].end(),
		type) != validationTable[attrName].end()) {
		return;
	}
	throw std::invalid_argument("invalid argument type for attrref");
}

void PQLParser::insertClauseFollows() {
  QueryEntity firstEntity = determineQueryEntity();
  expectToken(",");
  QueryEntity secondEntity = determineQueryEntity();
  checkFPNValidity(firstEntity, secondEntity);
  Clause c = Clause(ClauseType::Follows,
                    vector<QueryEntity>{firstEntity, secondEntity});
  this->clauses.push_back(c);
}

void PQLParser::insertClauseFollowsT() {
  QueryEntity firstEntity = determineQueryEntity();
  expectToken(",");
  QueryEntity secondEntity = determineQueryEntity();
  checkFPNValidity(firstEntity, secondEntity);
  Clause c = Clause(ClauseType::FollowsT,
                    vector<QueryEntity>{firstEntity, secondEntity});
  this->clauses.push_back(c);
}

void PQLParser::insertClauseParent() {
  QueryEntity firstEntity = determineQueryEntity();
  expectToken(",");
  QueryEntity secondEntity = determineQueryEntity();
  checkFPNValidity(firstEntity, secondEntity);
  Clause c = Clause(ClauseType::Parent,
                    vector<QueryEntity>{firstEntity, secondEntity});
  this->clauses.push_back(c);
}

void PQLParser::insertClauseParentT() {
  QueryEntity firstEntity = determineQueryEntity();
  expectToken(",");
  QueryEntity secondEntity = determineQueryEntity();
  checkFPNValidity(firstEntity, secondEntity);
  Clause c = Clause(ClauseType::ParentT,
                    vector<QueryEntity>{firstEntity, secondEntity});
  this->clauses.push_back(c);
}

void PQLParser::insertClauseNext() {
  QueryEntity firstEntity = determineQueryEntity();
  expectToken(",");
  QueryEntity secondEntity = determineQueryEntity();
  checkFPNValidity(firstEntity, secondEntity);
  Clause c =
      Clause(ClauseType::Next, vector<QueryEntity>{firstEntity, secondEntity});
  this->clauses.push_back(c);
}

void PQLParser::insertClauseNextT() {
  QueryEntity firstEntity = determineQueryEntity();
  expectToken(",");
  QueryEntity secondEntity = determineQueryEntity();
  checkFPNValidity(firstEntity, secondEntity);
  Clause c =
      Clause(ClauseType::NextT, vector<QueryEntity>{firstEntity, secondEntity});
  this->clauses.push_back(c);
}

void PQLParser::insertClauseModifiesS() {
  QueryEntity firstEntity = determineQueryEntity();
  if (firstEntity.type == QueryEntityType::Underscore) {
    throw std::invalid_argument("semantically invalid to have underscore as "
                                "the first argument for Modifies");
  }
  expectToken(",");
  QueryEntity secondEntity = determineQueryEntity();
  checkModifiesValidity(firstEntity, secondEntity);
  Clause c = Clause(ClauseType::ModifiesS,
                    vector<QueryEntity>{firstEntity, secondEntity});
  this->clauses.push_back(c);
}

void PQLParser::insertClauseUseS() {
  QueryEntity firstEntity = determineQueryEntity();
  if (firstEntity.type == QueryEntityType::Underscore) {
    throw std::invalid_argument("semantically invalid to have underscore as "
                                "the first argument for Uses");
  }
  expectToken(",");
  QueryEntity secondEntity = determineQueryEntity();
  checkUsesValidity(firstEntity, secondEntity);
  Clause c =
      Clause(ClauseType::UsesS, vector<QueryEntity>{firstEntity, secondEntity});
  this->clauses.push_back(c);
}

void PQLParser::insertClauseCalls() {
  QueryEntity firstEntity = determineQueryEntity();
  expectToken(",");
  QueryEntity secondEntity = determineQueryEntity();
  checkCallsValidity(firstEntity, secondEntity);
  Clause c =
      Clause(ClauseType::Calls, vector<QueryEntity>{firstEntity, secondEntity});
  this->clauses.push_back(c);
}

void PQLParser::insertClauseCallsT() {
  QueryEntity firstEntity = determineQueryEntity();
  expectToken(",");
  QueryEntity secondEntity = determineQueryEntity();
  checkCallsValidity(firstEntity, secondEntity);
  Clause c = Clause(ClauseType::CallsT,
                    vector<QueryEntity>{firstEntity, secondEntity});
  this->clauses.push_back(c);
}

void PQLParser::insertClausePattern() {
  getNextToken();
  std::map<std::string, QueryEntityType>::iterator it =
      entityMaps.find(token.name);
  if (it != entityMaps.end()) {
    QueryEntityType qet = it->second;
    checkPatValidity(qet);
    if (qet == QueryEntityType::Assign) {
      insertAssignPattern(token.name);
    } else if (qet == QueryEntityType::If) {
      insertIfPattern(token.name);
    } else // while
    {
      insertWhilePattern(token.name);
    }
  } else {
    throw std::logic_error("No matched synonym have been declared.");
  }
}

void PQLParser::insertAssignPattern(string name) {
  QueryEntity firstPara = QueryEntity(QueryEntityType::Assign, name);
  expectToken("(");
  QueryEntity qe = determineQueryEntity();
  if (std::find(validationTable["UMPAT2"].begin(),
                validationTable["UMPAT2"].end(),
                qe.type) != validationTable["UMPAT2"].end()) {
    QueryEntity secondPara = QueryEntity(qe.type, qe.name);
    expectToken(",");
    QueryEntity thirdPara = parseExpression();
    Clause c = Clause(ClauseType::AssignPatt,
                      vector<QueryEntity>{firstPara, secondPara, thirdPara});
    this->clauses.push_back(c);
  } else {
    throw std::invalid_argument(
        "Invalid query entity type for second parameter of assign pattern.");
  }
}

void PQLParser::insertIfPattern(string name) {
  QueryEntity firstPara = QueryEntity(QueryEntityType::If, name);
  expectToken("(");
  QueryEntity qe = determineQueryEntity();
  if (std::find(validationTable["UMPAT2"].begin(),
                validationTable["UMPAT2"].end(),
                qe.type) != validationTable["UMPAT2"].end()) {
    QueryEntity secondPara = QueryEntity(qe.type, qe.name);
    expectToken(",");
    expectToken("_");
    expectToken(",");
    expectToken("_");
    expectToken(")");
    Clause c =
        Clause(ClauseType::IfPatt, vector<QueryEntity>{firstPara, secondPara});
    this->clauses.push_back(c);
  } else {
    throw std::invalid_argument(
        "Invalid query entity type for second parameter of if pattern.");
  }
}

void PQLParser::insertWhilePattern(string name) {
  QueryEntity firstPara = QueryEntity(QueryEntityType::While, name);
  expectToken("(");
  QueryEntity qe = determineQueryEntity();
  if (std::find(validationTable["UMPAT2"].begin(),
                validationTable["UMPAT2"].end(),
                qe.type) != validationTable["UMPAT2"].end()) {
    QueryEntity secondPara = QueryEntity(qe.type, qe.name);
    expectToken(",");
    expectToken("_");
    expectToken(")");
    Clause c = Clause(ClauseType::WhilePatt,
                      vector<QueryEntity>{firstPara, secondPara});
    this->clauses.push_back(c);
  } else {
    throw std::invalid_argument(
        "Invalid query entity type for second parameter of while pattern.");
  }
}

int precedence(char c) {
  if (c == '^')
    return 3;
  else if (c == '*' || c == '/' || c == '%')
    return 2;
  else if (c == '+' || c == '-')
    return 1;
  else
    return -1;
}

bool isOp(char s) {
  return (s == '+' || s == '-' || s == '*' || s == '?' || s == '%');
}

string PQLParser::convertToPostfix(string expr0) {
  /*std::string::iterator end_pos = std::remove(expr.begin(), expr.end(), ' ');
  expr.erase(end_pos, expr.end());*/
  // combine multiple spaces into one a(_,"")
  if (expr0.empty()) {
    throw std::invalid_argument("Invalid expression");
  }
  std::string::iterator new_end =
      std::unique(expr0.begin(), expr0.end(), [=](char lhs, char rhs) {
        return (lhs == rhs) && (lhs == ' ');
      });
  expr0.erase(new_end, expr0.end());
  // remove unnecessary spaces
  string expr;
  for (int i = 0; i < expr0.size(); i++) {
    char c = expr0[i];
    if (c == ' ' && i != 0 && i != expr0.size() - 1 &&
        ((expr0[i - 1] >= 'a' && expr0[i - 1] <= 'z') ||
         (expr0[i - 1] >= 'A' && expr0[i - 1] <= 'Z') ||
         isdigit(expr0[i - 1])) &&
        ((expr0[i + 1] >= 'a' && expr0[i + 1] <= 'z') ||
         (expr0[i + 1] >= 'A' && expr0[i + 1] <= 'Z') ||
         isdigit(expr0[i + 1]))) {
      expr += ' ';
    }
    if (c != ' ') {
      expr += c;
    }
  }
  // check validation
  if (isOp(expr[0]) || isOp(expr[expr.size() - 1])) {
    throw std::invalid_argument("Invalid expression");
  }
  for (int j = 0; j < expr.size() - 1; j++) {
    if (isOp(expr[j]) && isOp(expr[j + 1])) {
      throw std::invalid_argument("Invalid expression");
    }
  }

  std::stack<char> st;
  st.push('N');
  string res;
  for (int i = 0; i < expr.size(); i++) {
    if ((expr[i] >= 'a' && expr[i] <= 'z') ||
        (expr[i] >= 'A' && expr[i] <= 'Z') || isdigit(expr[i]) ||
        expr[i] == ' ') {
      res += expr[i];
      if (i == expr.size() - 1 ||
          (!(expr[i + 1] >= 'a' && expr[i + 1] <= 'z') &&
           !(expr[i + 1] >= 'A' && expr[i + 1] <= 'Z') &&
           !isdigit(expr[i + 1]) && expr[i + 1] != ' ')) {
        res += ' ';
      }
    } else if (expr[i] == '(') {
      st.push('(');
    } else if (expr[i] == ')') {
      while (st.top() != 'N' && st.top() != '(') {
        char c = st.top();
        st.pop();
        res += c;
        res += ' ';
      }
      if (st.top() == '(') {
        char c = st.top();
        st.pop();
      }
    } else {
      while (st.top() != 'N' && precedence(expr[i]) <= precedence(st.top())) {
        char c = st.top();
        st.pop();
        res += c;
        res += ' ';
      }
      st.push(expr[i]);
    }
  }
  while (st.top() != 'N') {
    char c = st.top();
    st.pop();
    res += c;
    res += ' ';
  }
  /*for (int i = 0; i < res.length(); i++)
  {
          res.insert(++i, " ");
  }*/

  std::regex ws_re("\\s+");
  std::vector<std::string> result{
      std::sregex_token_iterator(res.begin(), res.end(), ws_re, -1), {}};
  string out = " ";
  for (vector<string>::const_iterator i = result.begin(); i != result.end();
       ++i) {
    string s = *i;
    if (isInt(s)) {
      s = std::to_string(stoi(s));
    }
    out += s + ' ';
  }
  return out;
}

QueryEntity PQLParser::parseExpression() {
  getNextToken();
  if (token.name == "_" && tokenQueue.front().name == ")") // underscore
  {
    expectToken(")");
    return QueryEntity(QueryEntityType::Underscore, "_");
  } else if (token.name == "_" &&
             tokenQueue.front().name == "\"") // partial match
  {
    getNextToken();
    getNextToken();
    string expression = token.name;
    string postfixExpr = convertToPostfix(expression);
    expectToken("\"");
    expectToken("_");
    expectToken(")");
    return QueryEntity(QueryEntityType::Expression, "_" + postfixExpr + "_");
  } else if (token.name == "\"") // exact match
  {
    getNextToken();
    string expression = token.name;
    string postfixExpr = convertToPostfix(expression);
    expectToken("\"");
    expectToken(")");
    return QueryEntity(QueryEntityType::Expression, postfixExpr);
  } else // invalid token
  {
    throw std::logic_error("Invalid Expression");
  }
}

QueryEntity PQLParser::determineWithClauseEntity() {
  getNextToken();
  if (isInt(token.name)) {
    return QueryEntity(QueryEntityType::Line, token.name);
  } else if (token.name == "\"") {
    getNextToken();
    QueryEntity qe = QueryEntity(QueryEntityType::Name, token.name);
    expectToken("\"");
    return qe;
  } else {
    std::map<std::string, QueryEntityType>::iterator it =
        entityMaps.find(token.name);
    if (it != entityMaps.end()) {
      QueryEntityType qet = it->second;
      string next = "";
      if (!tokenQueue.empty()) {
        next = tokenQueue.front().name;
      }
      if (next == ".") {
        getNextToken();
        getNextToken();
		checkAttrrefValidity(qet, token.name);
        return QueryEntity(QueryEntityType::Attrref,
                           it->first + "." + token.name, qet);
      } else {
        if (qet != QueryEntityType::Progline) {
          throw std::invalid_argument(
              "synonym must be of type 'prog_line' in with clause");
        }
        return QueryEntity(qet, token.name);
      }
    }
    throw std::logic_error("No matched synonym have been declared.");
  }
}

void PQLParser::insertClauseWith() {
  QueryEntity firstEntity = determineWithClauseEntity();
  expectToken("=");
  QueryEntity secondEntity = determineWithClauseEntity();
  checkWithValidity(firstEntity, secondEntity);
  Clause c =
      Clause(ClauseType::With, vector<QueryEntity>{firstEntity, secondEntity});
  this->clauses.push_back(c);
}

Query PQLParser::constructQuery() {
  Query q = Query();
  q.setQuery(target, selectors, clauses);
  this->query = q;
  return q;
}
