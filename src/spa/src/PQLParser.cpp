#include "PQLParser.h"
#include <algorithm>
#include <queue>
#include <string.h>
#include <string>
#include <vector>

using namespace std;

PQLParser::PQLParser() {
}

std::queue<Token> PQLParser::parse(std::string query) {
	/*PQLLexer pqlLexer;
	std::queue<Token> tokens = pqlLexer.tokenizeQuery(query);
	return tokens;*/
	std::queue<Token> tokens;
	return tokens;
}

Query PQLParser::getQuery()
{
	return query;
}

void PQLParser::buildQuery(std::queue<Token> &tokenQueue) {
	this->tokenQueue = tokenQueue;
	while (!this->tokenQueue.empty()) {
		getNextToken();
                if (token.name == "Select")
                {
			setQueryTarget();
			tokenizeSelect();
			break;
                }
		expectTokenIn(this->expectedEntityTokens);
		if (token.name == "variable")
		{
			insertQueryEntityVariable();
			expectToken(";");
		}
		else if (token.name == "procedure")
		{
			insertQueryEntityProcedure();
			expectToken(";");
		}
		else if (token.name == "read")
		{
			insertQueryEntityRead();
			expectToken(";");
		}
		else if (token.name == "print")
		{
			insertQueryEntityPrint();
			expectToken(";");
		}
		else if (token.name == "stmt")
		{
			insertQueryEntityStmt();
			expectToken(";");
		}
		else if (token.name == "call")
		{
			insertQueryEntityCall();
			expectToken(";");
		}
		else if (token.name == "assign")
		{
			insertQueryEntityAssign();
			expectToken(";");
		}
		else if (token.name == "constant")
		{
			insertQueryEntityConstant();
			expectToken(";");
		}
		else if (token.name == "if")
		{
			insertQueryEntityIf();
			expectToken(";");
		}
		else if (token.name == "while")
		{
			insertQueryEntityWhile();
			expectToken(";");
		}
		else
		{
			insertQueryEntityProgline();
			expectToken(";");
		}

	}
	constructQuery();
}

void PQLParser::getNextToken() {
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
	for (int i = 0; i < expectedTokens.size(); i++)
	{
		if (token.name == expectedTokens[i]) {
			isIn = true;
			break;
		}
	}
	if (!isIn)
	{
		throw std::logic_error("Expected query entity type but got '" + token.name + "'");
	}

}

void PQLParser::setQueryTarget()
{
	getNextToken();
	this->target = token.name;
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
		{
		}
	}
}

void PQLParser::insertQueryEntityVariable()
{
	getNextToken();
	QueryEntity qe = QueryEntity(QueryEntityType::Variable, token.name);
	this->entityMaps.insert(make_pair(token.name, QueryEntityType::Variable));
	this->selectors.push_back(qe);
}

void PQLParser::insertQueryEntityProcedure()
{
	getNextToken();
	QueryEntity qe = QueryEntity(QueryEntityType::Procedure, token.name);
	this->entityMaps.insert(make_pair(token.name, QueryEntityType::Procedure));
	this->selectors.push_back(qe);
}

void PQLParser::insertQueryEntityRead()
{
	getNextToken();
	QueryEntity qe = QueryEntity(QueryEntityType::Read, token.name);
	this->entityMaps.insert(make_pair(token.name, QueryEntityType::Read));
	this->selectors.push_back(qe);
}

void PQLParser::insertQueryEntityPrint()
{
	getNextToken();
	QueryEntity qe = QueryEntity(QueryEntityType::Print, token.name);
	this->entityMaps.insert(make_pair(token.name, QueryEntityType::Print));
	this->selectors.push_back(qe);
}

void PQLParser::insertQueryEntityIf()
{
	getNextToken();
	QueryEntity qe = QueryEntity(QueryEntityType::If, token.name);
	this->entityMaps.insert(make_pair(token.name, QueryEntityType::If));
	this->selectors.push_back(qe);
}

void PQLParser::insertQueryEntityWhile()
{
	getNextToken();
	QueryEntity qe = QueryEntity(QueryEntityType::While, token.name);
	this->entityMaps.insert(make_pair(token.name, QueryEntityType::While));
	this->selectors.push_back(qe);
}

void PQLParser::insertQueryEntityAssign()
{
	getNextToken();
	QueryEntity qe = QueryEntity(QueryEntityType::Assign, token.name);
	this->entityMaps.insert(make_pair(token.name, QueryEntityType::Assign));
	this->selectors.push_back(qe);
}

void PQLParser::insertQueryEntityCall()
{
	getNextToken();
	QueryEntity qe = QueryEntity(QueryEntityType::Call, token.name);
	this->entityMaps.insert(make_pair(token.name, QueryEntityType::Call));
	this->selectors.push_back(qe);
}

void PQLParser::insertQueryEntityStmt()
{
	getNextToken();
	QueryEntity qe = QueryEntity(QueryEntityType::Stmt, token.name);
	this->entityMaps.insert(make_pair(token.name, QueryEntityType::Stmt));
	this->selectors.push_back(qe);
}

void PQLParser::insertQueryEntityConstant()
{
	getNextToken();
	QueryEntity qe = QueryEntity(QueryEntityType::Constant, token.name);
	this->entityMaps.insert(make_pair(token.name, QueryEntityType::Constant));
	this->selectors.push_back(qe);
}

void PQLParser::insertQueryEntityProgline()
{
	getNextToken();
	QueryEntity qe = QueryEntity(QueryEntityType::Progline, token.name);
	this->entityMaps.insert(make_pair(token.name, QueryEntityType::Progline));
	this->selectors.push_back(qe);
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
			return QueryEntity(qet, token.name);
                } else
                {
			throw std::logic_error("No matched synonym have been declared.");
                }
		
    }
}

void PQLParser::insertClauseFollows()
{
	QueryEntity firstEntity = determineQueryEntity();
	expectToken(",");
	QueryEntity secondEntity = determineQueryEntity();
	Clause c = Clause(ClauseType::Follows, vector<QueryEntity> {firstEntity, secondEntity});
	this->clauses.push_back(c);
}

void PQLParser::insertClauseFollowsT()
{
	QueryEntity firstEntity = determineQueryEntity();
	expectToken(",");
	QueryEntity secondEntity = determineQueryEntity();
	Clause c = Clause(ClauseType::FollowsT, vector<QueryEntity> {firstEntity, secondEntity});
	this->clauses.push_back(c);
}

void PQLParser::insertClauseParent()
{
	QueryEntity firstEntity = determineQueryEntity();
	expectToken(",");
	QueryEntity secondEntity = determineQueryEntity();
	Clause c = Clause(ClauseType::Parent, vector<QueryEntity> {firstEntity, secondEntity});
	this->clauses.push_back(c);
}

void PQLParser::insertClauseParentT()
{
	QueryEntity firstEntity = determineQueryEntity();
	expectToken(",");
	QueryEntity secondEntity = determineQueryEntity();
	Clause c = Clause(ClauseType::ParentT, vector<QueryEntity> {firstEntity, secondEntity});
	this->clauses.push_back(c);
}

void PQLParser::insertClauseModifiesS()
{
	QueryEntity firstEntity = determineQueryEntity();
	if (firstEntity.type == QueryEntityType::Underscore)
	{
		throw std::invalid_argument("invalid to have underscore as the first argument for Modifies");
	}
        expectToken(",");
	QueryEntity secondEntity = determineQueryEntity();
	Clause c = Clause(ClauseType::ModifiesS, vector<QueryEntity> {firstEntity, secondEntity});
	this->clauses.push_back(c);
}

void PQLParser::insertClauseUseS()
{
	QueryEntity firstEntity = determineQueryEntity();
        if (firstEntity.type == QueryEntityType::Underscore)
        {
		throw std::invalid_argument("invalid to have underscore as the first argument for Uses");
        }
	expectToken(",");
	QueryEntity secondEntity = determineQueryEntity();
	Clause c = Clause(ClauseType::UsesS, vector<QueryEntity> {firstEntity, secondEntity});
	this->clauses.push_back(c);
}

void PQLParser::constructQuery()
{
	vector<QueryEntity> s(selectors);
	vector<Clause> c(clauses);
	Query q = Query();
	q.setQuery(s, c);
	this->query = q;
}

//void PQLParser::Tokenize(string input) {
//  vector<string> token = vectorize(input);
//  while (0 < token.size()) {
//    if (find(token.begin(), token.end(), "procedure") != token.end()) {
//      tokenizeProcedure(token);
//      token.erase(token.begin());
//      token.erase(token.begin());
//    }
//
//    if (find(token.begin(), token.end(), "variable") != token.end()) {
//      tokenizeVariable(token);
//      token.erase(token.begin());
//      token.erase(token.begin());
//    }
//
//    if (find(token.begin(), token.end(), "stmt") != token.end()) {
//      tokenizeStmt(token);
//      token.erase(token.begin());
//      token.erase(token.begin());
//    }
//
//    if (find(token.begin(), token.end(), "read") != token.end()) {
//      tokenizeRead(token);
//      token.erase(token.begin());
//      token.erase(token.begin());
//    }
//
//    if (find(token.begin(), token.end(), "print") != token.end()) {
//      tokenizePrint(token);
//      token.erase(token.begin());
//      token.erase(token.begin());
//    }
//
//    if (find(token.begin(), token.end(), "while") != token.end()) {
//      tokenizeWhile(token);
//      token.erase(token.begin());
//      token.erase(token.begin());
//    }
//
//    if (find(token.begin(), token.end(), "if") != token.end()) {
//      tokenizeIf(token);
//      token.erase(token.begin());
//      token.erase(token.begin());
//    }
//
//    if (find(token.begin(), token.end(), "call") != token.end()) {
//      tokenizeCall(token);
//      token.erase(token.begin());
//      token.erase(token.begin());
//    }
//
//    if (find(token.begin(), token.end(), "assign") != token.end()) {
//      tokenizeAssign(token);
//      token.erase(token.begin());
//      token.erase(token.begin());
//    }
//
//    if (find(token.begin(), token.end(), "constant") != token.end()) {
//      tokenizeConstant(token);
//      token.erase(token.begin());
//      token.erase(token.begin());
//    }
//
//    // if (find(token.begin(), token.end(), "select") != token.end()) {
//    if (token[0] == "select") {
//      tokenizeSelect(token);
//      token.erase(token.begin());
//      token.erase(token.begin());
//      if (token.size() > 0) {
//        if (token[0] == "such" && token[1] == "that") {
//          token.erase(token.begin());
//          token.erase(token.begin());
//          while (0 < token.size()) {
//            if (find(token.begin(), token.end(), "Modifies") != token.end()) {
//              tokenizeModifies(token);
//              token.erase(token.begin());
//              token.erase(token.begin());
//              token.erase(token.begin());
//            }
//
//            if (find(token.begin(), token.end(), "Uses") != token.end()) {
//              tokenizeUses(token);
//              token.erase(token.begin());
//              token.erase(token.begin());
//              token.erase(token.begin());
//            }
//
//            if (find(token.begin(), token.end(), "Parent*") != token.end()) {
//              tokenizeParentT(token);
//              token.erase(token.begin());
//              token.erase(token.begin());
//              token.erase(token.begin());
//            } else if (find(token.begin(), token.end(), "Parent") !=
//                       token.end()) {
//              tokenizeParent(token);
//              token.erase(token.begin());
//              token.erase(token.begin());
//              token.erase(token.begin());
//            }
//
//            if (find(token.begin(), token.end(), "Follows*") != token.end()) {
//              tokenizeFollowsT(token);
//              token.erase(token.begin());
//              token.erase(token.begin());
//              token.erase(token.begin());
//            } else if (find(token.begin(), token.end(), "Follows") !=
//                       token.end()) {
//              tokenizeFollows(token);
//              token.erase(token.begin());
//              token.erase(token.begin());
//              token.erase(token.begin());
//            }
//          }
//        }
//      }
//    }
//  }
//
//  if (find(token.begin(), token.end(), "variable") != token.end()) {
//    tokenizeVariable(token);
//    token.erase(token.begin());
//    token.erase(token.begin());
//  }
//
//  if (find(token.begin(), token.end(), "select") != token.end()) {
//    tokenizeSelect(token);
//    token.erase(token.begin());
//    token.erase(token.begin());
//  }
//}
//vector<string> PQLParser::vectorize(string) { return vector<string>(); }
//bool isInt(string s) {
//  if (s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+')))
//    return false;
//
//  char *p;
//  strtol(s.c_str(), &p, 10);
//
//  return (*p == 0);
//}
//
//queue<pair<DeclarationType, string>> PQLParser::getDeclarationQueue() {
//  return declarationQueue;
//}
//
//queue<tuple<RelationType, string, string>> PQLParser::getSelectQueue() {
//  return selectQueue;
//}
//
//string PQLParser::getTarget() { return target; }
//
//void PQLParser::tokenizeVariable(vector<string> token) {
//  declarationQueue.push(make_pair(DeclarationType::Variable, token[1]));
//}
//
//void PQLParser::tokenizeProcedure(vector<string> token) {
//  declarationQueue.push(make_pair(DeclarationType::Procedure, token[1]));
//}
//
//void PQLParser::tokenizeRead(vector<string> token) {
//  declarationQueue.push(make_pair(DeclarationType::Read, token[1]));
//}
//
//void PQLParser::tokenizePrint(vector<string> token) {
//  declarationQueue.push(make_pair(DeclarationType::Print, token[1]));
//}
//
//void PQLParser::tokenizeWhile(vector<string> token) {
//  declarationQueue.push(make_pair(DeclarationType::While, token[1]));
//}
//
//void PQLParser::tokenizeIf(vector<string> token) {
//  declarationQueue.push(make_pair(DeclarationType::If, token[1]));
//}
//
//void PQLParser::tokenizeAssign(vector<string> token) {
//  declarationQueue.push(make_pair(DeclarationType::Assign, token[1]));
//}
//
//void PQLParser::tokenizeStmt(vector<string> token) {
//  declarationQueue.push(make_pair(DeclarationType::Stmt, token[1]));
//}
//
//void PQLParser::tokenizeCall(vector<string> token) {
//  declarationQueue.push(make_pair(DeclarationType::Call, token[1]));
//}
//
//void PQLParser::tokenizeConstant(vector<string> token) {
//  declarationQueue.push(make_pair(DeclarationType::Constant, token[1]));
//}
//
//void PQLParser::tokenizeSelect(vector<string> token) { target = token[1]; }
//
//void PQLParser::tokenizePattern(vector<string>) {}
//
//void PQLParser::tokenizeFollows(vector<string> token) {
//  selectQueue.push(make_tuple(RelationType::Follows, token[1], token[2]));
//}
//
//void PQLParser::tokenizeFollowsT(vector<string> token) {
//  selectQueue.push(make_tuple(RelationType::FollowsT, token[1], token[2]));
//}
//
//void PQLParser::tokenizeParent(vector<string> token) {
//  selectQueue.push(make_tuple(RelationType::Parent, token[1], token[2]));
//}
//
//void PQLParser::tokenizeParentT(vector<string> token) {
//  selectQueue.push(make_tuple(RelationType::ParentT, token[1], token[2]));
//}
//
//void PQLParser::tokenizeUses(vector<string> token) {
//  if (isInt(token[0])) {
//    selectQueue.push(make_tuple(RelationType::UsesS, token[1], token[2]));
//  } else {
//    selectQueue.push(make_tuple(RelationType::UsesP, token[1], token[2]));
//  }
//}
//
//void PQLParser::tokenizeModifies(vector<string> token) {
//  if (isInt(token[0])) {
//    selectQueue.push(make_tuple(RelationType::ModifiesS, token[1], token[2]));
//  } else {
//    selectQueue.push(make_tuple(RelationType::ModifiesP, token[1], token[2]));
//  }
//}
