#include "Lexer.h"

#include "catch.hpp"
#include <sstream>


SCENARIO("Lexing invalid tokens") {

	SECTION("Identifier begins with digit") {
		std::stringstream ss;
		ss << "1number = 1;";

		std::istream &filestream = ss;

		Lexer lexer;
		REQUIRE_THROWS_WITH(lexer.tokenizeFile(filestream),
			"Invalid token: 1number");
	}

	SECTION("Invalid operator") {
		std::stringstream ss;
		ss << "x += 2;";

		std::istream &filestream = ss;

		Lexer lexer;
		REQUIRE_THROWS_WITH(lexer.tokenizeFile(filestream),
			"Invalid token: +=");
	}

	SECTION("Invalid bracket") {
		std::stringstream ss;
		ss << "x = 2 + [ x - 1 ];";

		std::istream &filestream = ss;

		Lexer lexer;
		REQUIRE_THROWS_WITH(lexer.tokenizeFile(filestream),
			"Invalid token: [");
	}

	SECTION("Invalid decimals") {
		std::stringstream ss;
		ss << "x = 1.5;";

		std::istream &filestream = ss;

		Lexer lexer;
		REQUIRE_THROWS_WITH(lexer.tokenizeFile(filestream),
			"Invalid token: 1.5");
	}

	SECTION("Invalid operator") {
		std::stringstream ss;
		ss << "x += 2;";

		std::istream &filestream = ss;

		Lexer lexer;
		REQUIRE_THROWS_WITH(lexer.tokenizeFile(filestream),
			"Invalid token: +=");
	}

	SECTION("Invalid comment with only one backslash") {
		std::stringstream ss;
		ss << std::string{ R"(x = 3; \ line with comment)" };

		std::istream &filestream = ss;

		Lexer lexer;
		REQUIRE_THROWS_WITH(lexer.tokenizeFile(filestream),
			"Invalid token: \\");
	}

}

SCENARIO("Lexing line with comments give same result as the same line without comments") {
  std::stringstream ss1, ss2;
  ss1 << std::string{R"(x = 2; \\ line with comment)"};
  ss2 << "x = 2; ";
 

  std::istream &fileStreamWithComment = ss1;
  std::istream &filestreamWithoutComment = ss2;

  Lexer lexer;
  std::queue<Token> tokensWithComment =
      lexer.tokenizeFile(fileStreamWithComment);
  std::queue<Token> tokensWithoutComment =
      lexer.tokenizeFile(filestreamWithoutComment);

  REQUIRE(tokensWithComment.size() == tokensWithoutComment.size());

  while (!tokensWithComment.empty()) {
    REQUIRE(tokensWithComment.front().type ==
            tokensWithoutComment.front().type);
    REQUIRE(tokensWithComment.front().name ==
            tokensWithoutComment.front().name);
    tokensWithComment.pop();
    tokensWithoutComment.pop();
  }
}

SCENARIO("Lexing Procedure with one assignment statement") {
  std::stringstream ss;
  ss << "procedure AssignmentTest {" << std::endl;
  ss << "w = (2+ (x-y) )* 3/z"
     << std::endl; // mixture of spaces, constants and identifiers
  ss << "}" << std::endl;

  std::istream &filestream = ss;
  Lexer lexer;
  std::queue<Token> tokens = lexer.tokenizeFile(filestream);

  REQUIRE(tokens.front().type == TokenType::Procedure);
  REQUIRE(tokens.front().name == "procedure");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Identifier);
  REQUIRE(tokens.front().name == "AssignmentTest");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Separator);
  REQUIRE(tokens.front().name == "{");
  tokens.pop();

  REQUIRE(tokens.front().type == TokenType::Identifier);
  REQUIRE(tokens.front().name == "w");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Separator);
  REQUIRE(tokens.front().name == "=");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::OpenParenthesis);
  REQUIRE(tokens.front().name == "(");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Constant);
  REQUIRE(tokens.front().name == "2");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Plus);
  REQUIRE(tokens.front().name == "+");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::OpenParenthesis);
  REQUIRE(tokens.front().name == "(");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Identifier);
  REQUIRE(tokens.front().name == "x");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Minus);
  REQUIRE(tokens.front().name == "-");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Identifier);
  REQUIRE(tokens.front().name == "y");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::CloseParenthesis);
  REQUIRE(tokens.front().name == ")");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::CloseParenthesis);
  REQUIRE(tokens.front().name == ")");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Multiply);
  REQUIRE(tokens.front().name == "*");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Constant);
  REQUIRE(tokens.front().name == "3");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Divide);
  REQUIRE(tokens.front().name == "/");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Identifier);
  REQUIRE(tokens.front().name == "z");
  tokens.pop();

  REQUIRE(tokens.front().type == TokenType::Separator);
  REQUIRE(tokens.front().name == "}");
  tokens.pop();

  REQUIRE(tokens.size() == 0);
}

SCENARIO("Lexing Procedure with read and print statements") {
  std::stringstream ss;
  ss << "procedure ReadAndPrintTest {" << std::endl;
  ss << "read input; " << std::endl;
  ss << "print result;} " << std::endl;

  std::istream &filestream = ss;
  Lexer lexer;
  std::queue<Token> tokens = lexer.tokenizeFile(filestream);

  REQUIRE(tokens.front().type == TokenType::Procedure);
  REQUIRE(tokens.front().name == "procedure");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Identifier);
  REQUIRE(tokens.front().name == "ReadAndPrintTest");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Separator);
  REQUIRE(tokens.front().name == "{");
  tokens.pop();

  REQUIRE(tokens.front().type == TokenType::Read);
  REQUIRE(tokens.front().name == "read");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Identifier);
  REQUIRE(tokens.front().name == "input");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Separator);
  REQUIRE(tokens.front().name == ";");
  tokens.pop();

  REQUIRE(tokens.front().type == TokenType::Print);
  REQUIRE(tokens.front().name == "print");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Identifier);
  REQUIRE(tokens.front().name == "result");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Separator);
  REQUIRE(tokens.front().name == ";");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Separator);
  REQUIRE(tokens.front().name == "}");
  tokens.pop();

  REQUIRE(tokens.size() == 0);
}

SCENARIO("Lexing Procedures with if statements") {
  std::stringstream ss;
  ss << "procedure IfTest { " << std::endl;
  ss << "if ((x == 1) && (y >= z)) then{" << std::endl;
  ss << "z = x + 1; }" << std::endl;
  ss << "else {" << std::endl;
  ss << "print x;" << std::endl;
  ss << "}}" << std::endl;

  std::istream &filestream = ss;
  Lexer lexer;
  std::queue<Token> tokens = lexer.tokenizeFile(filestream);

  REQUIRE(tokens.front().type == TokenType::Procedure);
  REQUIRE(tokens.front().name == "procedure");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Identifier);
  REQUIRE(tokens.front().name == "IfTest");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Separator);
  REQUIRE(tokens.front().name == "{");
  tokens.pop();

  REQUIRE(tokens.front().type == TokenType::If);
  REQUIRE(tokens.front().name == "if");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::OpenParenthesis);
  REQUIRE(tokens.front().name == "(");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::OpenParenthesis);
  REQUIRE(tokens.front().name == "(");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Identifier);
  REQUIRE(tokens.front().name == "x");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Equal);
  REQUIRE(tokens.front().name == "==");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Constant);
  REQUIRE(tokens.front().name == "1");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::CloseParenthesis);
  REQUIRE(tokens.front().name == ")");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::And);
  REQUIRE(tokens.front().name == "&&");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::OpenParenthesis);
  REQUIRE(tokens.front().name == "(");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Identifier);
  REQUIRE(tokens.front().name == "y");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::GreaterThanOrEqual);
  REQUIRE(tokens.front().name == ">=");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Identifier);
  REQUIRE(tokens.front().name == "z");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::CloseParenthesis);
  REQUIRE(tokens.front().name == ")");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::CloseParenthesis);
  REQUIRE(tokens.front().name == ")");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Then);
  REQUIRE(tokens.front().name == "then");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Separator);
  REQUIRE(tokens.front().name == "{");
  tokens.pop();

  REQUIRE(tokens.front().type == TokenType::Identifier);
  REQUIRE(tokens.front().name == "z");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Separator);
  REQUIRE(tokens.front().name == "=");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Identifier);
  REQUIRE(tokens.front().name == "x");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Plus);
  REQUIRE(tokens.front().name == "+");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Constant);
  REQUIRE(tokens.front().name == "1");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Separator);
  REQUIRE(tokens.front().name == ";");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Separator);
  REQUIRE(tokens.front().name == "}");
  tokens.pop();

  REQUIRE(tokens.front().type == TokenType::Else);
  REQUIRE(tokens.front().name == "else");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Separator);
  REQUIRE(tokens.front().name == "{");
  tokens.pop();

  REQUIRE(tokens.front().type == TokenType::Print);
  REQUIRE(tokens.front().name == "print");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Identifier);
  REQUIRE(tokens.front().name == "x");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Separator);
  REQUIRE(tokens.front().name == ";");
  tokens.pop();

  REQUIRE(tokens.front().type == TokenType::Separator);
  REQUIRE(tokens.front().name == "}");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Separator);
  REQUIRE(tokens.front().name == "}");
  tokens.pop();

  REQUIRE(tokens.size() == 0);
}

SCENARIO("Lexing procedure with one while loop") {
  std::stringstream ss;
  ss << "procedure WhileTest {" << std::endl;
  ss << "while(result != 0) {" << std::endl;
  ss << "result = result - 1;" << std::endl;
  ss << "} }" << std::endl;
  ;

  std::istream &filestream = ss;
  Lexer lexer;
  std::queue<Token> tokens = lexer.tokenizeFile(filestream);

  REQUIRE(tokens.front().type == TokenType::Procedure);
  REQUIRE(tokens.front().name == "procedure");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Identifier);
  REQUIRE(tokens.front().name == "WhileTest");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Separator);
  REQUIRE(tokens.front().name == "{");
  tokens.pop();

  REQUIRE(tokens.front().type == TokenType::While);
  REQUIRE(tokens.front().name == "while");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::OpenParenthesis);
  REQUIRE(tokens.front().name == "(");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Identifier);
  REQUIRE(tokens.front().name == "result");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::NotEqual);
  REQUIRE(tokens.front().name == "!=");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Constant);
  REQUIRE(tokens.front().name == "0");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::CloseParenthesis);
  REQUIRE(tokens.front().name == ")");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Separator);
  REQUIRE(tokens.front().name == "{");
  tokens.pop();

  REQUIRE(tokens.front().type == TokenType::Identifier);
  REQUIRE(tokens.front().name == "result");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Separator);
  REQUIRE(tokens.front().name == "=");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Identifier);
  REQUIRE(tokens.front().name == "result");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Minus);
  REQUIRE(tokens.front().name == "-");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Constant);
  REQUIRE(tokens.front().name == "1");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Separator);
  REQUIRE(tokens.front().name == ";");
  tokens.pop();

  REQUIRE(tokens.front().type == TokenType::Separator);
  REQUIRE(tokens.front().name == "}");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Separator);
  REQUIRE(tokens.front().name == "}");
  tokens.pop();

  REQUIRE(tokens.size() == 0);
}

SCENARIO(
    "Lexing procedure with multiple statements, nesting levels and comments") {
  std::stringstream ss;
  ss << "procedure ABC" << std::endl;
  ss << "{" << std::endl;
  ss << std::string{ R"( i=1; \\1)"} << std::endl;
  ss << std::string{ R"( read b ;	\\2)" } << std::endl;
  ss << std::string{ R"( c= a   ;\\3)" } << std::endl;
  ss << std::string{ R"( if (a < beta) then{ \\4)" } << std::endl;
  ss << std::string{ R"( while beta {	\\5)" } << std::endl;
  ss << std::string{ R"( oSCar  = 1 * beta + tmp;	\\6)" } << std::endl;
  ss << std::string{ R"( while tmp {	\\7)" } << std::endl;
  ss << std::string{ R"( oSCar = I - (k + j1k * chArlie); }}	\\8)" } << std::endl;
  ss << std::string{ R"( while (x!=1) {	\\9)" } << std::endl;
  ss << std::string{ R"( x = x + 1;	\\10)" } << std::endl;
  ss << std::string{ R"( if (left==0) then {	\\11)" } << std::endl;
  ss << std::string{ R"( while (right== 5){	\\12)" } << std::endl;
  ss << std::string{ R"( print Romeo;	\\13)" } << std::endl;
  ss << std::string{ R"( b = 0;	\\14)" } << std::endl;
  ss << std::string{ R"( c = delta    + l  * width + Romeo; }}	\\15)" } << std::endl;
  ss << std::string{ R"( else {	)" } << std::endl;
  ss << std::string{ R"( while (c>1) {	\\16)" } << std::endl;
  ss << std::string{ R"( c = c -1;} 	\\17)" } << std::endl;
  ss << std::string{ R"( x = x+ 1; }}}	\\18)" } << std::endl;
  ss << std::string{ R"( else{)" } << std::endl;
  ss << std::string{ R"( a= 2;}	\\19)" } << std::endl;
  ss << std::string{ R"( })" } << std::endl;

  std::istream &filestream = ss;
  Lexer lexer;
  std::queue<Token> tokens = lexer.tokenizeFile(filestream);

  REQUIRE(tokens.front().type == TokenType::Procedure);
  REQUIRE(tokens.front().name == "procedure");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Identifier);
  REQUIRE(tokens.front().name == "ABC");
  tokens.pop();

  REQUIRE(tokens.front().type == TokenType::Separator);
  REQUIRE(tokens.front().name == "{");
  tokens.pop();

  REQUIRE(tokens.front().type == TokenType::Identifier);
  REQUIRE(tokens.front().name == "i");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Separator);
  REQUIRE(tokens.front().name == "=");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Constant);
  REQUIRE(tokens.front().name == "1");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Separator);
  REQUIRE(tokens.front().name == ";");
  tokens.pop();

  REQUIRE(tokens.front().type == TokenType::Read);
  REQUIRE(tokens.front().name == "read");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Identifier);
  REQUIRE(tokens.front().name == "b");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Separator);
  REQUIRE(tokens.front().name == ";");
  tokens.pop();

  REQUIRE(tokens.front().type == TokenType::Identifier);
  REQUIRE(tokens.front().name == "c");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Separator);
  REQUIRE(tokens.front().name == "=");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Identifier);
  REQUIRE(tokens.front().name == "a");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Separator);
  REQUIRE(tokens.front().name == ";");
  tokens.pop();

  REQUIRE(tokens.front().type == TokenType::If);
  REQUIRE(tokens.front().name == "if");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::OpenParenthesis);
  REQUIRE(tokens.front().name == "(");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Identifier);
  REQUIRE(tokens.front().name == "a");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Lesser);
  REQUIRE(tokens.front().name == "<");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Identifier);
  REQUIRE(tokens.front().name == "beta");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::CloseParenthesis);
  REQUIRE(tokens.front().name == ")");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Then);
  REQUIRE(tokens.front().name == "then");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Separator);
  REQUIRE(tokens.front().name == "{");
  tokens.pop();

  REQUIRE(tokens.front().type == TokenType::While);
  REQUIRE(tokens.front().name == "while");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Identifier);
  REQUIRE(tokens.front().name == "beta");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Separator);
  REQUIRE(tokens.front().name == "{");
  tokens.pop();

  REQUIRE(tokens.front().type == TokenType::Identifier);
  REQUIRE(tokens.front().name == "oSCar");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Separator);
  REQUIRE(tokens.front().name == "=");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Constant);
  REQUIRE(tokens.front().name == "1");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Multiply);
  REQUIRE(tokens.front().name == "*");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Identifier);
  REQUIRE(tokens.front().name == "beta");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Plus);
  REQUIRE(tokens.front().name == "+");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Identifier);
  REQUIRE(tokens.front().name == "tmp");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Separator);
  REQUIRE(tokens.front().name == ";");
  tokens.pop();

  REQUIRE(tokens.front().type == TokenType::While);
  REQUIRE(tokens.front().name == "while");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Identifier);
  REQUIRE(tokens.front().name == "tmp");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Separator);
  REQUIRE(tokens.front().name == "{");
  tokens.pop();

  REQUIRE(tokens.front().type == TokenType::Identifier);
  REQUIRE(tokens.front().name == "oSCar");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Separator);
  REQUIRE(tokens.front().name == "=");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Identifier);
  REQUIRE(tokens.front().name == "I");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Minus);
  REQUIRE(tokens.front().name == "-");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::OpenParenthesis);
  REQUIRE(tokens.front().name == "(");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Identifier);
  REQUIRE(tokens.front().name == "k");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Plus);
  REQUIRE(tokens.front().name == "+");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Identifier);
  REQUIRE(tokens.front().name == "j1k");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Multiply);
  REQUIRE(tokens.front().name == "*");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Identifier);
  REQUIRE(tokens.front().name == "chArlie");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::CloseParenthesis);
  REQUIRE(tokens.front().name == ")");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Separator);
  REQUIRE(tokens.front().name == ";");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Separator);
  REQUIRE(tokens.front().name == "}");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Separator);
  REQUIRE(tokens.front().name == "}");
  tokens.pop();

  REQUIRE(tokens.front().type == TokenType::While);
  REQUIRE(tokens.front().name == "while");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::OpenParenthesis);
  REQUIRE(tokens.front().name == "(");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Identifier);
  REQUIRE(tokens.front().name == "x");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::NotEqual);
  REQUIRE(tokens.front().name == "!=");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Constant);
  REQUIRE(tokens.front().name == "1");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::CloseParenthesis);
  REQUIRE(tokens.front().name == ")");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Separator);
  REQUIRE(tokens.front().name == "{");
  tokens.pop();

  REQUIRE(tokens.front().type == TokenType::Identifier);
  REQUIRE(tokens.front().name == "x");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Separator);
  REQUIRE(tokens.front().name == "=");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Identifier);
  REQUIRE(tokens.front().name == "x");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Plus);
  REQUIRE(tokens.front().name == "+");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Constant);
  REQUIRE(tokens.front().name == "1");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Separator);
  REQUIRE(tokens.front().name == ";");
  tokens.pop();

  REQUIRE(tokens.front().type == TokenType::If);
  REQUIRE(tokens.front().name == "if");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::OpenParenthesis);
  REQUIRE(tokens.front().name == "(");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Identifier);
  REQUIRE(tokens.front().name == "left");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Equal);
  REQUIRE(tokens.front().name == "==");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Constant);
  REQUIRE(tokens.front().name == "0");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::CloseParenthesis);
  REQUIRE(tokens.front().name == ")");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Then);
  REQUIRE(tokens.front().name == "then");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Separator);
  REQUIRE(tokens.front().name == "{");
  tokens.pop();

  REQUIRE(tokens.front().type == TokenType::While);
  REQUIRE(tokens.front().name == "while");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::OpenParenthesis);
  REQUIRE(tokens.front().name == "(");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Identifier);
  REQUIRE(tokens.front().name == "right");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Equal);
  REQUIRE(tokens.front().name == "==");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Constant);
  REQUIRE(tokens.front().name == "5");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::CloseParenthesis);
  REQUIRE(tokens.front().name == ")");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Separator);
  REQUIRE(tokens.front().name == "{");
  tokens.pop();

  REQUIRE(tokens.front().type == TokenType::Print);
  REQUIRE(tokens.front().name == "print");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Identifier);
  REQUIRE(tokens.front().name == "Romeo");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Separator);
  REQUIRE(tokens.front().name == ";");
  tokens.pop();

  REQUIRE(tokens.front().type == TokenType::Identifier);
  REQUIRE(tokens.front().name == "b");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Separator);
  REQUIRE(tokens.front().name == "=");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Constant);
  REQUIRE(tokens.front().name == "0");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Separator);
  REQUIRE(tokens.front().name == ";");
  tokens.pop();

  REQUIRE(tokens.front().type == TokenType::Identifier);
  REQUIRE(tokens.front().name == "c");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Separator);
  REQUIRE(tokens.front().name == "=");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Identifier);
  REQUIRE(tokens.front().name == "delta");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Plus);
  REQUIRE(tokens.front().name == "+");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Identifier);
  REQUIRE(tokens.front().name == "l");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Multiply);
  REQUIRE(tokens.front().name == "*");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Identifier);
  REQUIRE(tokens.front().name == "width");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Plus);
  REQUIRE(tokens.front().name == "+");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Identifier);
  REQUIRE(tokens.front().name == "Romeo");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Separator);
  REQUIRE(tokens.front().name == ";");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Separator);
  REQUIRE(tokens.front().name == "}");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Separator);
  REQUIRE(tokens.front().name == "}");
  tokens.pop();

  REQUIRE(tokens.front().type == TokenType::Else);
  REQUIRE(tokens.front().name == "else");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Separator);
  REQUIRE(tokens.front().name == "{");
  tokens.pop();

  REQUIRE(tokens.front().type == TokenType::While);
  REQUIRE(tokens.front().name == "while");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::OpenParenthesis);
  REQUIRE(tokens.front().name == "(");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Identifier);
  REQUIRE(tokens.front().name == "c");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Greater);
  REQUIRE(tokens.front().name == ">");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Constant);
  REQUIRE(tokens.front().name == "1");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::CloseParenthesis);
  REQUIRE(tokens.front().name == ")");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Separator);
  REQUIRE(tokens.front().name == "{");
  tokens.pop();

  REQUIRE(tokens.front().type == TokenType::Identifier);
  REQUIRE(tokens.front().name == "c");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Separator);
  REQUIRE(tokens.front().name == "=");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Identifier);
  REQUIRE(tokens.front().name == "c");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Minus);
  REQUIRE(tokens.front().name == "-");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Constant);
  REQUIRE(tokens.front().name == "1");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Separator);
  REQUIRE(tokens.front().name == ";");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Separator);
  REQUIRE(tokens.front().name == "}");
  tokens.pop();

  REQUIRE(tokens.front().type == TokenType::Identifier);
  REQUIRE(tokens.front().name == "x");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Separator);
  REQUIRE(tokens.front().name == "=");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Identifier);
  REQUIRE(tokens.front().name == "x");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Plus);
  REQUIRE(tokens.front().name == "+");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Constant);
  REQUIRE(tokens.front().name == "1");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Separator);
  REQUIRE(tokens.front().name == ";");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Separator);
  REQUIRE(tokens.front().name == "}");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Separator);
  REQUIRE(tokens.front().name == "}");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Separator);
  REQUIRE(tokens.front().name == "}");
  tokens.pop();

  REQUIRE(tokens.front().type == TokenType::Else);
  REQUIRE(tokens.front().name == "else");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Separator);
  REQUIRE(tokens.front().name == "{");
  tokens.pop();

  REQUIRE(tokens.front().type == TokenType::Identifier);
  REQUIRE(tokens.front().name == "a");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Separator);
  REQUIRE(tokens.front().name == "=");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Constant);
  REQUIRE(tokens.front().name == "2");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Separator);
  REQUIRE(tokens.front().name == ";");
  tokens.pop();
  REQUIRE(tokens.front().type == TokenType::Separator);
  REQUIRE(tokens.front().name == "}");
  tokens.pop();

  REQUIRE(tokens.front().type == TokenType::Separator);
  REQUIRE(tokens.front().name == "}");
  tokens.pop();

  REQUIRE(tokens.size() == 0);
}
