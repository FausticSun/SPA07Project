#pragma once

#include<stdio.h>
#include<PKB.h>
#include <iostream>
#include <string>
#include <vector>
#include<list>

using namespace std;

typedef string QueryTokens;
typedef string Query;
typedef string Clause;

enum class DesignEntityType {
	Stmt,
	Read,
	Print,
	Call,
	While,
	If,
	Assign,
	Variable,
	Constant,
	Procedure,
};
enum class RelationshipType {
	Follows,
	FollowsT,
	Parent,
	Uses,
	UsesP,
	ModifiesS,
	ModifiesP,
};

class Query {
	public:
		Query();
		~Query();
		list<string> DesignEntities;
		string targetDesignEntities;
		Clause relClause;
		Clause patClause;
};

class PqlEvaluator {
	public:
		
		PqlEvaluator();
		~PqlEvaluator();
		QueryTokens getParsedQuery();
		int getQueryCount();
		Query evaluateQuery(QueryTokens query);

	private:
		list<QueryTokens> queries;
		int queryCount;
		Query evaluateSimpleQuery(DesignEntityType de);
		bool isSimpleQuery(QueryTokens query);

};