#include "PQLLexer.h"
//#include "Relation.h"
#include <algorithm>
#include <queue>
#include <string.h>
#include <string>
#include <vector>

// modifies and uses(left emplty), pattern a is not a actual assignemnt, type,
// how to check errors
using namespace std;
bool BothAreSpaces(char lhs, char rhs) { return (lhs == rhs) && (lhs == ' '); }
string standerize(string str) {
  string new_str;
  for (int i = 0; i < str.length(); i++) {
    if (str[i] == '\t' || str[i] == '\n') {
      str[i] = ' ';
    }
  }
  for (int i = 0; i < str.length() - 5; i++) {
	  if (str.substr(i, 4) == "with") {
		  if (str[i + 4] != ' ') {
			  throw("wrong format with clause");
		  }
	  }
  }
  std::string::iterator new_end =
      std::unique(str.begin(), str.end(), BothAreSpaces);
  str.erase(new_end, str.end());
  if (str.front() == ' ')
    str = str.substr(1, str.size());
  if (str.back() == ' ')
    str = str.substr(0, str.size() - 1);
  for (std::string::size_type i = 0; i < str.size(); ++i) {
    if (str[i] == ',') { // remove space in front and add space at back
      if (new_str.back() != string::npos && new_str.back() == ' ') {
        new_str = new_str.substr(0, new_str.size() - 1);
        new_str = new_str + str[i];
      } else {
        new_str = new_str + str[i];
      }
      if (str[i + 1] != string::npos && str[i + 1] != ' ') {
        new_str = new_str + ' ';
      }
    } else if (str[i] == ';') {
      if (new_str.back() != string::npos && new_str.back() == ' ') {
        new_str = new_str.substr(0, new_str.size() - 1);
        new_str = new_str + str[i];
      } else {
        new_str = new_str + str[i];
      }
      if (str[i + 1] != string::npos && str[i + 1] != ' ') {
        new_str = new_str + ' ';
      }
    } else if (str[i] == ')') {
      if (new_str.back() != string::npos && new_str.back() == ' ') {
        new_str = new_str.substr(0, new_str.size() - 1);
        new_str = new_str + str[i];
      } else {
        new_str = new_str + str[i];
      }
      if (str[i + 1] != string::npos && str[i + 1] != ' ') {
        new_str = new_str + ' ';
      }
    } else if (str[i] == '\"') {
      if (new_str[new_str.size() - 1] != string::npos &&
          new_str[new_str.size() - 2] != string::npos &&
          new_str[new_str.size() - 1] == ' ' &&
          new_str[new_str.size() - 2] != ',') {
        new_str = new_str.substr(0, new_str.size() - 1);
        new_str = new_str + str[i];
      } else {
        new_str = new_str + str[i];
      }
    } else if (str[i] == '(') {
      if (new_str.back() != string::npos && new_str.back() != ' ') {
        new_str = new_str + ' ';
        new_str = new_str + str[i];
      } else {
        new_str = new_str + str[i];
      }
    } else if (str[i] == ')' || str[i] == '+' || str[i] == '-' ||
               str[i] == '*' || str[i] == '/' ||
               str[i] == '%') { // remove space in front
      if (new_str.back() != string::npos && new_str.back() == ' ') {
        new_str = new_str.substr(0, new_str.size() - 1);
        new_str = new_str + str[i];
      } else {
        new_str = new_str + str[i];
      }
    } else if (str[i] == ' ') { // remove space at back
      if (new_str.back() != string::npos &&
          (new_str.back() == '(' || new_str.back() == '+' ||
           new_str.back() == '-' || new_str.back() == '*' ||
           new_str.back() == '/' || new_str.back() == '%' ||
           new_str.back() == '\"')) {

      } else {
        new_str = new_str + str[i];
      }
    } else {
      new_str = new_str + str[i];
    }
  }
  return new_str;
}
PQLLexer::PQLLexer(string input) {
  string standard_input = standerize(input);
  //Tokenize(standard_input);
  Tokenize(input);
  selectEnd = false;
}

vector<string> PQLLexer::vectorize(string input) {
  vector<string> tokens;
  char *p;
  char *temp = (char *)input.c_str();
  p = strtok(temp, " ");
  while (p) {
    tokens.push_back(p);
    p = strtok(NULL, " ");
  }
  return tokens;
}

bool is_identifier(char a) {
	if (a == 'a' || a == 'b' || a == 'c' || a == 'd' || a == 'e' || a == 'f' || a == 'g'
		|| a == 'h' || a == 'i' || a == 'j' || a == 'k' || a == 'l' || a == 'm' || a == 'n'
		|| a == 'o' || a == 'p' || a == 'q' || a == 'r' || a == 's' || a == 't' || a == 'u'
		|| a == 'v' || a == 'w' || a == 'x' || a == 'y' || a == 'z' || a == '_' || a == '#') {
		return true;
	}
	else {
		return false;
	}
}

bool is_separator(char a) {
	if (a == '\"' || a == '(' || a == ')' || a == ',' || a == ';' || a == '<' || a == '>'
		|| a == '=' || a=='.') {
		return true;
	}
	else {
		return false;
	}
}

bool is_same_type(queue<char> q, char a) {
	if (is_identifier(q.back() && is_identifier(a))){
		return true;
	}
	else {
		return false;
	}
}

void PQLLexer::Tokenize(string input) {
  vector<string> token = vectorize(input);
  string last_keyword;
  queue<string> process_string;
  int index = 0;
  string iden = "";
  bool appear_such = false;
  bool quotation_appear = false;
  while ((index == input.length() + 1) || index <= input.length()) {
	  iden = "";
	  if (index == input.length() + 1) {
		  if (process_string.empty()) {

		  }
		  else {
			  while (!process_string.empty()) {
				  iden.append(process_string.front());
				  process_string.pop();
			  }
			  if (iden == "such") {
				  process_string.push("such ");
			  }
			  else if (iden == "pattern" || iden == "Select" || iden == "Parent"
				  || iden == "with" || iden == "Parent*" || iden == "stmt" || iden == "read"
				  || iden == "Modifies" || iden == "prog_line" || iden == "assign" || iden == "print"
				  || iden == "while" || iden == "if" || iden == "variable" || iden == "constant"
				  || iden == "procedure" || iden == "call" || iden == "Calls"
				  || iden == "Next" || iden == "Calls*" || iden == "Next*"
				  || iden == "Follows" || iden == "Follows*" || iden == "Uses"
				  || iden == "and" || iden == "Affects" || iden == "Affects*" || (iden == "such that")) {
				  tokenQueue.push(make_pair(TokenType::Keyword, iden));
			  }
			  else {
				  if (iden.length() == 0) {
				  }
				  else {
					  tokenQueue.push(make_pair(TokenType::Identifier, iden));
				  }
			  }
		  }
		  break;
	  }

	  if (input[index] == ' ') {
		  if (quotation_appear)
		  {
			  process_string.push(" ");
		  } else if (process_string.empty()) {

		  }
		  else {
			  while (!process_string.empty()) {
				  iden.append(process_string.front());
				  process_string.pop();
			  }
			  if (iden == "such") {
				  process_string.push("such ");
			  }
			  else if (iden == "pattern" || iden == "Select" || iden == "Parent"
				  || iden == "with" || iden == "Parent*" || iden == "stmt" || iden == "read"
				  || iden == "Modifies" || iden == "prog_line" || iden == "assign" || iden == "print"
				  || iden == "while" || iden == "if" || iden == "variable" || iden == "constant"
				  || iden == "procedure" || iden == "call" || iden == "Calls"
				  || iden == "Next" || iden == "Calls*" || iden == "Next*"
				  || iden == "Follows" || iden == "Follows*" || iden == "Uses"
				  || iden == "and" || (iden == "such that") || iden == "Affects" || iden == "Affects*") {
				  tokenQueue.push(make_pair(TokenType::Keyword, iden));
			  }
			  else {
				  tokenQueue.push(make_pair(TokenType::Identifier, iden));
			  }
		  }
	  }
	  else if (is_separator(input[index])) {

		 
		 if (input[index] == '\"')
		  {
			  if (quotation_appear)
			  {
				  while (!process_string.empty()) {
					  iden.append(process_string.front());
					  process_string.pop();
				  }
				  tokenQueue.push(make_pair(TokenType::Identifier, iden));
				  tokenQueue.push(make_pair(TokenType::Separator, "\""));
				  quotation_appear = false;
			  }
			  else
			  {
                            if (!process_string.empty())
                            {
				while (!process_string.empty()) {
					iden.append(process_string.front());
					process_string.pop();
				}
				tokenQueue.push(make_pair(TokenType::Identifier, iden));
                            }
				  tokenQueue.push(make_pair(TokenType::Separator, "\""));
				  quotation_appear = true;
				  index++;
				  continue;
			  }
		 }
		 else if (quotation_appear == false){
			 if (process_string.empty()) {

			 }

			 else {
				 while (!process_string.empty()) {
					 iden.append(process_string.front());
					 process_string.pop();
				 }
				 if (iden == "such") {
					 process_string.push("such");
				 }
				 else if (iden == "pattern" || iden == "Select" || iden == "Parent"
					 || iden == "with" || iden == "Parent*" || iden == "stmt" || iden == "read"
					 || iden == "Modifies" || iden == "prog_line" || iden == "assign" || iden == "print"
					 || iden == "while" || iden == "if" || iden == "variable" || iden == "constant"
					 || iden == "procedure" || iden == "call" || iden == "Calls"
					 || iden == "Next" || iden == "Calls*" || iden == "Next*"
					 || iden == "Follows" || iden == "Follows*" || iden == "Uses"
					 || iden == "and" || (iden == "such that") || iden == "Affects" || iden == "Affects*") {
					 tokenQueue.push(make_pair(TokenType::Keyword, iden));
				 }
				 else {
					 tokenQueue.push(make_pair(TokenType::Identifier, iden));
				 }
			 }
			 tokenQueue.push(make_pair(TokenType::Separator, input.substr(index, 1)));
		 } else
		 {
			 process_string.push(input.substr(index, 1));
		 }
	  }
	  else {

		  process_string.push(input.substr(index, 1));
	  }
  
		  
	  index++;
  }
}

queue<pair<TokenType, string>> PQLLexer::getTokenQueue() { return tokenQueue; }


