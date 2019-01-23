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
	varList.push_back(s);
	numOfVars++;
}

bool VarTable::contains(string s) {
	return std::find(varList.begin(), varList.end(), s) != varList.end() ? true : false;
}

string VarTable::getAll() {
	string result = "";
	for (std::list<string>::iterator it = varList.begin(); it != varList.end(); ++it)
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
	procList.push_back(s);
	numOfProcs++;
}

bool ProcTable::contains(string s) {
	return std::find(procList.begin(), procList.end(), s) != procList.end() ? true : false;
}

string ProcTable::getAll() {
	string result = "";
	for (std::list<string>::iterator it = procList.begin(); it != procList.end(); ++it)
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
