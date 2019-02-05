#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <queue>
#include <Token.h>

using namespace std;
typedef short PROC;

class TNode;

class VarTable {  // no need to #include "VarTable.h" as all I need is pointer
public:
	VarTable();
	VarTable* buildVarTable(queue<Token*> tokens);
	bool contains(string s);
	unordered_set<string> getVarList();
 	string toString();
	int add(string s); //for testing

private:
	unordered_set<string> varList;
	int numOfVars;
};

class ProcTable {
public:
	ProcTable();
	ProcTable* buildProcTable(queue<Token*> tokens);
	bool contains(string s);
	unordered_set<string> getProcList();
	string toString();

private:
	unordered_set<string> procList;
	int numOfProcs;
	int add(string s);
};

class PKB {
public:
	static VarTable* varTable; 
	static ProcTable* procTable;
	static int setProcToAST(PROC p, TNode* r);
	static TNode* getRootAST (PROC p);

};