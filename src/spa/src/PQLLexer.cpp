#include "PQLLexer.h"
#include <algorithm>
#include <queue>
#include <string.h>
#include <string>
#include <vector>
#include "Relation.h"

//modifies and uses(left emplty), pattern a is not a actual assignemnt, type, how to check errors
using namespace std;

PQLLexer::PQLLexer(string input) {
  Tokenize(input);
  selectEnd = false;
}

vector<string> PQLLexer::vectorize(string input) {
  vector<string> tokens;

  char *p;
  char *temp = (char*)input.c_str();
  p = strtok(temp, " ");
  while (p) {
    tokens.push_back(p);
    p = strtok(NULL, " ");
  }
  return tokens;
}

void PQLLexer::Tokenize(string input) {
  vector<string> token = vectorize(input);
  while (!token.empty()) {
    if (token[0] == "procedure") {
      tokenizeProcedure(token);
      token.erase(token.begin());
      token.erase(token.begin());
    }

    if (token[0] == "variable") {
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

    //if (find(token.begin(), token.end(), "select") != token.end()) {
    if (token[0] == "select") {
      tokenizeSelect(token);
      // deal with "," and ";". If ";", go to the outer loop.
      //token.erase(token.begin());
      //token.erase(token.begin());
      if (!token.empty()) {
        if (token[0] == "such" && token[1] == "that") {
          token.erase(token.begin());
          token.erase(token.begin());
          while (!token.empty()) {
            if (find(token.begin(), token.end(), "Modifies") != token.end()) {
              tokenizeModifies(token);
            }

            if (find(token.begin(), token.end(), "Uses") != token.end()) {
              tokenizeUses(token);

            }

            if (find(token.begin(), token.end(), "Parent*") != token.end()) {
              tokenizeParentT(token);

            } else if (find(token.begin(), token.end(), "Parent") != token.end()
            ) {
              tokenizeParent(token);
            }

            if (find(token.begin(), token.end(), "Follows*") != token.end()) {
              tokenizeFollowsT(token);
            } else if (find(token.begin(), token.end(), "Follows") != token.end()) {
              tokenizeFollows(token);
            }
          }
        } else if (token[0] == "pattern")
        {
          
        }
      }
    }

  }

}

bool isInt(string s) {
  if (s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return
    false;

  char *p;
  strtol(s.c_str(), &p, 10);

  return (*p == 0);
}

queue<pair<TokenType, string>> PQLLexer::getTokenQueue() {
  return tokenQueue;
}


queue<string> PQLLexer::getTarget() {
  return target;
}

string PQLLexer::SplitSemi(string s) {
  s = s.substr(0, s.length() - 1);
  return s;
}

string PQLLexer::SplitCommas(string s) {
  s = s.substr(0, s.length() - 1);
  return s;
}


void PQLLexer::tokenizeVariable(vector<string> token) {
  string variableName = token[1];
  variableName = SplitSemi(variableName);
  tokenQueue.push(make_pair(TokenType::Keyword, "variable"));
  tokenQueue.push(make_pair(TokenType::Identifier, variableName));
  tokenQueue.push(make_pair(TokenType::Separator, ";"));
}


void PQLLexer::tokenizeProcedure(vector<string> token) {
  string procedureName;
  procedureName = token[1];
  procedureName = SplitSemi(procedureName);
  tokenQueue.push(make_pair(TokenType::Keyword, "procedure"));
  tokenQueue.push(make_pair(TokenType::Identifier, procedureName));
  tokenQueue.push(make_pair(TokenType::Separator, ";"));
}


void PQLLexer::tokenizeRead(vector<string> token) {
  string readName;
  readName = token[1];
  readName = SplitSemi(readName);
  tokenQueue.push(make_pair(TokenType::Keyword, "read"));
  tokenQueue.push(make_pair(TokenType::Identifier, readName));
  tokenQueue.push(make_pair(TokenType::Separator, ";"));
}


void PQLLexer::tokenizePrint(vector<string> token) {
  string printName;
  printName = token[1];
  printName = SplitSemi(printName);
  tokenQueue.push(make_pair(TokenType::Keyword, "print"));
  tokenQueue.push(make_pair(TokenType::Identifier, printName));
  tokenQueue.push(make_pair(TokenType::Separator, ";"));
}


void PQLLexer::tokenizeWhile(vector<string> token) {
  string whileName;
  whileName = token[1];
  whileName = SplitSemi(whileName);
  tokenQueue.push(make_pair(TokenType::Keyword, "while"));
  tokenQueue.push(make_pair(TokenType::Identifier, whileName));
  tokenQueue.push(make_pair(TokenType::Separator, ";"));
}


void PQLLexer::tokenizeIf(vector<string> token) {
  string ifName;
  ifName = token[1];
  ifName = SplitSemi(ifName);
  tokenQueue.push(make_pair(TokenType::Keyword, "if"));
  tokenQueue.push(make_pair(TokenType::Identifier, ifName));
  tokenQueue.push(make_pair(TokenType::Separator, ";"));
}


void PQLLexer::tokenizeAssign(vector<string> token) {
  string assignName;
  assignName = token[1];
  assignName = SplitSemi(assignName);
  tokenQueue.push(make_pair(TokenType::Keyword, "assign"));
  tokenQueue.push(make_pair(TokenType::Identifier, assignName));
  tokenQueue.push(make_pair(TokenType::Separator, ";"));
}


void PQLLexer::tokenizeStmt(vector<string> token) {
  string stmtName;
  stmtName = token[1];
  stmtName = SplitSemi(stmtName);
  tokenQueue.push(make_pair(TokenType::Keyword, "stmt"));
  tokenQueue.push(make_pair(TokenType::Identifier, stmtName));
  tokenQueue.push(make_pair(TokenType::Separator, ";"));
}


void PQLLexer::tokenizeCall(vector<string> token) {
  string callName;
  callName = token[1];
  callName = SplitSemi(callName);
  tokenQueue.push(make_pair(TokenType::Keyword, "call"));
  tokenQueue.push(make_pair(TokenType::Identifier, callName));
  tokenQueue.push(make_pair(TokenType::Separator, ";"));
}


void PQLLexer::tokenizeConstant(vector<string> token) {
  string constantName;
  constantName = token[1];
  constantName = SplitSemi(constantName);
  tokenQueue.push(make_pair(TokenType::Keyword, "constant"));
  tokenQueue.push(make_pair(TokenType::Identifier, constantName));
  tokenQueue.push(make_pair(TokenType::Separator, ";"));
}

bool PQLLexer::tokenContainCommas(string token) {
  return (token.substr(token.length() - 1, token.length()) == ",");
}

bool PQLLexer::tokenContainSemi(string token) {
	return (token.substr(token.length() - 1, token.length()) == ";");
}

void expectionOfSelect(vector<string> token)
{
  if (token[0] == "such" && token[1] == "that")
  {
    
  } else if (token[0] == "pattern")
  {
    
  } else
  {
	  throw invalid_argument("expect such that or pattern");
  }
}

void PQLLexer::tokenizeSelect(vector<string> token) {

  tokenQueue.push(make_pair(TokenType::Keyword, "select"));
  token.erase(token.begin());
  // push the select, then for three situations: 1. "a, b". 2. "a;" 3. "a" 
  bool end = false;
  while (!end) {
    if (tokenContainCommas(token[0])) // first situation
    {
      tokenQueue.push(make_pair(TokenType::Identifier, SplitCommas(token[0])));
	  target.push(token[0]);
      tokenQueue.push(make_pair(TokenType::Separator, ","));
	  token.erase(token.begin());
    } else if (tokenContainSemi(token[0])) // second situation
    {
		tokenQueue.push(make_pair(TokenType::Identifier, SplitSemi(token[0])));
		target.push(token[0]);
		tokenQueue.push(make_pair(TokenType::Separator, ";"));
		token.erase(token.begin());
		end = true;
    } else
    {
		tokenQueue.push(make_pair(TokenType::Identifier, token[0]));
		target.push(token[0]);
		token.erase(token.begin());
		end = true;
    }
	expectionOfSelect(token);
  }

}


void PQLLexer::tokenizePattern(vector<string>) {
}

void PQLLexer::tokenizeFollows(vector<string> token) {
  tokenQueue.push(make_pair(TokenType::Keyword, "follows"));

  int check = 1;
  int n0, n1, n2, n3, iter = 0;
  string s;
  // KEYWORD, "(" "," ")" ";"

   while (!token.empty() && check == 1)
   {
     if (token[iter].find(")"))
     {
		 check = 0;
     }
   }
   s = token[0];
    for (int i = 1; i <=iter; i++)
    {
		s = s + token[i];
    }
    for (int i = 0; i < iter + 1; i++)
    {
		token.erase(token.begin());
    }
            for (int i = 0; i < s.length(); i++)
            {
              // no space
              if (s.at(i) == '(')
              {
				  n0 = i;
              }
	         if (token[0].at(i) == ',')
			  {
				  n1 = i;
			  }
			  if (token[0].at(i) == ')')
			  {
				  n2 = i;
			  }
			  tokenQueue.push(make_pair(TokenType::Keyword, "follows"));
			  tokenQueue.push(make_pair(TokenType::Separator, "("));
                  tokenQueue.push(make_pair(TokenType::Identifier, token[0].substr(n0, n1-1)));
		  tokenQueue.push(make_pair(TokenType::Separator, ","));
		  tokenQueue.push(make_pair(TokenType::Identifier, token[0].substr(n1, n2 - 1)));
		  tokenQueue.push(make_pair(TokenType::Separator, ")"));
            }
  if(s.find(";"))
  {
	  tokenQueue.push(make_pair(TokenType::Separator, ";"));
  } else if(token[0].find(";"))
  {
	  tokenQueue.push(make_pair(TokenType::Separator, ";"));
              token.erase(token.begin());
  }
  else
  {
    if (token[0] == "pattern")
    {
      
    } else
    {
		throw "should be ; or pattern";
    }
  }
         
  
}


void PQLLexer::tokenizeFollowsT(vector<string> token) {
	tokenQueue.push(make_pair(TokenType::Keyword, "followsT"));

	int check = 1;
	int n0, n1, n2, n3, iter = 0;
	string s;
	// KEYWORD, "(" "," ")" ";"

	while (!token.empty() && check == 1)
	{
		if (token[iter].find(")"))
		{
			check = 0;
		}
	}
	s = token[0];
	for (int i = 1; i <= iter; i++)
	{
		s = s + token[i];
	}
	for (int i = 0; i < iter + 1; i++)
	{
		token.erase(token.begin());
	}
	for (int i = 0; i < s.length(); i++)
	{
		// no space
		if (s.at(i) == '(')
		{
			n0 = i;
		}
		if (token[0].at(i) == ',')
		{
			n1 = i;
		}
		if (token[0].at(i) == ')')
		{
			n2 = i;
		}
		tokenQueue.push(make_pair(TokenType::Keyword, "followsT"));
		tokenQueue.push(make_pair(TokenType::Separator, "("));
		tokenQueue.push(make_pair(TokenType::Identifier, token[0].substr(n0, n1 - 1)));
		tokenQueue.push(make_pair(TokenType::Separator, ","));
		tokenQueue.push(make_pair(TokenType::Identifier, token[0].substr(n1, n2 - 1)));
		tokenQueue.push(make_pair(TokenType::Separator, ")"));
	}
	if (s.find(";"))
	{
		tokenQueue.push(make_pair(TokenType::Separator, ";"));
	}
	else if (token[0].find(";"))
	{
		tokenQueue.push(make_pair(TokenType::Separator, ";"));
		token.erase(token.begin());
	}
	else
	{
		if (token[0] == "pattern")
		{

		}
		else
		{
			throw "should be ; or pattern";
		}
	}
}


void PQLLexer::tokenizeParent(vector<string> token) {
	tokenQueue.push(make_pair(TokenType::Keyword, "parent"));

	int check = 1;
	int n0, n1, n2, n3, iter = 0;
	string s;
	// KEYWORD, "(" "," ")" ";"

	while (!token.empty() && check == 1)
	{
		if (token[iter].find(")"))
		{
			check = 0;
		}
	}
	s = token[0];
	for (int i = 1; i <= iter; i++)
	{
		s = s + token[i];
	}
	for (int i = 0; i < iter + 1; i++)
	{
		token.erase(token.begin());
	}
	for (int i = 0; i < s.length(); i++)
	{
		// no space
		if (s.at(i) == '(')
		{
			n0 = i;
		}
		if (token[0].at(i) == ',')
		{
			n1 = i;
		}
		if (token[0].at(i) == ')')
		{
			n2 = i;
		}
		tokenQueue.push(make_pair(TokenType::Keyword, "parent"));
		tokenQueue.push(make_pair(TokenType::Separator, "("));
		tokenQueue.push(make_pair(TokenType::Identifier, token[0].substr(n0, n1 - 1)));
		tokenQueue.push(make_pair(TokenType::Separator, ","));
		tokenQueue.push(make_pair(TokenType::Identifier, token[0].substr(n1, n2 - 1)));
		tokenQueue.push(make_pair(TokenType::Separator, ")"));
	}
	if (s.find(";"))
	{
		tokenQueue.push(make_pair(TokenType::Separator, ";"));
	}
	else if (token[0].find(";"))
	{
		tokenQueue.push(make_pair(TokenType::Separator, ";"));
		token.erase(token.begin());
	}
	else
	{
		if (token[0] == "pattern")
		{

		}
		else
		{
			throw "should be ; or pattern";
		}
	}
}


void PQLLexer::tokenizeParentT(vector<string> token) {
	tokenQueue.push(make_pair(TokenType::Keyword, "parentT"));

	int check = 1;
	int n0, n1, n2, n3, iter = 0;
	string s;
	// KEYWORD, "(" "," ")" ";"

	while (!token.empty() && check == 1)
	{
		if (token[iter].find(")"))
		{
			check = 0;
		}
	}
	s = token[0];
	for (int i = 1; i <= iter; i++)
	{
		s = s + token[i];
	}
	for (int i = 0; i < iter + 1; i++)
	{
		token.erase(token.begin());
	}
	for (int i = 0; i < s.length(); i++)
	{
		// no space
		if (s.at(i) == '(')
		{
			n0 = i;
		}
		if (token[0].at(i) == ',')
		{
			n1 = i;
		}
		if (token[0].at(i) == ')')
		{
			n2 = i;
		}
		tokenQueue.push(make_pair(TokenType::Keyword, "parentT"));
		tokenQueue.push(make_pair(TokenType::Separator, "("));
		tokenQueue.push(make_pair(TokenType::Identifier, token[0].substr(n0, n1 - 1)));
		tokenQueue.push(make_pair(TokenType::Separator, ","));
		tokenQueue.push(make_pair(TokenType::Identifier, token[0].substr(n1, n2 - 1)));
		tokenQueue.push(make_pair(TokenType::Separator, ")"));
	}
	if (s.find(";"))
	{
		tokenQueue.push(make_pair(TokenType::Separator, ";"));
	}
	else if (token[0].find(";"))
	{
		tokenQueue.push(make_pair(TokenType::Separator, ";"));
		token.erase(token.begin());
	}
	else
	{
		if (token[0] == "pattern")
		{

		}
		else
		{
			throw "should be ; or pattern";
		}
	}
}

void PQLLexer::tokenizeUses(vector<string> token) {
  if (isInt(token[0])) {
	  tokenQueue.push(make_pair(TokenType::Keyword, "useS"));

	  int check = 1;
	  int n0, n1, n2, n3, iter = 0;
	  string s;
	  // KEYWORD, "(" "," ")" ";"

	  while (!token.empty() && check == 1)
	  {
		  if (token[iter].find(")"))
		  {
			  check = 0;
		  }
	  }
	  s = token[0];
	  for (int i = 1; i <= iter; i++)
	  {
		  s = s + token[i];
	  }
	  for (int i = 0; i < iter + 1; i++)
	  {
		  token.erase(token.begin());
	  }
	  for (int i = 0; i < s.length(); i++)
	  {
		  // no space
		  if (s.at(i) == '(')
		  {
			  n0 = i;
		  }
		  if (token[0].at(i) == ',')
		  {
			  n1 = i;
		  }
		  if (token[0].at(i) == ')')
		  {
			  n2 = i;
		  }
		  tokenQueue.push(make_pair(TokenType::Keyword, "useS"));
		  tokenQueue.push(make_pair(TokenType::Separator, "("));
		  tokenQueue.push(make_pair(TokenType::Identifier, token[0].substr(n0, n1 - 1)));
		  tokenQueue.push(make_pair(TokenType::Separator, ","));
		  tokenQueue.push(make_pair(TokenType::Identifier, token[0].substr(n1, n2 - 1)));
		  tokenQueue.push(make_pair(TokenType::Separator, ")"));
	  }
	  if (s.find(";"))
	  {
		  tokenQueue.push(make_pair(TokenType::Separator, ";"));
	  }
	  else if (token[0].find(";"))
	  {
		  tokenQueue.push(make_pair(TokenType::Separator, ";"));
		  token.erase(token.begin());
	  }
	  else
	  {
		  if (token[0] == "pattern")
		  {

		  }
		  else
		  {
			  throw "should be ; or pattern";
		  }
	  }
  } else {
	  tokenQueue.push(make_pair(TokenType::Keyword, "usesP"));

	  int check = 1;
	  int n0, n1, n2, n3, iter = 0;
	  string s;
	  // KEYWORD, "(" "," ")" ";"

	  while (!token.empty() && check == 1)
	  {
		  if (token[iter].find(")"))
		  {
			  check = 0;
		  }
	  }
	  s = token[0];
	  for (int i = 1; i <= iter; i++)
	  {
		  s = s + token[i];
	  }
	  for (int i = 0; i < iter + 1; i++)
	  {
		  token.erase(token.begin());
	  }
	  for (int i = 0; i < s.length(); i++)
	  {
		  // no space
		  if (s.at(i) == '(')
		  {
			  n0 = i;
		  }
		  if (token[0].at(i) == ',')
		  {
			  n1 = i;
		  }
		  if (token[0].at(i) == ')')
		  {
			  n2 = i;
		  }
		  tokenQueue.push(make_pair(TokenType::Keyword, "usesP"));
		  tokenQueue.push(make_pair(TokenType::Separator, "("));
		  tokenQueue.push(make_pair(TokenType::Identifier, token[0].substr(n0, n1 - 1)));
		  tokenQueue.push(make_pair(TokenType::Separator, ","));
		  tokenQueue.push(make_pair(TokenType::Identifier, token[0].substr(n1, n2 - 1)));
		  tokenQueue.push(make_pair(TokenType::Separator, ")"));
	  }
	  if (s.find(";"))
	  {
		  tokenQueue.push(make_pair(TokenType::Separator, ";"));
	  }
	  else if (token[0].find(";"))
	  {
		  tokenQueue.push(make_pair(TokenType::Separator, ";"));
		  token.erase(token.begin());
	  }
	  else
	  {
		  if (token[0] == "pattern")
		  {

		  }
		  else
		  {
			  throw "should be ; or pattern";
		  }
	  }
  }
}


void PQLLexer::tokenizeModifies(vector<string> token) {
  if (isInt(token[0])) {
	  tokenQueue.push(make_pair(TokenType::Keyword, "modifiesS"));

	  int check = 1;
	  int n0, n1, n2, n3, iter = 0;
	  string s;
	  // KEYWORD, "(" "," ")" ";"

	  while (!token.empty() && check == 1)
	  {
		  if (token[iter].find(")"))
		  {
			  check = 0;
		  }
	  }
	  s = token[0];
	  for (int i = 1; i <= iter; i++)
	  {
		  s = s + token[i];
	  }
	  for (int i = 0; i < iter + 1; i++)
	  {
		  token.erase(token.begin());
	  }
	  for (int i = 0; i < s.length(); i++)
	  {
		  // no space
		  if (s.at(i) == '(')
		  {
			  n0 = i;
		  }
		  if (token[0].at(i) == ',')
		  {
			  n1 = i;
		  }
		  if (token[0].at(i) == ')')
		  {
			  n2 = i;
		  }
		  tokenQueue.push(make_pair(TokenType::Keyword, "modifiesS"));
		  tokenQueue.push(make_pair(TokenType::Separator, "("));
		  tokenQueue.push(make_pair(TokenType::Identifier, token[0].substr(n0, n1 - 1)));
		  tokenQueue.push(make_pair(TokenType::Separator, ","));
		  tokenQueue.push(make_pair(TokenType::Identifier, token[0].substr(n1, n2 - 1)));
		  tokenQueue.push(make_pair(TokenType::Separator, ")"));
	  }
	  if (s.find(";"))
	  {
		  tokenQueue.push(make_pair(TokenType::Separator, ";"));
	  }
	  else if (token[0].find(";"))
	  {
		  tokenQueue.push(make_pair(TokenType::Separator, ";"));
		  token.erase(token.begin());
	  }
	  else
	  {
		  if (token[0] == "pattern")
		  {

		  }
		  else
		  {
			  throw "should be ; or pattern";
		  }
	  }
  } else {
	  tokenQueue.push(make_pair(TokenType::Keyword, "modifiesP"));

	  int check = 1;
	  int n0, n1, n2, n3, iter = 0;
	  string s;
	  // KEYWORD, "(" "," ")" ";"

	  while (!token.empty() && check == 1)
	  {
		  if (token[iter].find(")"))
		  {
			  check = 0;
		  }
	  }
	  s = token[0];
	  for (int i = 1; i <= iter; i++)
	  {
		  s = s + token[i];
	  }
	  for (int i = 0; i < iter + 1; i++)
	  {
		  token.erase(token.begin());
	  }
	  for (int i = 0; i < s.length(); i++)
	  {
		  // no space
		  if (s.at(i) == '(')
		  {
			  n0 = i;
		  }
		  if (token[0].at(i) == ',')
		  {
			  n1 = i;
		  }
		  if (token[0].at(i) == ')')
		  {
			  n2 = i;
		  }
		  tokenQueue.push(make_pair(TokenType::Keyword, "modifiesP"));
		  tokenQueue.push(make_pair(TokenType::Separator, "("));
		  tokenQueue.push(make_pair(TokenType::Identifier, token[0].substr(n0, n1 - 1)));
		  tokenQueue.push(make_pair(TokenType::Separator, ","));
		  tokenQueue.push(make_pair(TokenType::Identifier, token[0].substr(n1, n2 - 1)));
		  tokenQueue.push(make_pair(TokenType::Separator, ")"));
	  }
	  if (s.find(";"))
	  {
		  tokenQueue.push(make_pair(TokenType::Separator, ";"));
	  }
	  else if (token[0].find(";"))
	  {
		  tokenQueue.push(make_pair(TokenType::Separator, ";"));
		  token.erase(token.begin());
	  }
	  else
	  {
		  if (token[0] == "pattern")
		  {

		  }
		  else
		  {
			  throw "should be ; or pattern";
		  }
	  }
  }
}
