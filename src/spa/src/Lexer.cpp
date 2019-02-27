#include "Lexer.h"
#include <iostream>

Lexer::Lexer() {}

Lexer::~Lexer() {}

std::queue<Token> Lexer::tokenizeFile(std::istream &fileStream) {
  std::queue<Token> allTokensInFile;
  std::string line;
  while (std::getline(fileStream, line)) {
    std::vector<Token> allTokensInLine = tokenize(line);
    for (Token i : allTokensInLine) {
      allTokensInFile.push(i);
    }
  }
  return allTokensInFile;
}

std::vector<Token> Lexer::tokenize(std::string input) {
  std::vector<Token> toAST;
  std::vector<std::string> tokens = vectorize(input);
  for (auto i : tokens) {
    if (isKeyword(i)) {
      toAST.push_back(pushKeyword(i));
    } else if (isSeparator(i)) {
      toAST.push_back(pushSeparator(i));
    } else if (isOperator(i)) {
      toAST.push_back(pushOperator(i));
    } else if (isConstant(i)) {
      toAST.push_back(pushConstant(i));
    } else if(isIdentifier(i)){
      toAST.push_back(pushIdentifier(i));
	} else {
		throw("Invalid token: " + i);
	}
  }
  return toAST;
}

std::vector<std::string> Lexer::vectorize(std::string input) {
  std::vector<std::string> tokens;
  std::queue<std::string> current;

  for (std::string::iterator it = input.begin(); it != input.end(); ++it) {
    std::string temp;
    temp.push_back(*it);
    if (isspace(*it)) {
      if (!current.empty()) {
        tokens.push_back(convertQueueToString(current));
        std::queue<std::string> empty;
        swap(current, empty);
      }
    } else if (isSeparator(temp)) {
      if (!current.empty()) {
        tokens.push_back(convertQueueToString(current));
        std::queue<std::string> empty;
        swap(current, empty);
      }
      tokens.push_back(temp);
    } else if (temp == "\\") {
		std::string nextTemp;
		nextTemp.push_back(*std::next(it,1)); 
		if(nextTemp == "\\") { //next char is backslash -> clear current queue and stop reading line
			if (!current.empty()) {
				tokens.push_back(convertQueueToString(current));
			}
			break;
		} else {
			throw("Invalid token: \\");
		}
    } else if (isOperator(temp)) { // current queue only contains Operator
                                   // or Letter/Number
      if (!current.empty()) {
        if (!isOperator(
                current.front())) { // queue currently stores variable
          tokens.push_back(convertQueueToString(current));
          std::queue<std::string> empty;
          swap(current, empty);
        }
      }
      current.push(temp);

    } else { // is either letter or digit
      if (!current.empty()) {
        if (isOperator(
                current.front())) { // queue currently stores operators
          tokens.push_back(convertQueueToString(current));
          std::queue<std::string> empty;
          swap(current, empty);
        }
      }
      current.push(temp);
    }
  }
  if (!current.empty()) {
    tokens.push_back(convertQueueToString(current));
  }
  return tokens;
}

std::string Lexer::convertQueueToString(std::queue<std::string> q) {
  std::string result;
  while (!q.empty()) {
    result += q.front();
    q.pop();
  }

  return result;
}

Token Lexer::getToken(std::string s) {
  if (isConstant(s)) {
    return Token(TokenType::Constant, s);
  } else if (isSeparator(s)) {
    return pushSeparator(s);
  } else if (isOperator(s)) {
    return pushOperator(s);
  } else if (isIdentifier(s)) {
    return Token(TokenType::Identifier, s);
  } else {
    throw("Invalid Identifier: " + s);
  }
}

Token Lexer::pushKeyword(std::string s) {

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
    throw("Invalid Keyword: " + s);
  }
}

Token Lexer::pushConstant(std::string s) {
  return Token(TokenType::Constant, s);
}

Token Lexer::pushIdentifier(std::string s) {
    return Token(TokenType::Identifier, s);
 
}

Token Lexer::pushSeparator(std::string s) {
  if (s == "(") {
    return Token(TokenType::OpenParenthesis, s);
  } else if (s == ")") {
    return Token(TokenType::CloseParenthesis, s);
  } else {
    return Token(TokenType::Separator, s);
  }
}

Token Lexer::pushOperator(std::string s) {
  if (s == "+") {
    return Token(TokenType::Plus, s);
  } else if (s == "-") {
    return Token(TokenType::Minus, s);
  } else if (s == "*") {
    return Token(TokenType::Multiply, s);
  } else if (s == "/") {
    return Token(TokenType::Divide, s);
  } else if (s == "%") {
    return Token(TokenType::Mod, s);
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
  } else if (s == "&&") {
    return Token(TokenType::And, s);
  } else if (s == "||") {
    return Token(TokenType::Or, s);
  }
}

bool Lexer::isKeyword(std::string s) {
  return (s == "procedure" || s == "print" || s == "read" || s == "while" ||
          s == "if" || s == "then" || s == "else");
}

bool Lexer::isIdentifier(std::string s) {
  if (isdigit(s.at(0))) {
    return false;
  } else {
	  for (char &c : s) {
		  if (!(isalnum(c))) {
			  return false;
		  }
	}
	  return true;
  }
}

bool Lexer::isSeparator(std::string s) {
  return (s == ";" || s == "{" || s == "}" || s == "(" || s == ")");
}

bool Lexer::isOperator(std::string s) {
  return (s == "+" || s == "-" || s == "*" || s == "/" || s == "%" ||
          s == "=" || s == "!" || s == ">" || s == "<" || s == "==" ||
          s == ">=" || s == "<=" || s == "!=" || s == "&" || s == "|" ||
          s == "&&" || s == "||");
}

bool Lexer::isConstant(std::string s) {
	for (char &c : s) {
		if (!(isdigit(c))) {
			return false;
		}
	}
	return true;
}
