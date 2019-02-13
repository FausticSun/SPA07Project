#include "PQLParser.h"
#include <algorithm>
#include <queue>
#include <string.h>
#include <string>
#include <vector>

using namespace std;

PQLParser::PQLParser(string input) { Tokenize(input); }

void PQLParser::Tokenize(string input) {
  vector<string> token = vectorize(input);
  while (0 < token.size()) {
    if (find(token.begin(), token.end(), "procedure") != token.end()) {
      tokenizeProcedure(token);
      token.erase(token.begin());
      token.erase(token.begin());
    }

    if (find(token.begin(), token.end(), "variable") != token.end()) {
      tokenizeVariable(token);
      token.erase(token.begin());
      token.erase(token.begin());
    }

    if (find(token.begin(), token.end(), "stmt") != token.end()) {
      tokenizeStmt(token);
      token.erase(token.begin());
      token.erase(token.begin());
    }

    if (find(token.begin(), token.end(), "read") != token.end()) {
      tokenizeRead(token);
      token.erase(token.begin());
      token.erase(token.begin());
    }

    if (find(token.begin(), token.end(), "print") != token.end()) {
      tokenizePrint(token);
      token.erase(token.begin());
      token.erase(token.begin());
    }

    if (find(token.begin(), token.end(), "while") != token.end()) {
      tokenizeWhile(token);
      token.erase(token.begin());
      token.erase(token.begin());
    }

    if (find(token.begin(), token.end(), "if") != token.end()) {
      tokenizeIf(token);
      token.erase(token.begin());
      token.erase(token.begin());
    }

    if (find(token.begin(), token.end(), "call") != token.end()) {
      tokenizeCall(token);
      token.erase(token.begin());
      token.erase(token.begin());
    }

    if (find(token.begin(), token.end(), "assign") != token.end()) {
      tokenizeAssign(token);
      token.erase(token.begin());
      token.erase(token.begin());
    }

    if (find(token.begin(), token.end(), "constant") != token.end()) {
      tokenizeConstant(token);
      token.erase(token.begin());
      token.erase(token.begin());
    }

    // if (find(token.begin(), token.end(), "select") != token.end()) {
    if (token[0] == "select") {
      tokenizeSelect(token);
      token.erase(token.begin());
      token.erase(token.begin());
      if (token.size() > 0) {
        if (token[0] == "such" && token[1] == "that") {
          token.erase(token.begin());
          token.erase(token.begin());
          while (0 < token.size()) {
            if (find(token.begin(), token.end(), "Modifies") != token.end()) {
              tokenizeModifies(token);
              token.erase(token.begin());
              token.erase(token.begin());
              token.erase(token.begin());
            }

            if (find(token.begin(), token.end(), "Uses") != token.end()) {
              tokenizeUses(token);
              token.erase(token.begin());
              token.erase(token.begin());
              token.erase(token.begin());
            }

            if (find(token.begin(), token.end(), "Parent*") != token.end()) {
              tokenizeParentT(token);
              token.erase(token.begin());
              token.erase(token.begin());
              token.erase(token.begin());
            } else if (find(token.begin(), token.end(), "Parent") !=
                       token.end()) {
              tokenizeParent(token);
              token.erase(token.begin());
              token.erase(token.begin());
              token.erase(token.begin());
            }

            if (find(token.begin(), token.end(), "Follows*") != token.end()) {
              tokenizeFollowsT(token);
              token.erase(token.begin());
              token.erase(token.begin());
              token.erase(token.begin());
            } else if (find(token.begin(), token.end(), "Follows") !=
                       token.end()) {
              tokenizeFollows(token);
              token.erase(token.begin());
              token.erase(token.begin());
              token.erase(token.begin());
            }
          }
        }
      }
    }
  }

  if (find(token.begin(), token.end(), "variable") != token.end()) {
    tokenizeVariable(token);
    token.erase(token.begin());
    token.erase(token.begin());
  }

  if (find(token.begin(), token.end(), "select") != token.end()) {
    tokenizeSelect(token);
    token.erase(token.begin());
    token.erase(token.begin());
  }
}
vector<string> PQLParser::vectorize(string) { return vector<string>(); }
bool isInt(string s) {
  if (s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+')))
    return false;

  char *p;
  strtol(s.c_str(), &p, 10);

  return (*p == 0);
}

queue<pair<DeclarationType, string>> PQLParser::getDeclarationQueue() {
  return declarationQueue;
}

queue<tuple<RelationType, string, string>> PQLParser::getSelectQueue() {
  return selectQueue;
}

string PQLParser::getTarget() { return target; }

void PQLParser::tokenizeVariable(vector<string> token) {
  declarationQueue.push(make_pair(DeclarationType::Variable, token[1]));
}

void PQLParser::tokenizeProcedure(vector<string> token) {
  declarationQueue.push(make_pair(DeclarationType::Procedure, token[1]));
}

void PQLParser::tokenizeRead(vector<string> token) {
  declarationQueue.push(make_pair(DeclarationType::Read, token[1]));
}

void PQLParser::tokenizePrint(vector<string> token) {
  declarationQueue.push(make_pair(DeclarationType::Print, token[1]));
}

void PQLParser::tokenizeWhile(vector<string> token) {
  declarationQueue.push(make_pair(DeclarationType::While, token[1]));
}

void PQLParser::tokenizeIf(vector<string> token) {
  declarationQueue.push(make_pair(DeclarationType::If, token[1]));
}

void PQLParser::tokenizeAssign(vector<string> token) {
  declarationQueue.push(make_pair(DeclarationType::Assign, token[1]));
}

void PQLParser::tokenizeStmt(vector<string> token) {
  declarationQueue.push(make_pair(DeclarationType::Stmt, token[1]));
}

void PQLParser::tokenizeCall(vector<string> token) {
  declarationQueue.push(make_pair(DeclarationType::Call, token[1]));
}

void PQLParser::tokenizeConstant(vector<string> token) {
  declarationQueue.push(make_pair(DeclarationType::Constant, token[1]));
}

void PQLParser::tokenizeSelect(vector<string> token) { target = token[1]; }

void PQLParser::tokenizePattern(vector<string>) {}

void PQLParser::tokenizeFollows(vector<string> token) {
  selectQueue.push(make_tuple(RelationType::Follows, token[1], token[2]));
}

void PQLParser::tokenizeFollowsT(vector<string> token) {
  selectQueue.push(make_tuple(RelationType::FollowsT, token[1], token[2]));
}

void PQLParser::tokenizeParent(vector<string> token) {
  selectQueue.push(make_tuple(RelationType::Parent, token[1], token[2]));
}

void PQLParser::tokenizeParentT(vector<string> token) {
  selectQueue.push(make_tuple(RelationType::ParentT, token[1], token[2]));
}

void PQLParser::tokenizeUses(vector<string> token) {
  if (isInt(token[0])) {
    selectQueue.push(make_tuple(RelationType::UsesS, token[1], token[2]));
  } else {
    selectQueue.push(make_tuple(RelationType::UsesP, token[1], token[2]));
  }
}

void PQLParser::tokenizeModifies(vector<string> token) {
  if (isInt(token[0])) {
    selectQueue.push(make_tuple(RelationType::ModifiesS, token[1], token[2]));
  } else {
    selectQueue.push(make_tuple(RelationType::ModifiesP, token[1], token[2]));
  }
}
