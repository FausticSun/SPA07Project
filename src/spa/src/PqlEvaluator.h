#pragma once

#include<stdio.h>
#include<PKB.h>
#include<PQLParser.h>
#include <iostream>
#include <string>
#include <vector>
#include<list>

using namespace std;

typedef string QueryTokens;
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

class PqlEvaluator {
	public:
		PqlEvaluator(const PKB& pkb);
		~PqlEvaluator();
		list<string> evaluateQuery(string input);

	private:
		PKB mypkb;

};