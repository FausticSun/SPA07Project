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

Lexer::Lexer() {}

Lexer::~Lexer() {}

queue<Token> Lexer::tokenizeFile(string filePath) {
  ifstream inputFile;
  inputFile.open(filePath);
  if (!inputFile) {
    throw invalid_argument("FileNotFoundException: " + filePath);
  }
  queue<Token> allTokensInFile;
  string line;
  while (getline(inputFile, line)) {
    vector<Token> allTokensInLine = tokenize(line);
    for (Token i : allTokensInLine) {
      allTokensInFile.push(i);
    }
  }
  inputFile.close();
  return allTokensInFile;
}

vector<Token> Lexer::tokenize(string input) {

  vector<string> tokens = vectorize(input);

  // identifying different types of statement
  if (find(tokens.begin(), tokens.end(), "procedure") != tokens.end()) {
    return tokenizeProcedure(tokens);
  } else if (find(tokens.begin(), tokens.end(), "=") != tokens.end()) {
    return tokenizeAssignment(tokens);
  } else if (find(tokens.begin(), tokens.end(), "read") != tokens.end()) {
    return tokenizeRead(tokens);
  } else if (find(tokens.begin(), tokens.end(), "print") != tokens.end()) {
    return tokenizePrint(tokens);
  } else if (find(tokens.begin(), tokens.end(), "if") != tokens.end()) {
    return tokenizeIf(tokens);
  } else if (find(tokens.begin(), tokens.end(), "while") != tokens.end()) {
    return tokenizeWhile(tokens);
  } else {
    throw "Invalid statement";
  }
}
vector<string> Lexer::vectorize(string input) {
  vector<string> tokens;
  queue<string> current;

  for (char &c : input) {
    string temp;
    temp.push_back(c);

    if (isspace(c)) {
      if (!current.empty()) {
        tokens.push_back(convertQueueToString(current));
        queue<string> empty;
        swap(current, empty);
      }
    } else if (isSeparator(temp)) {
      if (!current.empty()) {
        tokens.push_back(convertQueueToString(current));
        queue<string> empty;
        swap(current, empty);
      }

      tokens.push_back(temp);
    } else if (isOperator(temp)) { // current queue only contains Operator or
                                   // Letter/Number
      if (!current.empty()) {
        if (!isOperator(current.back())) { // queue currently stores variable
          tokens.push_back(convertQueueToString(current));
          queue<string> empty;
          swap(current, empty);
        }
      }
      current.push(temp);
    } else { // is either letter or digit
      if (!current.empty()) {
        if (isOperator(current.back())) { // queue currently stores operators
          tokens.push_back(convertQueueToString(current));
          queue<string> empty;
          swap(current, empty);
        }
      }
      current.push(temp);
    }
  }
  for (auto i : tokens) {
    cout << i << " ";
  }
  return tokens;
}

string Lexer::convertQueueToString(queue<string> q) {
  string result;
  while (!q.empty()) {
    result += q.front();
    q.pop();
  }

  return result;
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

  for (auto i : tokens) {
    toAST.push_back(getToken(i));
  }

  return toAST;
}

vector<Token> Lexer::tokenizeRead(vector<string> tokens) {
  vector<Token> toAST;
  toAST.push_back(Token(TokenType::Read, "read"));
  tokens.erase(tokens.begin());

  for (auto i : tokens) {
    toAST.push_back(getToken(i));
  }

  return toAST;
}

vector<Token> Lexer::tokenizePrint(vector<string> tokens) {
  vector<Token> toAST;
  toAST.push_back(Token(TokenType::Print, "print"));
  tokens.erase(tokens.begin());

  for (auto i : tokens) {
    toAST.push_back(getToken(i));
  }

  return toAST;
}

vector<Token> Lexer::tokenizeIf(vector<string> tokens) {
  vector<Token> toAST;
  toAST.push_back(Token(TokenType::If, "if"));
  tokens.erase(tokens.begin());

  for (auto i : tokens) {
    toAST.push_back(getToken(i));
  }

  return toAST;
}

vector<Token> Lexer::tokenizeWhile(vector<string> tokens) {
  vector<Token> toAST;
  toAST.push_back(Token(TokenType::While, "while"));
  tokens.erase(tokens.begin());

  for (auto i : tokens) {
    toAST.push_back(getToken(i));
  }

  return toAST;
}

Token Lexer::getToken(string s) {
  if (isConstant(s)) {
    return Token(TokenType::Constant, s);
  } else if (isSeparator(s)) {
    return pushSeparator(s);
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

Token Lexer::pushSeparator(string s) {
  if (s == "(") {
    return Token(TokenType::OpenParenthesis, s);
  } else if (s == ")") {
    return Token(TokenType::CloseParenthesis, s);
  } else {
    return Token(TokenType::Separator, s);
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
    return Token(TokenType::Separator, s);
  } else if (s == "!") {
    return Token(TokenType::ExclamationMark, s);
  } else if (s == ">") {
    return Token(TokenType::Greater, s);
  } else if (s == "<") {
    return Token(TokenType::Lesser, s);
  } else if (s == "==") {
    return Token(TokenType::Equal, s);
  } else if (s == ">=") {
    return Token(TokenType::GreaterThanOrEqual, s);
  } else if (s == "<=") {
    return Token(TokenType::LesserThanOrEqual, s);
  } else if (s == "!=") {
    return Token(TokenType::NotEqual, s);
  }
}

bool Lexer::isIdentifier(string s) {
  if (isConstant(s.substr(0))) {
    return false;
  } else {
    return (!onlyContainDigits(s));
  }
}

bool Lexer::isSeparator(string s) {
  return (s == ";" || s == "{" || s == "}" || s == "(" || s == ")");
}

bool Lexer::isOperator(string s) {
  return (s == "+" || s == "-" || s == "*" || s == "/" || s == "=" ||
          s == "!" || s == ">" || s == "<" || s == "==" || s == ">=" ||
          s == "<=" || s == "!=");
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
