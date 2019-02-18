#include "PQLParser.h"
#include <algorithm>
#include <queue>
#include <string.h>
#include <string>
#include <vector>
#include <stack>

using namespace std;

PQLParser::PQLParser() {}

std::queue<Token> PQLParser::parse(std::string query) {
	/*PQLLexer pqlLexer;
	std::queue<Token> tokens = pqlLexer.getTokenQueue(query);
	std::queue<Token> tokens;
	return tokens;*/
	return std::queue<Token>{};
}

Query PQLParser::getQuery() { return query; }

Query PQLParser::buildQuery(std::queue<Token> &tokenQueue) {
  this->tokenQueue = tokenQueue;
  while (!this->tokenQueue.empty()) {
    getNextToken();
    if (token.name == "Select") {
      setQueryTarget();
      tokenizeSelect();
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
  return constructQuery();
}

void PQLParser::getNextToken() {
        if (tokenQueue.empty())
        {
		throw std::logic_error("Token Queue is empty!");
        }
	Token nextToken = tokenQueue.front();
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
  std::map<std::string, QueryEntityType>::iterator it =
      entityMaps.find(token.name);
  if (it != entityMaps.end()) {
    QueryEntity qe = QueryEntity(it->second, token.name);
    this->target = qe;
  } else {
    throw std::logic_error("No matched synonym have been declared.");
  }
}

void PQLParser::tokenizeSelect()
{
	while(!this->tokenQueue.empty())
	{
		getNextToken();
		expectTokenIn(this->expectedIndicatorTokens);
		if (token.name == "such that")
		{
			getNextToken();
			expectTokenIn(expectedClauseTokens);
			if (token.name == "Follows")
			{
				expectToken("(");
				insertClauseFollows();
				expectToken(")");
			}
			if (token.name == "Follows*")
			{
				expectToken("(");
				insertClauseFollowsT();
				expectToken(")");
			}
			if (token.name == "Parent")
			{
				expectToken("(");
				insertClauseParent();
				expectToken(")");
			}
			if (token.name == "Parent*")
			{
				expectToken("(");
				insertClauseParentT();
				expectToken(")");
			}
			if (token.name == "Modifies")
			{
				expectToken("(");
				insertClauseModifiesS();
				expectToken(")");
			}
			if (token.name == "Uses")
			{
				expectToken("(");
				insertClauseUseS();
				expectToken(")");
			}
		}
		else
		{//pattern
			insertClausePattern();
		}
	}
}

void PQLParser::insertQueryEntityVariable() {
  getNextToken();
  QueryEntity qe = QueryEntity(QueryEntityType::Variable, token.name);
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
  QueryEntity qe = QueryEntity(QueryEntityType::Procedure, token.name);
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
  QueryEntity qe = QueryEntity(QueryEntityType::Read, token.name);
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
  QueryEntity qe = QueryEntity(QueryEntityType::Print, token.name);
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
  QueryEntity qe = QueryEntity(QueryEntityType::If, token.name);
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
  QueryEntity qe = QueryEntity(QueryEntityType::While, token.name);
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
  QueryEntity qe = QueryEntity(QueryEntityType::Assign, token.name);
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
  QueryEntity qe = QueryEntity(QueryEntityType::Call, token.name);
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
  QueryEntity qe = QueryEntity(QueryEntityType::Stmt, token.name);
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
  QueryEntity qe = QueryEntity(QueryEntityType::Constant, token.name);
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
  QueryEntity qe = QueryEntity(QueryEntityType::Stmt, token.name);
  this->entityMaps.insert(make_pair(token.name, QueryEntityType::Stmt));
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

bool isInt(string s) {
  if (s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+')))
    return false;

  char *p;
  strtol(s.c_str(), &p, 10);

  return (*p == 0);
}

QueryEntity PQLParser::determineQueryEntity()
{
	getNextToken();
        if (isInt(token.name))
        {
		    return QueryEntity(QueryEntityType::Line, token.name);
        } else if (token.name == "_")
        {
		    return QueryEntity(QueryEntityType::Underscore, token.name);
        } else if (token.name == "\"")
        {
		    getNextToken();
		    QueryEntity qe = QueryEntity(QueryEntityType::Name, token.name);
		    expectToken("\"");
		    return qe;
        } else
        {
		    std::map<std::string, QueryEntityType>::iterator it = entityMaps.find(token.name);
                    if (it != entityMaps.end())
                    {
			    QueryEntityType qet = it->second;
                            if (qet == QueryEntityType::Constant)
                            {
		                throw std::invalid_argument("invalid to have constant synonym as the argument for clauses");
                            }
			    return QueryEntity(qet, token.name);
                    }
                    throw std::logic_error("No matched synonym have been declared.");

        }
}

void PQLParser::checkFPValidity(QueryEntity firstEntity, QueryEntity secondEntity)
{//Stmt,Read,Print,Call,While,If,Assign,Line, Underscore are allowed
	std::vector<QueryEntityType> validTypes{QueryEntityType::Assign, QueryEntityType::If, QueryEntityType::While, 
	        QueryEntityType::Call, QueryEntityType::Print, QueryEntityType::Read, QueryEntityType::Stmt, QueryEntityType::Line, 
                QueryEntityType::Underscore};
        QueryEntityType firstType = firstEntity.type;
	QueryEntityType secondType = secondEntity.type;
	if (std::find(validTypes.begin(), validTypes.end(), firstType) != validTypes.end() && 
		std::find(validTypes.begin(), validTypes.end(), secondType) != validTypes.end())
	{
		return;
	}
	throw std::invalid_argument("invalid argument combination for clauses");
}

void PQLParser::checkModifiesValidity(QueryEntity firstEntity, QueryEntity secondEntity)
{ //first: Stmt,Read,Call,While,If,Assign, Procedure, Line, Name are allowed
  //second: Variable, Name, Underscore are allowed
	std::vector<QueryEntityType> validTypesFirst{ QueryEntityType::Assign, QueryEntityType::If, QueryEntityType::While,
			QueryEntityType::Call, QueryEntityType::Read, QueryEntityType::Stmt, QueryEntityType::Line,
			QueryEntityType::Procedure, QueryEntityType::Name};
	std::vector<QueryEntityType> validTypesSecond{ QueryEntityType::Variable, QueryEntityType::Name, QueryEntityType::Underscore};
	QueryEntityType firstType = firstEntity.type;
	QueryEntityType secondType = secondEntity.type;
	if (std::find(validTypesFirst.begin(), validTypesFirst.end(), firstType) != validTypesFirst.end() &&
		std::find(validTypesSecond.begin(), validTypesSecond.end(), secondType) != validTypesSecond.end())
	{
		return;
	}
	throw std::invalid_argument("invalid argument combination for clauses");
}

void PQLParser::checkUsesValidity(QueryEntity firstEntity, QueryEntity secondEntity)
{ //first: Stmt,Print,Call,While,If,Assign, Procedure, Line, Name are allowed
  //second: Variable, Name, Underscore are allowed
	std::vector<QueryEntityType> validTypesFirst{ QueryEntityType::Assign, QueryEntityType::If, QueryEntityType::While,
			QueryEntityType::Call, QueryEntityType::Print, QueryEntityType::Stmt, QueryEntityType::Line,
			QueryEntityType::Procedure, QueryEntityType::Name };
	std::vector<QueryEntityType> validTypesSecond{ QueryEntityType::Variable, QueryEntityType::Name, QueryEntityType::Underscore };
	QueryEntityType firstType = firstEntity.type;
	QueryEntityType secondType = secondEntity.type;
	if (std::find(validTypesFirst.begin(), validTypesFirst.end(), firstType) != validTypesFirst.end() &&
		std::find(validTypesSecond.begin(), validTypesSecond.end(), secondType) != validTypesSecond.end())
	{
		return;
	}
	throw std::invalid_argument("invalid argument combination for clauses");
}

void PQLParser::insertClauseFollows()
{
	QueryEntity firstEntity = determineQueryEntity();
	expectToken(",");
	QueryEntity secondEntity = determineQueryEntity();
	checkFPValidity(firstEntity, secondEntity);
	Clause c = Clause(ClauseType::Follows, vector<QueryEntity> {firstEntity, secondEntity});
	this->clauses.push_back(c);
}

void PQLParser::insertClauseFollowsT()
{
	QueryEntity firstEntity = determineQueryEntity();
	expectToken(",");
	QueryEntity secondEntity = determineQueryEntity();
	checkFPValidity(firstEntity, secondEntity);
	Clause c = Clause(ClauseType::FollowsT, vector<QueryEntity> {firstEntity, secondEntity});
	this->clauses.push_back(c);
}

void PQLParser::insertClauseParent()
{
	QueryEntity firstEntity = determineQueryEntity();
	expectToken(",");
	QueryEntity secondEntity = determineQueryEntity();
	checkFPValidity(firstEntity, secondEntity);
	Clause c = Clause(ClauseType::Parent, vector<QueryEntity> {firstEntity, secondEntity});
	this->clauses.push_back(c);
}

void PQLParser::insertClauseParentT()
{
	QueryEntity firstEntity = determineQueryEntity();
	expectToken(",");
	QueryEntity secondEntity = determineQueryEntity();
	checkFPValidity(firstEntity, secondEntity);
	Clause c = Clause(ClauseType::ParentT, vector<QueryEntity> {firstEntity, secondEntity});
	this->clauses.push_back(c);
}

void PQLParser::insertClauseModifiesS()
{
	QueryEntity firstEntity = determineQueryEntity();
	if (firstEntity.type == QueryEntityType::Underscore)
	{
		throw std::invalid_argument("semantically invalid to have underscore as the first argument for Modifies");
	}
        expectToken(",");
	QueryEntity secondEntity = determineQueryEntity();
	checkModifiesValidity(firstEntity, secondEntity);
	Clause c = Clause(ClauseType::ModifiesS, vector<QueryEntity> {firstEntity, secondEntity});
	this->clauses.push_back(c);
}

void PQLParser::insertClauseUseS()
{
	QueryEntity firstEntity = determineQueryEntity();
        if (firstEntity.type == QueryEntityType::Underscore)
        {
		throw std::invalid_argument("semantically invalid to have underscore as the first argument for Uses");
        }
	expectToken(",");
	QueryEntity secondEntity = determineQueryEntity();
	checkUsesValidity(firstEntity, secondEntity);
	Clause c = Clause(ClauseType::UsesS, vector<QueryEntity> {firstEntity, secondEntity});
	this->clauses.push_back(c);
}

void PQLParser::insertClausePattern()
{
	getNextToken();
	std::map<std::string, QueryEntityType>::iterator it = entityMaps.find(token.name);
	if (it != entityMaps.end())
	{
		QueryEntityType qet = it->second;
		if (qet != QueryEntityType::Assign)
		{
			throw std::invalid_argument("syn-assign must be declared as synonym of assignment for pattern");
		}
		QueryEntity firstPara = QueryEntity(qet, token.name);
		expectToken("(");
		QueryEntity qe = determineQueryEntity();
	        std::vector<QueryEntityType> validTypes{ QueryEntityType::Variable, QueryEntityType::Name, QueryEntityType::Underscore };
		if (std::find(validTypes.begin(), validTypes.end(), qe.type) != validTypes.end())
		{
			QueryEntity secondPara = QueryEntity(qe.type, token.name);
			expectToken(",");
			QueryEntity thirdPara = parseExpression();
			Clause c = Clause(ClauseType::AssignPatt, vector<QueryEntity> {firstPara, secondPara, thirdPara});
			this->clauses.push_back(c);
		} else
		{
			throw std::invalid_argument("Invalid query entity type for fisrt parameter of pattern.");
		}
	} else
	{
		throw std::logic_error("No matched synonym have been declared.");
	}

}

int precedence(char c)
{
	if (c == '^') return 3;
	else if (c == '*' || c == '/') return 2;
	else if (c == '+' || c == '-') return 1;
	else return -1;
}

string convertToPostfix(string expr)
{
	std::string::iterator end_pos = std::remove(expr.begin(), expr.end(), ' ');
	expr.erase(end_pos, expr.end());
	std::stack<char> st;
	st.push('N');
	string res;
	for (int i = 0; i < expr.size(); i++)
	{
		if ((expr[i] >= 'a' && expr[i] <= 'z') || (expr[i] >= 'A' && expr[i] <= 'Z') || isdigit(expr[i])) {
			res += expr[i];
		}
		else if (expr[i] == '(') {
			st.push('(');
		}
		else if (expr[i] == ')')
		{
			while (st.top() != 'N' && st.top() != '(')
			{
				char c = st.top();
				st.pop();
				res += c;
			}
			if (st.top() == '(')
			{
				char c = st.top();
				st.pop();
			}
		}
		else {
			while (st.top() != 'N' && precedence(expr[i]) <= precedence(st.top()))
			{
				char c = st.top();
				st.pop();
				res += c;
			}
			st.push(expr[i]);
		}
	}
	while (st.top() != 'N')
	{
		char c = st.top();
		st.pop();
		res += c;
	}
	return res;
}

QueryEntity PQLParser::parseExpression()
{
	getNextToken();
	if (token.name == "_" && tokenQueue.front().name == ")") //underscore
	{
		expectToken(")");
		return QueryEntity(QueryEntityType::Expression, "_");
	}
	else if (token.name == "_" && tokenQueue.front().name == "\"") //partial match
	{
		getNextToken();
		getNextToken();
		string expression = token.name;
		string postfixExpr = convertToPostfix(expression);
		expectToken("\"");
		expectToken("_");
		expectToken(")");
		return QueryEntity(QueryEntityType::Expression, "_" + postfixExpr + "_");
	}
	else if (token.name == "\"") //exact match
	{
		getNextToken();
		string expression = token.name;
		string postfixExpr = convertToPostfix(expression);
		expectToken("\"");
		expectToken(")");
		return QueryEntity(QueryEntityType::Expression, postfixExpr);
	} else //invalid token 
	{
	  throw std::logic_error("Invalid Expression");
	}
}

Query PQLParser::constructQuery()
{
	Query q = Query();
	q.setQuery(target, selectors, clauses);
	this->query = q;
	return q;
}
