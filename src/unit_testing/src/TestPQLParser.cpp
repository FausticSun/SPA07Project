#include "PQLParser.h"

#include "catch.hpp"

using namespace std;

SCENARIO("Only declare one variable a") {
  queue<Token> tokens;
  tokens.push(Token(TokenType::Identifier, "variable"));
  tokens.push(Token(TokenType::Identifier, "a"));
  tokens.push(Token(TokenType::Identifier, ";"));
  PQLParser p = PQLParser();
  const Query q = p.buildQuery(tokens);
  const std::vector<QueryEntity> selectors = q.selectors;

        WHEN("Successfully parsed:")
        {
	  SECTION("selectors: Only one QueryEntity is parsed inside") {
		  REQUIRE(selectors.size() == 1);
		  const QueryEntity qe = selectors.front();
		  REQUIRE(qe.name == "a");
		  REQUIRE(qe.type == QueryEntityType::Variable);
	  }
        }
}

SCENARIO("Declare 2 variables a and b") {
	queue<Token> tokens;
	tokens.push(Token(TokenType::Identifier, "variable"));
	tokens.push(Token(TokenType::Identifier, "a"));
	tokens.push(Token(TokenType::Identifier, ","));
	tokens.push(Token(TokenType::Identifier, "b"));
	tokens.push(Token(TokenType::Identifier, ";"));
	PQLParser p = PQLParser();
	const Query q = p.buildQuery(tokens);
	const std::vector<QueryEntity> selectors = q.selectors;

	WHEN("Successfully parsed:")
	{
		SECTION("selectors: two QueryEntity are parsed inside") {
			REQUIRE(selectors.size() == 2);
			const QueryEntity qe1 = selectors.front();
			const QueryEntity qe2 = selectors[1];
			REQUIRE(qe1.name == "a");
			REQUIRE(qe1.type == QueryEntityType::Variable);
			REQUIRE(qe2.name == "b");
			REQUIRE(qe2.type == QueryEntityType::Variable);
		}
	}
}

SCENARIO("Declare several different type of query entities, one for each") {
	queue<Token> tokens;
	tokens.push(Token(TokenType::Identifier, "variable"));
	tokens.push(Token(TokenType::Identifier, "a"));
	tokens.push(Token(TokenType::Identifier, ";"));
	tokens.push(Token(TokenType::Identifier, "procedure"));
	tokens.push(Token(TokenType::Identifier, "p"));
	tokens.push(Token(TokenType::Identifier, ";"));
	tokens.push(Token(TokenType::Identifier, "stmt"));
	tokens.push(Token(TokenType::Identifier, "s"));
	tokens.push(Token(TokenType::Identifier, ";"));
	PQLParser p = PQLParser();
	const Query q = p.buildQuery(tokens);
	const std::vector<QueryEntity> selectors = q.selectors;

	WHEN("Successfully parsed:")
        {
	      SECTION("selectors: three QueryEntity are parsed inside") {
		      REQUIRE(selectors.size() == 3);
		      const QueryEntity qe1 = selectors.front();
		      const QueryEntity qe2 = selectors[1];
		      const QueryEntity qe3 = selectors[2];
		      REQUIRE(qe1.name == "a");
		      REQUIRE(qe1.type == QueryEntityType::Variable);
		      REQUIRE(qe2.name == "p");
		      REQUIRE(qe2.type == QueryEntityType::Procedure);
		      REQUIRE(qe3.name == "s");
		      REQUIRE(qe3.type == QueryEntityType::Stmt);
	      }
        }
}

SCENARIO("Declare several different type of query entities, several for each") {
	queue<Token> tokens;
	tokens.push(Token(TokenType::Identifier, "read"));
	tokens.push(Token(TokenType::Identifier, "r1"));
	tokens.push(Token(TokenType::Identifier, ","));
	tokens.push(Token(TokenType::Identifier, "r2"));
	tokens.push(Token(TokenType::Identifier, ";"));
	tokens.push(Token(TokenType::Identifier, "if"));
	tokens.push(Token(TokenType::Identifier, "ifs1"));
	tokens.push(Token(TokenType::Identifier, ","));
	tokens.push(Token(TokenType::Identifier, "ifs2"));
	tokens.push(Token(TokenType::Identifier, ";"));
	tokens.push(Token(TokenType::Identifier, "constant"));
	tokens.push(Token(TokenType::Identifier, "c1"));
	tokens.push(Token(TokenType::Identifier, ","));
	tokens.push(Token(TokenType::Identifier, "c2"));
	tokens.push(Token(TokenType::Identifier, ","));
	tokens.push(Token(TokenType::Identifier, "c3"));
	tokens.push(Token(TokenType::Identifier, ";"));
	PQLParser p = PQLParser();
	const Query q = p.buildQuery(tokens);
	const std::vector<QueryEntity> selectors = q.selectors;

	WHEN("Successfully parsed:")
        {
	  SECTION("selectors: three types of QueryEntity are parsed inside, and two for each type") {
		  REQUIRE(selectors.size() == 7);
		  const QueryEntity qe1 = selectors.front();
		  const QueryEntity qe2 = selectors[1];
		  const QueryEntity qe3 = selectors[2];
		  const QueryEntity qe4 = selectors[3];
		  const QueryEntity qe5 = selectors[4];
		  const QueryEntity qe6 = selectors[5];
		  const QueryEntity qe7 = selectors[6];
		  REQUIRE(qe1.name == "r1");
		  REQUIRE(qe1.type == QueryEntityType::Read);
		  REQUIRE(qe2.name == "r2");
		  REQUIRE(qe2.type == QueryEntityType::Read);
		  REQUIRE(qe3.name == "ifs1");
		  REQUIRE(qe3.type == QueryEntityType::If);
		  REQUIRE(qe4.name == "ifs2");
		  REQUIRE(qe4.type == QueryEntityType::If);
		  REQUIRE(qe5.name == "c1");
		  REQUIRE(qe5.type == QueryEntityType::Constant);
		  REQUIRE(qe6.name == "c2");
		  REQUIRE(qe6.type == QueryEntityType::Constant);
		  REQUIRE(qe7.name == "c3");
		  REQUIRE(qe7.type == QueryEntityType::Constant);
	  }
        }
}

SCENARIO("Select all variables") {
	queue<Token> tokens;
	tokens.push(Token(TokenType::Identifier, "variable"));
	tokens.push(Token(TokenType::Identifier, "a"));
	tokens.push(Token(TokenType::Identifier, ";"));
	tokens.push(Token(TokenType::Identifier, "Select"));
	tokens.push(Token(TokenType::Identifier, "a"));
	PQLParser p = PQLParser();
	const Query q = p.buildQuery(tokens);
	const std::vector<QueryEntity> selectors = q.selectors;
        const QueryEntity tar = q.target;

        WHEN("Successfully parsed:")
        {
	      SECTION("selectors: one QueryEntity is inside") {
		      REQUIRE(selectors.size() == 1);
		      const QueryEntity qe = selectors.front();
		      REQUIRE(qe.name == "a");
		      REQUIRE(qe.type == QueryEntityType::Variable);
	      }
              SECTION("target")
	      {
		      REQUIRE(tar.name == "a");
		      REQUIRE(tar.type == QueryEntityType::Variable);
	      }
        }
}

SCENARIO("Including one such that clause") {
	SECTION("One valid Follows clause")
	{
		queue<Token> tokens;
		tokens.push(Token(TokenType::Identifier, "assign"));
		tokens.push(Token(TokenType::Identifier, "a"));
		tokens.push(Token(TokenType::Identifier, ";"));
		tokens.push(Token(TokenType::Identifier, "Select"));
		tokens.push(Token(TokenType::Identifier, "a"));
		tokens.push(Token(TokenType::Identifier, "such that"));
		tokens.push(Token(TokenType::Identifier, "Follows"));
		tokens.push(Token(TokenType::Identifier, "("));
		tokens.push(Token(TokenType::Identifier, "a"));
		tokens.push(Token(TokenType::Identifier, ","));
		tokens.push(Token(TokenType::Identifier, "5"));
		tokens.push(Token(TokenType::Identifier, ")"));
		PQLParser p = PQLParser();
		Query q = p.buildQuery(tokens);
		std::vector<QueryEntity> selectors = q.selectors;
		std::vector<Clause> clauses = q.clauses;
		QueryEntity tar = q.target;

		WHEN("Successfully parsed:")
		{
			SECTION("selectors: one QueryEntity is inside") {
				REQUIRE(selectors.size() == 1);
				QueryEntity qe = selectors.front();
				REQUIRE(qe.name == "a");
				REQUIRE(qe.type == QueryEntityType::Assign);
			}
			SECTION("target")
			{
				REQUIRE(tar.name == "a");
				REQUIRE(tar.type == QueryEntityType::Assign);
			}
			SECTION("clauses: one Follows clause is inside")
			{
				REQUIRE(clauses.size() == 1);
				Clause c = clauses.front();
				std::vector<QueryEntity> parameters = c.parameters;
				REQUIRE(c.clauseType == ClauseType::Follows);
				REQUIRE(parameters[0].name == "a");
				REQUIRE(parameters[0].type == QueryEntityType::Assign);
				REQUIRE(parameters[1].name == "5");
				REQUIRE(parameters[1].type == QueryEntityType::Line);
			}
		}
	}
	SECTION("One valid Follows* clause")
	{
		queue<Token> tokens;
		tokens.push(Token(TokenType::Identifier, "while"));
		tokens.push(Token(TokenType::Identifier, "w"));
		tokens.push(Token(TokenType::Identifier, ";"));
		tokens.push(Token(TokenType::Identifier, "Select"));
		tokens.push(Token(TokenType::Identifier, "w"));
		tokens.push(Token(TokenType::Identifier, "such that"));
		tokens.push(Token(TokenType::Identifier, "Follows*"));
		tokens.push(Token(TokenType::Identifier, "("));
		tokens.push(Token(TokenType::Identifier, "w"));
		tokens.push(Token(TokenType::Identifier, ","));
		tokens.push(Token(TokenType::Identifier, "_"));
		tokens.push(Token(TokenType::Identifier, ")"));
		PQLParser p = PQLParser();
		Query q = p.buildQuery(tokens);
		std::vector<QueryEntity> selectors = q.selectors;
		std::vector<Clause> clauses = q.clauses;
		QueryEntity tar = q.target;
		WHEN("Successfully parsed:")
		{
			SECTION("selectors: one QueryEntity is inside") {
				REQUIRE(selectors.size() == 1);
				QueryEntity qe = selectors.front();
				REQUIRE(qe.name == "w");
				REQUIRE(qe.type == QueryEntityType::While);
			}
			SECTION("target")
			{
				REQUIRE(tar.name == "w");
				REQUIRE(tar.type == QueryEntityType::While);
			}
			SECTION("clauses: one FollowsT clause is inside")
			{
				REQUIRE(clauses.size() == 1);
				Clause c = clauses.front();
				std::vector<QueryEntity> parameters = c.parameters;
				REQUIRE(c.clauseType == ClauseType::FollowsT);
				REQUIRE(parameters[0].name == "w");
				REQUIRE(parameters[0].type == QueryEntityType::While);
				REQUIRE(parameters[1].name == "_");
				REQUIRE(parameters[1].type == QueryEntityType::Underscore);
			}
		}
	}
	SECTION("One valid Parent clause")
	{
		queue<Token> tokens;
		tokens.push(Token(TokenType::Identifier, "call"));
		tokens.push(Token(TokenType::Identifier, "c"));
		tokens.push(Token(TokenType::Identifier, ";"));
		tokens.push(Token(TokenType::Identifier, "Select"));
		tokens.push(Token(TokenType::Identifier, "c"));
		tokens.push(Token(TokenType::Identifier, "such that"));
		tokens.push(Token(TokenType::Identifier, "Parent"));
		tokens.push(Token(TokenType::Identifier, "("));
		tokens.push(Token(TokenType::Identifier, "_"));
		tokens.push(Token(TokenType::Identifier, ","));
		tokens.push(Token(TokenType::Identifier, "c"));
		tokens.push(Token(TokenType::Identifier, ")"));
		PQLParser p = PQLParser();
		Query q = p.buildQuery(tokens);
		std::vector<QueryEntity> selectors = q.selectors;
		std::vector<Clause> clauses = q.clauses;
		QueryEntity tar = q.target;
		WHEN("Successfully parsed:")
		{
			SECTION("selectors: one QueryEntity is inside") {
				REQUIRE(selectors.size() == 1);
				QueryEntity qe = selectors.front();
				REQUIRE(qe.name == "c");
				REQUIRE(qe.type == QueryEntityType::Call);
			}
			SECTION("target")
			{
				REQUIRE(tar.name == "c");
				REQUIRE(tar.type == QueryEntityType::Call);
			}
			SECTION("clauses: one Parent clause is inside")
			{
				REQUIRE(clauses.size() == 1);
				Clause c = clauses.front();
				std::vector<QueryEntity> parameters = c.parameters;
				REQUIRE(c.clauseType == ClauseType::Parent);
				REQUIRE(parameters[0].name == "_");
				REQUIRE(parameters[0].type == QueryEntityType::Underscore);
				REQUIRE(parameters[1].name == "c");
				REQUIRE(parameters[1].type == QueryEntityType::Call);
			}
		}
	}
	SECTION("One valid Parent* clause")
	{
		queue<Token> tokens;
		tokens.push(Token(TokenType::Identifier, "print"));
		tokens.push(Token(TokenType::Identifier, "pr"));
		tokens.push(Token(TokenType::Identifier, ";"));
		tokens.push(Token(TokenType::Identifier, "Select"));
		tokens.push(Token(TokenType::Identifier, "pr"));
		tokens.push(Token(TokenType::Identifier, "such that"));
		tokens.push(Token(TokenType::Identifier, "Parent*"));
		tokens.push(Token(TokenType::Identifier, "("));
		tokens.push(Token(TokenType::Identifier, "_"));
		tokens.push(Token(TokenType::Identifier, ","));
		tokens.push(Token(TokenType::Identifier, "pr"));
		tokens.push(Token(TokenType::Identifier, ")"));
		PQLParser p = PQLParser();
		Query q = p.buildQuery(tokens);
		std::vector<QueryEntity> selectors = q.selectors;
		std::vector<Clause> clauses = q.clauses;
		QueryEntity tar = q.target;
		WHEN("Successfully parsed:")
		{
			SECTION("selectors: one QueryEntity is inside") {
				REQUIRE(selectors.size() == 1);
				QueryEntity qe = selectors.front();
				REQUIRE(qe.name == "pr");
				REQUIRE(qe.type == QueryEntityType::Print);
			}
			SECTION("target")
			{
				REQUIRE(tar.name == "pr");
				REQUIRE(tar.type == QueryEntityType::Print);
			}
			SECTION("clauses: one ParentT clause is inside")
			{
				REQUIRE(clauses.size() == 1);
				Clause c = clauses.front();
				std::vector<QueryEntity> parameters = c.parameters;
				REQUIRE(c.clauseType == ClauseType::ParentT);
				REQUIRE(parameters[0].name == "_");
				REQUIRE(parameters[0].type == QueryEntityType::Underscore);
				REQUIRE(parameters[1].name == "pr");
				REQUIRE(parameters[1].type == QueryEntityType::Print);
			}
		}
	}
	SECTION("One valid Modifies clause")
	{
		queue<Token> tokens;
		tokens.push(Token(TokenType::Identifier, "prog_line"));
		tokens.push(Token(TokenType::Identifier, "pl"));
		tokens.push(Token(TokenType::Identifier, ";"));
		tokens.push(Token(TokenType::Identifier, "variable"));
		tokens.push(Token(TokenType::Identifier, "v"));
		tokens.push(Token(TokenType::Identifier, ";"));
		tokens.push(Token(TokenType::Identifier, "Select"));
		tokens.push(Token(TokenType::Identifier, "pl"));
		tokens.push(Token(TokenType::Identifier, "such that"));
		tokens.push(Token(TokenType::Identifier, "Modifies"));
		tokens.push(Token(TokenType::Identifier, "("));
		tokens.push(Token(TokenType::Identifier, "10"));
		tokens.push(Token(TokenType::Identifier, ","));
		tokens.push(Token(TokenType::Identifier, "v"));
		tokens.push(Token(TokenType::Identifier, ")"));
		PQLParser p = PQLParser();
		Query q = p.buildQuery(tokens);
		std::vector<QueryEntity> selectors = q.selectors;
		std::vector<Clause> clauses = q.clauses;
		QueryEntity tar = q.target;
		WHEN("Successfully parsed:")
		{
			SECTION("selectors: two QueryEntity is inside") {
				REQUIRE(selectors.size() == 2);
				QueryEntity qe = selectors.front();
				QueryEntity qe2 = selectors[1];
				REQUIRE(qe.name == "pl");
				REQUIRE(qe.type == QueryEntityType::Stmt);
				REQUIRE(qe2.name == "v");
				REQUIRE(qe2.type == QueryEntityType::Variable);
			}
			SECTION("target")
			{
				REQUIRE(tar.name == "pl");
				REQUIRE(tar.type == QueryEntityType::Stmt);
			}
			SECTION("clauses: one ModifiesS clause is inside")
			{
				REQUIRE(clauses.size() == 1);
				Clause c = clauses.front();
				std::vector<QueryEntity> parameters = c.parameters;
				REQUIRE(c.clauseType == ClauseType::ModifiesS);
				REQUIRE(parameters[0].name == "10");
				REQUIRE(parameters[0].type == QueryEntityType::Line);
				REQUIRE(parameters[1].name == "v");
				REQUIRE(parameters[1].type == QueryEntityType::Variable);
			}
		}
	}
	SECTION("One valid Uses clause")
	{
		queue<Token> tokens;
		tokens.push(Token(TokenType::Identifier, "variable"));
		tokens.push(Token(TokenType::Identifier, "v"));
		tokens.push(Token(TokenType::Identifier, ";"));
		tokens.push(Token(TokenType::Identifier, "Select"));
		tokens.push(Token(TokenType::Identifier, "v"));
		tokens.push(Token(TokenType::Identifier, "such that"));
		tokens.push(Token(TokenType::Identifier, "Uses"));
		tokens.push(Token(TokenType::Identifier, "("));
		tokens.push(Token(TokenType::Identifier, "\""));
		tokens.push(Token(TokenType::Identifier, "proc1"));
		tokens.push(Token(TokenType::Identifier, "\""));
		tokens.push(Token(TokenType::Identifier, ","));
		tokens.push(Token(TokenType::Identifier, "\""));
		tokens.push(Token(TokenType::Identifier, "var1"));
		tokens.push(Token(TokenType::Identifier, "\""));
		tokens.push(Token(TokenType::Identifier, ")"));
		PQLParser p = PQLParser();
		Query q = p.buildQuery(tokens);
		std::vector<QueryEntity> selectors = q.selectors;
		std::vector<Clause> clauses = q.clauses;
		QueryEntity tar = q.target;
		WHEN("Successfully parsed:")
		{
			SECTION("selectors: one QueryEntity is inside") {
				REQUIRE(selectors.size() == 1);
				QueryEntity qe = selectors.front();
				REQUIRE(qe.name == "v");
				REQUIRE(qe.type == QueryEntityType::Variable);
			}
			SECTION("target")
			{
				REQUIRE(tar.name == "v");
				REQUIRE(tar.type == QueryEntityType::Variable);
			}
			SECTION("clauses: one UsesS clause is inside")
			{
				REQUIRE(clauses.size() == 1);
				Clause c = clauses.front();
				std::vector<QueryEntity> parameters = c.parameters;
				REQUIRE(c.clauseType == ClauseType::UsesS);
				REQUIRE(parameters[0].name == "proc1");
				REQUIRE(parameters[0].type == QueryEntityType::Name);
				REQUIRE(parameters[1].name == "var1");
				REQUIRE(parameters[1].type == QueryEntityType::Name);
			}
		}
	}
}

SCENARIO("Test pattern clause exact match") {
	queue<Token> tokens;
	tokens.push(Token(TokenType::Identifier, "assign"));
	tokens.push(Token(TokenType::Identifier, "a"));
	tokens.push(Token(TokenType::Identifier, ";"));
	tokens.push(Token(TokenType::Identifier, "variable"));
	tokens.push(Token(TokenType::Identifier, "v"));
	tokens.push(Token(TokenType::Identifier, ";"));
	tokens.push(Token(TokenType::Identifier, "Select"));
	tokens.push(Token(TokenType::Identifier, "a"));
	tokens.push(Token(TokenType::Identifier, "pattern"));
	tokens.push(Token(TokenType::Identifier, "a"));
	tokens.push(Token(TokenType::Identifier, "("));
	tokens.push(Token(TokenType::Identifier, "_"));
	tokens.push(Token(TokenType::Identifier, ","));
	tokens.push(Token(TokenType::Identifier, "\""));
	tokens.push(Token(TokenType::Identifier, "v+x * (y8y +z)* t"));
	tokens.push(Token(TokenType::Identifier, "\""));
	tokens.push(Token(TokenType::Identifier, ")"));
	tokens.push(Token(TokenType::Identifier, "such that"));
	tokens.push(Token(TokenType::Identifier, "Modifies"));
	tokens.push(Token(TokenType::Identifier, "("));
	tokens.push(Token(TokenType::Identifier, "a"));
	tokens.push(Token(TokenType::Identifier, ","));
	tokens.push(Token(TokenType::Identifier, "v"));
	tokens.push(Token(TokenType::Identifier, ")"));
	PQLParser p = PQLParser();
	Query q = p.buildQuery(tokens);
	std::vector<Clause> clauses = q.clauses;
	SECTION("clauses: one AssignPatt clause is inside")
	{
		REQUIRE(clauses.size() == 2);
		Clause c = clauses.front();
		std::vector<QueryEntity> parameters = c.parameters;
		REQUIRE(c.clauseType == ClauseType::AssignPatt);
		REQUIRE(parameters[0].name == "a");
		REQUIRE(parameters[0].type == QueryEntityType::Assign);
		REQUIRE(parameters[1].name == "_");
		REQUIRE(parameters[1].type == QueryEntityType::Underscore);
		REQUIRE(parameters[2].name == "vxy8yz+*t*+");
		REQUIRE(parameters[2].type == QueryEntityType::Expression);
	}
}

SCENARIO("Test pattern clause partial match") {
	queue<Token> tokens;
	tokens.push(Token(TokenType::Identifier, "assign"));
	tokens.push(Token(TokenType::Identifier, "a"));
	tokens.push(Token(TokenType::Identifier, ";"));
	tokens.push(Token(TokenType::Identifier, "variable"));
	tokens.push(Token(TokenType::Identifier, "v"));
	tokens.push(Token(TokenType::Identifier, ";"));
	tokens.push(Token(TokenType::Identifier, "Select"));
	tokens.push(Token(TokenType::Identifier, "a"));
	tokens.push(Token(TokenType::Identifier, "pattern"));
	tokens.push(Token(TokenType::Identifier, "a"));
	tokens.push(Token(TokenType::Identifier, "("));
	tokens.push(Token(TokenType::Identifier, "v"));
	tokens.push(Token(TokenType::Identifier, ","));
	tokens.push(Token(TokenType::Identifier, "_"));
	tokens.push(Token(TokenType::Identifier, "\""));
	tokens.push(Token(TokenType::Identifier, "(y+z)*t"));
	tokens.push(Token(TokenType::Identifier, "\""));
	tokens.push(Token(TokenType::Identifier, "_"));
	tokens.push(Token(TokenType::Identifier, ")"));
	PQLParser p = PQLParser();
	Query q = p.buildQuery(tokens);
	std::vector<Clause> clauses = q.clauses;
	SECTION("clauses: one AssignPatt clause is inside")
	{
		REQUIRE(clauses.size() == 1);
		Clause c = clauses.front();
		std::vector<QueryEntity> parameters = c.parameters;
		REQUIRE(c.clauseType == ClauseType::AssignPatt);
		REQUIRE(parameters[0].name == "a");
		REQUIRE(parameters[0].type == QueryEntityType::Assign);
		REQUIRE(parameters[1].name == "v");
		REQUIRE(parameters[1].type == QueryEntityType::Variable);
		REQUIRE(parameters[2].name == "_yz+t*_");
		REQUIRE(parameters[2].type == QueryEntityType::Expression);
	}
}

SCENARIO("Test pattern clause underscore") {
	queue<Token> tokens;
	tokens.push(Token(TokenType::Identifier, "assign"));
	tokens.push(Token(TokenType::Identifier, "a"));
	tokens.push(Token(TokenType::Identifier, ";"));
	tokens.push(Token(TokenType::Identifier, "variable"));
	tokens.push(Token(TokenType::Identifier, "v"));
	tokens.push(Token(TokenType::Identifier, ";"));
	tokens.push(Token(TokenType::Identifier, "Select"));
	tokens.push(Token(TokenType::Identifier, "a"));
	tokens.push(Token(TokenType::Identifier, "pattern"));
	tokens.push(Token(TokenType::Identifier, "a"));
	tokens.push(Token(TokenType::Identifier, "("));
	tokens.push(Token(TokenType::Identifier, "v"));
	tokens.push(Token(TokenType::Identifier, ","));
	tokens.push(Token(TokenType::Identifier, "_"));
	tokens.push(Token(TokenType::Identifier, ")"));
	PQLParser p = PQLParser();
	Query q = p.buildQuery(tokens);
	std::vector<Clause> clauses = q.clauses;
	SECTION("clauses: one AssignPatt clause is inside")
	{
		REQUIRE(clauses.size() == 1);
		Clause c = clauses.front();
		std::vector<QueryEntity> parameters = c.parameters;
		REQUIRE(c.clauseType == ClauseType::AssignPatt);
		REQUIRE(parameters[0].name == "a");
		REQUIRE(parameters[0].type == QueryEntityType::Assign);
		REQUIRE(parameters[1].name == "v");
		REQUIRE(parameters[1].type == QueryEntityType::Variable);
		REQUIRE(parameters[2].name == "_");
		REQUIRE(parameters[2].type == QueryEntityType::Expression);
	}
}

// Scenarios that will throw exceptions
SCENARIO("select nothing") {
	queue<Token> tokens;
	tokens.push(Token(TokenType::Identifier, "variable"));
	tokens.push(Token(TokenType::Identifier, "a"));
	tokens.push(Token(TokenType::Identifier, ";"));
	tokens.push(Token(TokenType::Identifier, "Select"));
	PQLParser p = PQLParser();
	REQUIRE_THROWS_WITH(p.buildQuery(tokens), "Token Queue is empty!");
}

SCENARIO("select a entity that is not declared") {
	SECTION("in the Select target")
	{
		queue<Token> tokens;
		tokens.push(Token(TokenType::Identifier, "variable"));
		tokens.push(Token(TokenType::Identifier, "a"));
		tokens.push(Token(TokenType::Identifier, ";"));
		tokens.push(Token(TokenType::Identifier, "Select"));
		tokens.push(Token(TokenType::Identifier, "b"));
		PQLParser p = PQLParser();
		REQUIRE_THROWS_WITH(p.buildQuery(tokens), "No matched synonym have been declared.");
	}
	SECTION("in the such that clause")
	{
		queue<Token> tokens;
		tokens.push(Token(TokenType::Identifier, "stmt"));
		tokens.push(Token(TokenType::Identifier, "s"));
		tokens.push(Token(TokenType::Identifier, ";"));
		tokens.push(Token(TokenType::Identifier, "Select"));
		tokens.push(Token(TokenType::Identifier, "s"));
		tokens.push(Token(TokenType::Identifier, "such that"));
		tokens.push(Token(TokenType::Identifier, "Parent*"));
		tokens.push(Token(TokenType::Identifier, "("));
		tokens.push(Token(TokenType::Identifier, "p"));
		tokens.push(Token(TokenType::Identifier, ","));
		tokens.push(Token(TokenType::Identifier, "_"));
		tokens.push(Token(TokenType::Identifier, ")"));
		PQLParser p = PQLParser();
		REQUIRE_THROWS_WITH(p.buildQuery(tokens), "No matched synonym have been declared.");
	}
	SECTION("in the pattern clause")
	{
		queue<Token> tokens;
		tokens.push(Token(TokenType::Identifier, "assign"));
		tokens.push(Token(TokenType::Identifier, "a"));
		tokens.push(Token(TokenType::Identifier, ";"));
		tokens.push(Token(TokenType::Identifier, "Select"));
		tokens.push(Token(TokenType::Identifier, "a"));
		tokens.push(Token(TokenType::Identifier, "pattern"));
		tokens.push(Token(TokenType::Identifier, "a1"));
		tokens.push(Token(TokenType::Identifier, "("));
		tokens.push(Token(TokenType::Identifier, "_"));
		tokens.push(Token(TokenType::Identifier, ","));
		tokens.push(Token(TokenType::Identifier, "_"));
		tokens.push(Token(TokenType::Identifier, ")"));
		PQLParser p = PQLParser();
		REQUIRE_THROWS_WITH(p.buildQuery(tokens), "No matched synonym have been declared.");
	}
}

SCENARIO("have underscore as the first argument for Modifies") {
	queue<Token> tokens;
	tokens.push(Token(TokenType::Identifier, "assign"));
	tokens.push(Token(TokenType::Identifier, "a"));
	tokens.push(Token(TokenType::Identifier, ";"));
	tokens.push(Token(TokenType::Identifier, "variable"));
	tokens.push(Token(TokenType::Identifier, "v"));
	tokens.push(Token(TokenType::Identifier, ";"));
	tokens.push(Token(TokenType::Identifier, "Select"));
	tokens.push(Token(TokenType::Identifier, "a"));
	tokens.push(Token(TokenType::Identifier, "such that"));
	tokens.push(Token(TokenType::Identifier, "Modifies"));
	tokens.push(Token(TokenType::Identifier, "("));
	tokens.push(Token(TokenType::Identifier, "_"));
	tokens.push(Token(TokenType::Identifier, ","));
	tokens.push(Token(TokenType::Identifier, "v"));
	tokens.push(Token(TokenType::Identifier, ")"));
	PQLParser p = PQLParser();
	REQUIRE_THROWS_WITH(p.buildQuery(tokens),
		"semantically invalid to have underscore as the first argument for Modifies");
}

SCENARIO("have constant synonym as the argument for clauses") {
	queue<Token> tokens;
	tokens.push(Token(TokenType::Identifier, "constant"));
	tokens.push(Token(TokenType::Identifier, "c"));
	tokens.push(Token(TokenType::Identifier, ";"));
	tokens.push(Token(TokenType::Identifier, "Select"));
	tokens.push(Token(TokenType::Identifier, "c"));
	tokens.push(Token(TokenType::Identifier, "such that"));
	tokens.push(Token(TokenType::Identifier, "Uses"));
	tokens.push(Token(TokenType::Identifier, "("));
	tokens.push(Token(TokenType::Identifier, "\""));
	tokens.push(Token(TokenType::Identifier, "proc1"));
	tokens.push(Token(TokenType::Identifier, "\""));
	tokens.push(Token(TokenType::Identifier, ","));
	tokens.push(Token(TokenType::Identifier, "c"));
	tokens.push(Token(TokenType::Identifier, ")"));
	PQLParser p = PQLParser();
	REQUIRE_THROWS_WITH(p.buildQuery(tokens), "invalid to have constant synonym as the argument for clauses");
}

SCENARIO("have procedure name as the first argument for Parent") {
	queue<Token> tokens;
	tokens.push(Token(TokenType::Identifier, "procedure"));
	tokens.push(Token(TokenType::Identifier, "p"));
	tokens.push(Token(TokenType::Identifier, ";"));
	tokens.push(Token(TokenType::Identifier, "Select"));
	tokens.push(Token(TokenType::Identifier, "p"));
	tokens.push(Token(TokenType::Identifier, "such that"));
	tokens.push(Token(TokenType::Identifier, "Parent"));
	tokens.push(Token(TokenType::Identifier, "("));
	tokens.push(Token(TokenType::Identifier, "\""));
	tokens.push(Token(TokenType::Identifier, "p"));
	tokens.push(Token(TokenType::Identifier, "\""));
	tokens.push(Token(TokenType::Identifier, ","));
	tokens.push(Token(TokenType::Identifier, "6"));
	tokens.push(Token(TokenType::Identifier, ")"));
	PQLParser p = PQLParser();
	REQUIRE_THROWS_WITH(p.buildQuery(tokens), "invalid argument combination for clauses");
}

SCENARIO("no token matched for one expect token") {
	SECTION("no match for '('")
	{
		queue<Token> tokens;
		tokens.push(Token(TokenType::Identifier, "procedure"));
		tokens.push(Token(TokenType::Identifier, "p"));
		tokens.push(Token(TokenType::Identifier, ";"));
		tokens.push(Token(TokenType::Identifier, "Select"));
		tokens.push(Token(TokenType::Identifier, "p"));
		tokens.push(Token(TokenType::Identifier, "such that"));
		tokens.push(Token(TokenType::Identifier, "Follows"));
		tokens.push(Token(TokenType::Identifier, "\""));
		tokens.push(Token(TokenType::Identifier, "p"));
		tokens.push(Token(TokenType::Identifier, "\""));
		tokens.push(Token(TokenType::Identifier, ","));
		tokens.push(Token(TokenType::Identifier, "6"));
		tokens.push(Token(TokenType::Identifier, ")"));
		PQLParser p = PQLParser();
		REQUIRE_THROWS_WITH(p.buildQuery(tokens), "Expected '(' but got '\"'");
	}
	SECTION("no match for ';'")
	{
		queue<Token> tokens;
		tokens.push(Token(TokenType::Identifier, "procedure"));
		tokens.push(Token(TokenType::Identifier, "p"));
		tokens.push(Token(TokenType::Identifier, "Select"));
		tokens.push(Token(TokenType::Identifier, "p"));
		tokens.push(Token(TokenType::Identifier, "such that"));
		tokens.push(Token(TokenType::Identifier, "Follows"));
		tokens.push(Token(TokenType::Identifier, "("));
		tokens.push(Token(TokenType::Identifier, "\""));
		tokens.push(Token(TokenType::Identifier, "p"));
		tokens.push(Token(TokenType::Identifier, "\""));
		tokens.push(Token(TokenType::Identifier, ","));
		tokens.push(Token(TokenType::Identifier, "6"));
		tokens.push(Token(TokenType::Identifier, ")"));
		PQLParser p = PQLParser();
		REQUIRE_THROWS_WITH(p.buildQuery(tokens), "Expected ';' but got 'Select'");
	}
}

SCENARIO("no token matched in a list of expect tokens") {
	queue<Token> tokens;
	tokens.push(Token(TokenType::Identifier, "p"));
	tokens.push(Token(TokenType::Identifier, ";"));
	tokens.push(Token(TokenType::Identifier, "Select"));
	tokens.push(Token(TokenType::Identifier, "p"));
	tokens.push(Token(TokenType::Identifier, "such that"));
	tokens.push(Token(TokenType::Identifier, "Follows"));
	tokens.push(Token(TokenType::Identifier, "("));
	tokens.push(Token(TokenType::Identifier, "\""));
	tokens.push(Token(TokenType::Identifier, "p"));
	tokens.push(Token(TokenType::Identifier, "\""));
	tokens.push(Token(TokenType::Identifier, ","));
	tokens.push(Token(TokenType::Identifier, "6"));
	tokens.push(Token(TokenType::Identifier, ")"));
	PQLParser p = PQLParser();
	REQUIRE_THROWS_WITH(p.buildQuery(tokens), "Expected query entity type but got 'p'");
}

SCENARIO("syn-assign is not assignment synonym in pattern clause")
{
	queue<Token> tokens;
	tokens.push(Token(TokenType::Identifier, "variable"));
	tokens.push(Token(TokenType::Identifier, "v"));
	tokens.push(Token(TokenType::Identifier, ";"));
	tokens.push(Token(TokenType::Identifier, "Select"));
	tokens.push(Token(TokenType::Identifier, "v"));
	tokens.push(Token(TokenType::Identifier, "pattern"));
	tokens.push(Token(TokenType::Identifier, "v"));
	tokens.push(Token(TokenType::Identifier, "("));
	tokens.push(Token(TokenType::Identifier, "_"));
	tokens.push(Token(TokenType::Identifier, ","));
	tokens.push(Token(TokenType::Identifier, "_"));
	tokens.push(Token(TokenType::Identifier, ")"));
	PQLParser p = PQLParser();
	REQUIRE_THROWS_WITH(p.buildQuery(tokens), "syn-assign must be declared as synonym of assignment for pattern");
}

SCENARIO("wrong query entity type for the fisrt parameter of pattern")
{
	queue<Token> tokens;
	tokens.push(Token(TokenType::Identifier, "assign"));
	tokens.push(Token(TokenType::Identifier, "a1"));
	tokens.push(Token(TokenType::Identifier, ";"));
	tokens.push(Token(TokenType::Identifier, "procedure"));
	tokens.push(Token(TokenType::Identifier, "p"));
	tokens.push(Token(TokenType::Identifier, ";"));
	tokens.push(Token(TokenType::Identifier, "Select"));
	tokens.push(Token(TokenType::Identifier, "p"));
	tokens.push(Token(TokenType::Identifier, "pattern"));
	tokens.push(Token(TokenType::Identifier, "a1"));
	tokens.push(Token(TokenType::Identifier, "("));
	tokens.push(Token(TokenType::Identifier, "p"));
	tokens.push(Token(TokenType::Identifier, ","));
	tokens.push(Token(TokenType::Identifier, "\""));
	tokens.push(Token(TokenType::Identifier, "v+ 3"));
	tokens.push(Token(TokenType::Identifier, "\""));
	tokens.push(Token(TokenType::Identifier, ")"));
	PQLParser p = PQLParser();
	REQUIRE_THROWS_WITH(p.buildQuery(tokens), "Invalid query entity type for fisrt parameter of pattern.");
}