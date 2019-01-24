#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

#include "PKB.h"
#include "TNode.h"

VarTable::VarTable() {
	numOfVars = 0;
}

int VarTable::add(string s) {
	varList.insert(s);
	numOfVars++;
	return numOfVars;
}

bool VarTable::contains(string s) {
	unordered_set<string>::const_iterator got = varList.find(s);
	return got != varList.end() ? true : false;
}

string VarTable::getAll() {
	string result = "";
	for (unordered_set<string>::iterator it = varList.begin(); it != varList.end(); ++it)
		result += *it + ", ";
	if (!result.empty()) {
		result.pop_back();
		result.pop_back();
	}
	return result;
}

ProcTable::ProcTable() {
	numOfProcs = 0;
}

int ProcTable::add(string s) {
	procList.insert(s);
	numOfProcs++;
	return numOfProcs;
}

bool ProcTable::contains(string s) {
	unordered_set<string>::const_iterator got = procList.find(s);
	return got != procList.end() ? true : false;
}

string ProcTable::getAll() {
	string result = "";
	for (unordered_set<string>::iterator it = procList.begin(); it != procList.end(); ++it)
		result += *it + ", ";
	if (!result.empty()) {
		result.pop_back();
		result.pop_back();
	}
	return result;
}

int PKB::setProcToAST(PROC p, TNode* r) {
	return NULL;
}

TNode* PKB::getRootAST (PROC p){

	return NULL;
}
