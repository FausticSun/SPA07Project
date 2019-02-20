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

  vector<Token> toAST;
	
  vector<string> tokens = vectorize(input);

  for (auto i : tokens) {
	  if (isKeyword(i)) {
		  toAST.push_back(pushKeyword(i));
	  }	  else if (isSeparator(i)) {
		  toAST.push_back(pushSeparator(i));
	  }	  else if (isOperator(i)) {
		  toAST.push_back(pushOperator(i));
	  }	  else if (isConstant(i)) {
		  toAST.push_back(pushConstant(i));
	  }	  else {
		  toAST.push_back(pushIdentifier(i));
	  }
  }

  return toAST;
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

Token Lexer::pushKeyword(string s) {

	if (s == "procedure") {
		return Token(TokenType::Procedure, s);
	} else if (s == "print") {
		return Token(TokenType::Print, s);
	} else if (s == "read") {
		return Token(TokenType::Read, s);
	} else if (s == "while") {
		return Token(TokenType::While, s);
	} else if (s == "if") {
		return Token(TokenType::If, s);
	} else if (s == "then") {
		return Token(TokenType::Then, s);
	} else if (s == "else") {
		return Token(TokenType::Else, s);
	} else {
		throw ("Invalid Keyword");
	}
}

Token Lexer::pushConstant(string s) {
	return Token(TokenType::Constant, s);
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

bool Lexer::isKeyword(string s) {
	return (s == "procedure" || s == "print" || s == "read" || s == "while" ||
		s == "if" || s == "then" || s == "else");
}

bool Lexer::isIdentifier(string s) {
  if (isConstant(s.substr(0))) {
    return false;
  } else {
    return (!isConstant(s));
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
  if (s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) {
    return false;
  }

  char *p;
  strtol(s.c_str(), &p, 10);

  return (*p == 0);
}
