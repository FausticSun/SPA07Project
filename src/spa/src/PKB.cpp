#include "PKB.h"

VarTable::VarTable() { numOfVars = 0; }

VarTable *VarTable::buildVarTable(queue<Token *> tokens) {
  try {
    string previous = "";
    while (!tokens.empty()) {
      Token *token = tokens.front();
      if (token->type == TokenType::Identifier && previous != "procedure") {
        add(token->name);
      }
      previous = token->name;
      tokens.pop();
    }
    return this;
  } catch (exception &e) {
    ;
  }
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

unordered_set<string> VarTable::getVarList() { return varList; }

string VarTable::toString() {
  string result = "";
  for (unordered_set<string>::iterator it = varList.begin();
       it != varList.end(); ++it)
    result += *it + ", ";
  if (!result.empty()) {
    result.pop_back();
    result.pop_back();
  }
  return result;
}

ProcTable::ProcTable() { numOfProcs = 0; }

ProcTable *ProcTable::buildProcTable(queue<Token *> tokens) {
  try {
    string previous = "";
    while (!tokens.empty()) {
      Token *token = tokens.front();
      if (token->type == TokenType::Identifier && previous == "procedure") {
        add(token->name);
      }
      previous = token->name;
      tokens.pop();
    }
    return this;
  } catch (exception &e) {
    ;
  }
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

unordered_set<string> ProcTable::getProcList() { return procList; }

string ProcTable::toString() {
  string result = "";
  for (unordered_set<string>::iterator it = procList.begin();
       it != procList.end(); ++it)
    result += *it + ", ";
  if (!result.empty()) {
    result.pop_back();
    result.pop_back();
  }
  return result;
}

PKB::PKB(const std::set<std::string> &varTable,
         const std::set<std::string> &procTable,
         const std::set<Relation> &relTable)
    : varTable(varTable), procTable(procTable), relTable(relTable){};
const std::set<std::string> PKB::getVarTable() const { return this->varTable; }
const std::set<std::string> PKB::getProcTable() const {
  return this->procTable;
}
const std::set<Relation> PKB::getRelTable() const { return this->relTable; }
