#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>

using namespace std;
typedef short PROC;

class TNode;

class VarTable {  // no need to #include "VarTable.h" as all I need is pointer
public:
	unordered_set<string> varList;
	int numOfVars;
	VarTable();
	int add(string s);
	bool contains(string s);
	string getAll();
};

class ProcTable {
public:
	unordered_set<string> procList;
	int numOfProcs;
	ProcTable();
	int add(string s);
	bool contains(string s);
	string getAll();
};

class PKB {
public:
	static VarTable* varTable; 
	static ProcTable* procTable;
	static int setProcToAST(PROC p, TNode* r);
	static TNode* getRootAST (PROC p);

};