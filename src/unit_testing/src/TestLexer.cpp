#include "Lexer.h"

#include "catch.hpp"
#include "fstream"
#include "iostream"

using namespace std;


SCENARIO("Testing123") {
  ifstream file;

  file.open(
      "C:\\Users\\Flora Fong\\Documents\\SPAProject\\src\\spa\\src\\input.txt");
  string line;
  Lexer lexer;
  vector<Token> tokens;
  while (getline(file, line)) {
	  tokens = lexer.tokenize(line);
    for (auto i : tokens) {
      string type;
	  TokenType t = i.type;
      switch (t) {
      case TokenType::Procedure:
        type = "procedure";
        break;
      case TokenType::Assign:
        type = "assign";
        break;
      case TokenType::Print:
        type = "print";
        break;
      case TokenType::Read:
        type = "read";
        break;
      case TokenType::Identifier:
        type = "identifier";
        break;
      case TokenType::Constant:
        type = "constant";
        break;
      case TokenType::Separator:
        type = "separator";
        break;
      case TokenType::Plus:
        type = "plus";
        break;
      case TokenType::Minus:
        type = "minus";
        break;
      case TokenType::Equal:
        type = "equal";
        break;
      case TokenType::OpenParenthesis:
        type = "open paranthesis";
        break;
      case TokenType::CloseParenthesis:
        type = "close paranthesis";
        break;
	  case TokenType::If:
		  type = "if";
		  break;
	  case TokenType::While:
		  type = "while";
		  break;
	  case TokenType::Greater:
		  type = "greater";
		  break;
	  case TokenType::GreaterThanOrEqual:
		  type = "greater or equal";
		  break;
	  case TokenType::Lesser:
		  type = "lesser";
		  break;
	  case TokenType::LesserThanOrEqual:
		  type = "lesser or equal";
		  break;
	  case TokenType::NotEqual:
		  type = "not equal";
		  break;
	  case TokenType::ExclamationMark:
		  type = "exclaimation";
		  break;
	  case TokenType::Then:
		  type = "then";
		  break;
	  case TokenType::Else:
		  type = "else";
		  break;
      default:
        type = "fail";
      }
	  cout << type << " " << i.name << endl;
    }
  }
  file.close();
}

/**
SCENARIO("Lexing prodedure command") {
  string input = "procedure AssignmentTest {";
  Lexer lexer{input};
  queue<pair<TokenType, string>> tokenQueue = lexer.getTokenQueue();

  WHEN("Pushing into TokenQueue") {
    THEN("First element is Keyword procedure") {
      pair<TokenType, string> q1 = tokenQueue.front();
      REQUIRE(q1.first == TokenType::Procedure);
      REQUIRE(q1.second == "procedure");
    }
    tokenQueue.pop();

    THEN("Second element is Identifier AssignmentTest") {
      pair<TokenType, string> q2 = tokenQueue.front();
      REQUIRE(q2.first == TokenType::Identifier);
      REQUIRE(q2.second == "AssignmentTest");
    }
    tokenQueue.pop();

    THEN("Last element is Separator {") {
      pair<TokenType, string> q3 = tokenQueue.front();
      REQUIRE(q3.first == TokenType::Separator);
      REQUIRE(q3.second == "{");
    }
  }
}

SCENARIO("Lexing assign command"){

    WHEN("Pushing into TokenQueue"){string input = "x = 2 + y;";
Lexer lexer{input};
queue<pair<TokenType, string>> tokenQueue = lexer.getTokenQueue();

THEN("First element is Keyword assign") {
  pair<TokenType, string> q1 = tokenQueue.front();
  REQUIRE(q1.first == TokenType::Identifier);
  REQUIRE(q1.second == "assign");
}
tokenQueue.pop();

THEN("Second element is Identifier x") {
  pair<TokenType, string> q2 = tokenQueue.front();
  REQUIRE(q2.first == TokenType::Identifier);
  REQUIRE(q2.second == "x");
}
tokenQueue.pop();

THEN("Third element is Operator =") {
  pair<TokenType, string> q3 = tokenQueue.front();
  REQUIRE(q3.first == TokenType::Equal);
  REQUIRE(q3.second == "=");
}
tokenQueue.pop();

THEN("Fourth element is Literal 2") {
  pair<TokenType, string> q4 = tokenQueue.front();
  REQUIRE(q4.first == TokenType::Constant);
  REQUIRE(q4.second == "2");
}
tokenQueue.pop();

THEN("Fifth element is Operator +") {
  pair<TokenType, string> q5 = tokenQueue.front();
  REQUIRE(q5.first == TokenType::Plus);
  REQUIRE(q5.second == "+");
}
tokenQueue.pop();

THEN("Sixth element is Identifier y") {
  pair<TokenType, string> q6 = tokenQueue.front();
  REQUIRE(q6.first == TokenType::Identifier);
  REQUIRE(q6.second == "y");
}
tokenQueue.pop();

THEN("Last element is Separator ;") {
  pair<TokenType, string> q5 = tokenQueue.front();
  REQUIRE(q5.first == TokenType::Separator);
  REQUIRE(q5.second == ";");
}
}
}
**/
