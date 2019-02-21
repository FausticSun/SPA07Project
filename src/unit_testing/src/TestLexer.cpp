#include "Lexer.h"

#include "catch.hpp"
#include "fstream"
#include "iostream"
#include "sstream"

using namespace std;


SCENARIO("Lexing line with comments give same result as the same line without comments") {
	stringstream ss1, ss2;
	ss1 << "x = 2; \\line with comment";
	ss2 << "x = 2; ";

	istream &fileStreamWithComment = ss1;
	istream &filestreamWithoutComment = ss2;

	Lexer lexer;
	queue<Token> tokensWithComment = lexer.tokenizeFile(fileStreamWithComment);
	queue<Token> tokensWithoutComment = lexer.tokenizeFile(filestreamWithoutComment);

	REQUIRE(tokensWithComment.size() == tokensWithoutComment.size());

	while (!tokensWithComment.empty()) {
		REQUIRE(tokensWithComment.front().type == tokensWithoutComment.front().type);
		REQUIRE(tokensWithComment.front().name == tokensWithoutComment.front().name);
		tokensWithComment.pop();
		tokensWithoutComment.pop();
	}

}

SCENARIO("Lexing Procedure with one assignment statement") {
	stringstream ss;
	ss << "procedure AssignmentTest {" << endl;
	ss << "w = (2+ (x-y) )* 3/z" << endl; // mixture of spaces, constants and identifiers
	ss << "}" << endl;

	istream &filestream = ss;
	Lexer lexer;
	queue<Token> tokens = lexer.tokenizeFile(filestream);

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

/**
SCENARIO("Lexing an invalid identifier") {
	stringstream ss;
	ss << "7test = 2;";

	istream &filestream = ss;
	Lexer lexer;
	queue<Token> tokens = lexer.tokenizeFile(filestream);
}
**/

SCENARIO("Lexing Procedure with read and print statements") {

	stringstream ss;
	ss << "procedure ReadAndPrintTest {" << endl;
	ss << "read input; " << endl;
	ss << "print result;} " << endl;

	istream &filestream = ss;
	Lexer lexer;
	queue<Token> tokens = lexer.tokenizeFile(filestream);

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
	stringstream ss;
	ss << "procedure IfTest { " << endl;
	ss << "if ((x == 1) && (y >= z)) then{" << endl;
	ss << "z = x + 1; }" << endl;
	ss << "else {" << endl;
	ss << "print x;" << endl;
	ss << "}}" << endl;

	istream &filestream = ss;
	Lexer lexer;
	queue<Token> tokens = lexer.tokenizeFile(filestream);

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
	stringstream ss;
	ss << "procedure WhileTest {" << endl;
	ss << "while(result != 0) {" <<endl;
	ss << "result = result - 1;" << endl;
	ss << "} }" << endl;;

	istream &filestream = ss;
	Lexer lexer;
	queue<Token> tokens = lexer.tokenizeFile(filestream);

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

SCENARIO("Lexing procedure with multiple statements, nesting levels and comments") {

	stringstream ss;
	ss << "procedure ABC" << endl;
	ss << "{" << endl;
	ss << " i=1; \\1" << endl;
	ss << "read b ;	\\2" << endl;
	ss << "c= a   ;\\3" << endl;
	ss << "if (a < beta) then{ \\4" << endl;
	ss << "while beta {	\\5" << endl;
	ss << "oSCar  = 1 * beta + tmp;	\\6" << endl;
	ss << "while tmp {	\\7" << endl;
	ss << "oSCar = I - (k + j1k * chArlie); }}	\\8" << endl;
	ss << "while (x!=1) {	\\9" << endl;
	ss << "x = x + 1;	\\10" << endl;
	ss << "if (left==0) then {	\\11" << endl;
	ss << "while (right== 5){	\\12" << endl;
	ss << "print Romeo;	\\13" << endl;
	ss << "b = 0;	\\14" << endl;
	ss << "c = delta    + l  * width + Romeo; }}	\\15" << endl;
	ss << "else {	" << endl;
	ss << "while (c>1) {	\\16" << endl;
	ss << "c = c -1;} 	\\17" << endl;
	ss << "x = x+ 1; }}}	\\18" << endl;
	ss << "else{" << endl;
	ss << "a= 2;}	\\19" << endl;
	ss << "}" << endl;




	istream &filestream = ss;
	Lexer lexer;
	queue<Token> tokens = lexer.tokenizeFile(filestream);

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
	REQUIRE(tokens.front().type == TokenType:: Constant);
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


