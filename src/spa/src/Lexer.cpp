#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <vector>

using namespace std;

#include "Lexer.h"
#include "PKB.h"

Lexer::Lexer() { int statementLine = 0; }

Lexer::~Lexer() {}

queue<Token> Lexer::tokenizeFile(string filePath) {
  ifstream inputFile;
  inputFile.open(filePath);
  if (!inputFile) {
    throw invalid_argument("FileNotFoundException: " + filePath);
  }
  queue<Token> allTokens;
  string line;
  statementLine = 0;
  while (getline(inputFile, line)) {
    // to be implemented
  }
  inputFile.close();
  return allTokens;
}

vector<Token> Lexer::tokenize(string input) {

  input.erase(remove_if(input.begin(), input.end(),
                        static_cast<int (&)(int)>(std::isspace)),
              input.end());
  vector<string> tokens = vectorize(input);

  // identifying different types of statement
  if (find(tokens.begin(), tokens.end(), "procedure") != tokens.end()) {
    statementLine = 0;
    return tokenizeProcedure(tokens);
  } else if (find(tokens.begin(), tokens.end(), "=") != tokens.end()) {
    statementLine++;
    return tokenizeAssignment(tokens);
  } else if (find(tokens.begin(), tokens.end(), "read") != tokens.end()) {
    statementLine++;
    return tokenizeRead(tokens);
  } else if (find(tokens.begin(), tokens.end(), "print") != tokens.end()) {
    statementLine++;
    return tokenizePrint(tokens);
  } else {
    throw "Invalid statement";
  }
}
vector<string> Lexer::vectorize(string input) {
  vector<string> tokens;
  string curr = "";
  for (char &c : input) {
    string temp;
    temp.push_back(c);
    if (isOperator(temp) || isSeparator(temp)) {
      if (curr.length() != 0) {
        tokens.push_back(curr);
        curr = "";
      }
      tokens.push_back(temp);
    } else if (curr == "procedure" || curr == "read" || curr == "print") {
      tokens.push_back(curr);
      curr = temp;
    } else {
      curr.push_back(c);
    }
  }

  return tokens;
}

vector<Token> Lexer::tokenizeProcedure(vector<string> tokens) {
  vector<Token> toAST;
  toAST.push_back(Token(TokenType::Procedure, "procedure"));
  toAST.push_back(pushIdentifier(tokens[1]));
  toAST.push_back(Token(TokenType::Separator, "{"));

  return toAST;
}

vector<Token> Lexer::tokenizeAssignment(vector<string> tokens) {
  vector<Token> toAST;
  toAST.push_back(Token(TokenType::Assign, "assign"));

  for (auto i : tokens) {
    toAST.push_back(pushToken(i));
  }

  return toAST;
}

vector<Token> Lexer::tokenizeRead(vector<string> tokens) {
  vector<Token> toAST;
  toAST.push_back(Token(TokenType::Read, "read"));
  tokens.erase(tokens.begin());

  for (auto i : tokens) {
    toAST.push_back(pushToken(i));
  }

  return toAST;
}

vector<Token> Lexer::tokenizePrint(vector<string> tokens) {
  vector<Token> toAST;
  toAST.push_back(Token(TokenType::Print, "print"));
  tokens.erase(tokens.begin());

  for (auto i : tokens) {
    toAST.push_back(pushToken(i));
  }

  return toAST;
}

Token Lexer::pushToken(string s) {
  if (isConstant(s)) {
    return Token(TokenType::Constant, s);
  } else if (isSeparator(s)) {
    return Token(TokenType::Separator, s);
  } else if (isOperator(s)) {
    return pushOperator(s);
  } else if (isIdentifier(s)) {
    return Token(TokenType::Identifier, s);
  } else {
    throw "Invalid Identifier";
  }
}

Token Lexer::pushIdentifier(string s) {

  if (isIdentifier(s)) {
    return Token(TokenType::Identifier, s);
  } else {
    throw "Invalid Identifier";
  }
}

Token Lexer::pushOperator(string s) {
  if (s == "+") {
    return Token(TokenType::Plus, s);
  } else if (s == "-") {
    return Token(TokenType::Minus, s);
  } else if (s == "*") {
    return Token(TokenType::Multiply, s);
  } else if (s == "/") {
    return Token(TokenType::Divide, s);
  } else if (s == "=") {
    return Token(TokenType::Equal, s);
  } else if (s == "(") {
    return Token(TokenType::OpenParenthesis, s);
  } else if (s == ")") {
    return Token(TokenType::CloseParenthesis, s);
  }
}

bool Lexer::isIdentifier(string s) {
  if (isConstant(s.substr(0))) {
    return false;
  } else {
    return (!onlyContainDigits(s));
  }
}

bool Lexer::isSeparator(string s) { return (s == ";" || s == "{" || s == "}"); }

bool Lexer::isOperator(string s) {
  return (s == "+" || s == "-" || s == "*" || s == "/" || s == "=" ||
          s == "(" || s == ")");
}

bool Lexer::isConstant(string s) {
  if (s == "true" || s == "false") {
    return true;
  } else if (s[0] == '"') {
    return true;
  } else {
    return onlyContainDigits(s);
  }
}

bool Lexer::onlyContainDigits(string s) {
  if (s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) {
    return false;
  }

  char *p;
  strtol(s.c_str(), &p, 10);

  return (*p == 0);
}
