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

    if (token[0] == "statement") {
      tokenizeStmt(token);
      token.erase(token.begin());
      token.erase(token.begin());
    }

    if (token[0] == "read") {
      tokenizeRead(token);
      token.erase(token.begin());
      token.erase(token.begin());
    }

    if (token[0] == "print") {
      tokenizePrint(token);
      token.erase(token.begin());
      token.erase(token.begin());
    }

    if (token[0] == "while") {
      tokenizeWhile(token);
      token.erase(token.begin());
      token.erase(token.begin());
    }

    if (token[0] == "if") {
      tokenizeIf(token);
      token.erase(token.begin());
      token.erase(token.begin());
    }

    if (token[0] == "call") {
      tokenizeCall(token);
      token.erase(token.begin());
      token.erase(token.begin());
    }

    if (token[0] == "assign") {
      tokenizeAssign(token);
      token.erase(token.begin());
      token.erase(token.begin());
    }

    if (token[0] == "constant") {
      tokenizeConstant(token);
      token.erase(token.begin());
      token.erase(token.begin());
    }

    //if (find(token.begin(), token.end(), "select") != token.end()) {
    if (!token.empty() && token[0] == "Select") {
      token = tokenizeSelect(token);
      // deal with "," and ";". If ";", go to the outer loop.
      //token.erase(token.begin());
      //token.erase(token.begin());
      if (!token.empty()) {
        if (token[0] == "such" && token[1] == "that") {
			tokenQueue.push(make_pair(TokenType::Keyword, "such that"));
          token.erase(token.begin());
          token.erase(token.begin());
          while (!token.empty()) {
            if (token[0] == "Modifies") {
				token = tokenizeModifies(token);
            }

             else if (token[0] == "Uses") {
				token = tokenizeUses(token);

            }

             else if (token[0] == "Parent*") {
				token = tokenizeParentT(token);

            } else if (token[0] == "Parent")
            {
				token = tokenizeParent(token);
            }

            else if (token[0] == "Follows*") {
              token = tokenizeFollowsT(token);
            } else if (token[0] == "Follows") {
              token = tokenizeFollows(token);
            }
          }
        } else if (token[0] == "pattern")
        {
			token = tokenizePattern(token); 
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


vector<string> PQLLexer::tokenizeVariable(vector<string> token) {
	tokenQueue.push(make_pair(TokenType::Keyword, "variable"));
	token.erase(token.begin());
	bool end = false;
	while (!end) {
		if (tokenContainCommas(token[0])) // first situation
		{
			tokenQueue.push(make_pair(TokenType::Identifier, SplitCommas(token[0])));
			tokenQueue.push(make_pair(TokenType::Separator, ","));
			token.erase(token.begin());
		}
		else if (tokenContainSemi(token[0])) // second situation
		{
			tokenQueue.push(make_pair(TokenType::Identifier, SplitSemi(token[0])));
			tokenQueue.push(make_pair(TokenType::Separator, ";"));
			token.erase(token.begin());
			end = true;
		}
		else
		{
			tokenQueue.push(make_pair(TokenType::Identifier, token[0]));
			token.erase(token.begin());
			end = true;
		}
		if (!end) {
			expectionOfSelect(token);
		}
	}
	return token;
}


vector<string> PQLLexer::tokenizeProcedure(vector<string> token) {
	tokenQueue.push(make_pair(TokenType::Keyword, "procedure"));
	token.erase(token.begin());
	bool end = false;
	while (!end) {
		if (tokenContainCommas(token[0])) // first situation
		{
			tokenQueue.push(make_pair(TokenType::Identifier, SplitCommas(token[0])));
			tokenQueue.push(make_pair(TokenType::Separator, ","));
			token.erase(token.begin());
		}
		else if (tokenContainSemi(token[0])) // second situation
		{
			tokenQueue.push(make_pair(TokenType::Identifier, SplitSemi(token[0])));
			tokenQueue.push(make_pair(TokenType::Separator, ";"));
			token.erase(token.begin());
			end = true;
		}
		else
		{
			tokenQueue.push(make_pair(TokenType::Identifier, token[0]));
			token.erase(token.begin());
			end = true;
		}
		if (!end) {
			expectionOfSelect(token);
		}
	}
	return token;
}


vector<string> PQLLexer::tokenizeRead(vector<string> token) {
	tokenQueue.push(make_pair(TokenType::Keyword, "read"));
	token.erase(token.begin());
	bool end = false;
	while (!end) {
		if (tokenContainCommas(token[0])) // first situation
		{
			tokenQueue.push(make_pair(TokenType::Identifier, SplitCommas(token[0])));
			tokenQueue.push(make_pair(TokenType::Separator, ","));
			token.erase(token.begin());
		}
		else if (tokenContainSemi(token[0])) // second situation
		{
			tokenQueue.push(make_pair(TokenType::Identifier, SplitSemi(token[0])));
			tokenQueue.push(make_pair(TokenType::Separator, ";"));
			token.erase(token.begin());
			end = true;
		}
		else
		{
			tokenQueue.push(make_pair(TokenType::Identifier, token[0]));
			token.erase(token.begin());
			end = true;
		}
		if (!end) {
			expectionOfSelect(token);
		}
	}
	return token;
}


vector<string> PQLLexer::tokenizePrint(vector<string> token) {
	tokenQueue.push(make_pair(TokenType::Keyword, "print"));
	token.erase(token.begin());
	bool end = false;
	while (!end) {
		if (tokenContainCommas(token[0])) // first situation
		{
			tokenQueue.push(make_pair(TokenType::Identifier, SplitCommas(token[0])));
			tokenQueue.push(make_pair(TokenType::Separator, ","));
			token.erase(token.begin());
		}
		else if (tokenContainSemi(token[0])) // second situation
		{
			tokenQueue.push(make_pair(TokenType::Identifier, SplitSemi(token[0])));
			tokenQueue.push(make_pair(TokenType::Separator, ";"));
			token.erase(token.begin());
			end = true;
		}
		else
		{
			tokenQueue.push(make_pair(TokenType::Identifier, token[0]));
			token.erase(token.begin());
			end = true;
		}
		if (!end) {
			expectionOfSelect(token);
		}
	}
	return token;
}


vector<string> PQLLexer::tokenizeWhile(vector<string> token) {
  tokenQueue.push(make_pair(TokenType::Keyword, "while"));
  token.erase(token.begin());
  bool end = false;
  while (!end) {
	  if (tokenContainCommas(token[0])) // first situation
	  {
		  tokenQueue.push(make_pair(TokenType::Identifier, SplitCommas(token[0])));
		  tokenQueue.push(make_pair(TokenType::Separator, ","));
		  token.erase(token.begin());
	  }
	  else if (tokenContainSemi(token[0])) // second situation
	  {
		  tokenQueue.push(make_pair(TokenType::Identifier, SplitSemi(token[0])));
		  tokenQueue.push(make_pair(TokenType::Separator, ";"));
		  token.erase(token.begin());
		  end = true;
	  }
	  else
	  {
		  tokenQueue.push(make_pair(TokenType::Identifier, token[0]));
		  token.erase(token.begin());
		  end = true;
	  }
	  if (!end) {
		  expectionOfSelect(token);
	  }
  }
  return token;
}



vector<string> PQLLexer::tokenizeIf(vector<string> token) {
	tokenQueue.push(make_pair(TokenType::Keyword, "if"));
	token.erase(token.begin());
	bool end = false;
	while (!end) {
		if (tokenContainCommas(token[0])) // first situation
		{
			tokenQueue.push(make_pair(TokenType::Identifier, SplitCommas(token[0])));
			tokenQueue.push(make_pair(TokenType::Separator, ","));
			token.erase(token.begin());
		}
		else if (tokenContainSemi(token[0])) // second situation
		{
			tokenQueue.push(make_pair(TokenType::Identifier, SplitSemi(token[0])));
			tokenQueue.push(make_pair(TokenType::Separator, ";"));
			token.erase(token.begin());
			end = true;
		}
		else
		{
			tokenQueue.push(make_pair(TokenType::Identifier, token[0]));
			token.erase(token.begin());
			end = true;
		}
		if (!end) {
			expectionOfSelect(token);
		}
	}
	return token;
}


vector<string> PQLLexer::tokenizeAssign(vector<string> token) {
	tokenQueue.push(make_pair(TokenType::Keyword, "assign"));
	token.erase(token.begin());
	bool end = false;
	while (!end) {
		if (tokenContainCommas(token[0])) // first situation
		{
			tokenQueue.push(make_pair(TokenType::Identifier, SplitCommas(token[0])));
			tokenQueue.push(make_pair(TokenType::Separator, ","));
			token.erase(token.begin());
		}
		else if (tokenContainSemi(token[0])) // second situation
		{
			tokenQueue.push(make_pair(TokenType::Identifier, SplitSemi(token[0])));
			tokenQueue.push(make_pair(TokenType::Separator, ";"));
			token.erase(token.begin());
			end = true;
		}
		else
		{
			tokenQueue.push(make_pair(TokenType::Identifier, token[0]));
			token.erase(token.begin());
			end = true;
		}
		if (!end) {
			expectionOfSelect(token);
		}
	}
	return token;
}


vector<string> PQLLexer::tokenizeStmt(vector<string> token) {
	tokenQueue.push(make_pair(TokenType::Keyword, "statement"));
	token.erase(token.begin());
	bool end = false;
	while (!end) {
		if (tokenContainCommas(token[0])) // first situation
		{
			tokenQueue.push(make_pair(TokenType::Identifier, SplitCommas(token[0])));
			tokenQueue.push(make_pair(TokenType::Separator, ","));
			token.erase(token.begin());
		}
		else if (tokenContainSemi(token[0])) // second situation
		{
			tokenQueue.push(make_pair(TokenType::Identifier, SplitSemi(token[0])));
			tokenQueue.push(make_pair(TokenType::Separator, ";"));
			token.erase(token.begin());
			end = true;
		}
		else
		{
			tokenQueue.push(make_pair(TokenType::Identifier, token[0]));
			token.erase(token.begin());
			end = true;
		}
		if (!end) {
			expectionOfSelect(token);
		}
	}
	return token;
}


vector<string> PQLLexer::tokenizeCall(vector<string> token) {
	tokenQueue.push(make_pair(TokenType::Keyword, "call"));
	token.erase(token.begin());
	bool end = false;
	while (!end) {
		if (tokenContainCommas(token[0])) // first situation
		{
			tokenQueue.push(make_pair(TokenType::Identifier, SplitCommas(token[0])));
			tokenQueue.push(make_pair(TokenType::Separator, ","));
			token.erase(token.begin());
		}
		else if (tokenContainSemi(token[0])) // second situation
		{
			tokenQueue.push(make_pair(TokenType::Identifier, SplitSemi(token[0])));
			tokenQueue.push(make_pair(TokenType::Separator, ";"));
			token.erase(token.begin());
			end = true;
		}
		else
		{
			tokenQueue.push(make_pair(TokenType::Identifier, token[0]));
			token.erase(token.begin());
			end = true;
		}
		if (!end) {
			expectionOfSelect(token);
		}
	}
	return token;
}


vector<string> PQLLexer::tokenizeConstant(vector<string> token) {
	tokenQueue.push(make_pair(TokenType::Keyword, "constant"));
	token.erase(token.begin());
	bool end = false;
	while (!end) {
		if (tokenContainCommas(token[0])) // first situation
		{
			tokenQueue.push(make_pair(TokenType::Identifier, SplitCommas(token[0])));
			tokenQueue.push(make_pair(TokenType::Separator, ","));
			token.erase(token.begin());
		}
		else if (tokenContainSemi(token[0])) // second situation
		{
			tokenQueue.push(make_pair(TokenType::Identifier, SplitSemi(token[0])));
			tokenQueue.push(make_pair(TokenType::Separator, ";"));
			token.erase(token.begin());
			end = true;
		}
		else
		{
			tokenQueue.push(make_pair(TokenType::Identifier, token[0]));
			token.erase(token.begin());
			end = true;
		}
		if (!end) {
			expectionOfSelect(token);
		}
	}
	return token;
}

bool PQLLexer::tokenContainCommas(string token) {
  return (token.find(",") != token.npos);
}

bool PQLLexer::tokenContainSemi(string token) {
	return (token.find(";") != token.npos);
}

void PQLLexer::expectionOfSelect(vector<string> token)
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

void PQLLexer::expectionOfDeclaration(vector<string> token) {
	if (token[0] == "Select" || token[0] == "statement" || token[0] == "read" 
		|| token[0] == "print" || token[0] == "while" || token[0] == "if"
		|| token[0] == "assign" || token[0] == "variable" || token[0] == "constant"
		|| token[0] == "procedure") {

	}
	else {
		throw invalid_argument("should follows a new declaration or selection");
	}
}

vector<string> PQLLexer::tokenizeSelect(vector<string> token) {

  tokenQueue.push(make_pair(TokenType::Keyword, "select"));
  token.erase(token.begin());
  // push the select, then for three situations: 1. "a, b". 2. "a;" 3. "a" 
  bool end = false;
  while (!end) {
    if (token[0].find(",") != token[0].npos) // first situation
    {
      tokenQueue.push(make_pair(TokenType::Identifier, SplitCommas(token[0])));
	  target.push(token[0]);
      tokenQueue.push(make_pair(TokenType::Separator, ","));
	  token.erase(token.begin());
    } else if (token[0].find(";") != token[0].npos) // second situation
    {
		tokenQueue.push(make_pair(TokenType::Identifier, SplitSemi(token[0])));
		target.push(token[0]);
		tokenQueue.push(make_pair(TokenType::Separator, ";"));
		token.erase(token.begin());
		end = true;
    } else
    {
		tokenQueue.push(make_pair(TokenType::Identifier, token[0]));
		token.erase(token.begin());

                if (token[0].find(";")!=token[0].npos && token[0][0] == ';') {
			if (token[0].length() == 1) {
				end = true;
				tokenQueue.push(make_pair(TokenType::Separator, ";"));
				token.erase(token.begin());
			}
			else {
				token[0] = token[0].substr(1, token[0].length() - 1);
			}
		}
		else {
			
		}
				end = true;
    }

		

  }
  if (!token.empty())
  expectionOfSelect(token);
  return token;
}


vector<string> PQLLexer::tokenizePattern(vector<string> token) {
	int check = 1;
	int n0 = -1, n1 = -1, n2 = -1, n3, iter = 0;
	string s;
	string first_s, second_s;
	// KEYWORD,"a" "(" (3 situations: v, "v", _) "," "_" or _" adfsf"_ ")" ";"

	while (!token.empty() && check == 1)
	{
		if (token[iter].find(")") != token[iter].npos)
		{
			check = 0;
		}
		iter++;
	}
	iter = iter - 1;
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
		if (s[i] == '(')
		{
			n0 = i;
		}
		if (s[i] == ',')
		{
			n1 = i;
		}
		if (s[i] == ')')
		{
			n2 = i;
		}
	}
	if (n0 != -1 && n1 != -1 && n2 != -1) {
		tokenQueue.push(make_pair(TokenType::Keyword, "pattern"));
		tokenQueue.push(make_pair(TokenType::Identifier, s.substr(7, n0 - 7)));
		tokenQueue.push(make_pair(TokenType::Separator, "("));
		first_s =  s.substr(n0 + 1, n1 - 1 - n0);
		if (first_s.length() == 1)
		{
			if (first_s[0] == '_')
			{
				tokenQueue.push(make_pair(TokenType::Identifier, "_"));

			}
			else {
				tokenQueue.push(make_pair(TokenType::Identifier, first_s.substr(0, 1)));
			}
		} else
		{
		  if (first_s[0] == '\"' && first_s[first_s.length() - 1] == '\"')
		  {
			  tokenQueue.push(make_pair(TokenType::Identifier, first_s));
		  } else
		  {
			  throw invalid_argument("first parameter format error");
		  }
		}
		tokenQueue.push(make_pair(TokenType::Separator, ","));
		second_s = s.substr(n1 + 1, n2 - 1 - n1);
                if (second_s.length() == 1)
                {
                  if (second_s[0] == '_')
                  {
			  tokenQueue.push(make_pair(TokenType::Identifier, "_"));
                  } else
                  {
					  throw invalid_argument("cannot have a variable on the right");
                  }
                } else
                {
                  if (second_s.find('_')!= second_s.npos)
                  {
                    if (second_s[0] == '_' && second_s[second_s.length() - 1] == '_' && second_s[1] == '\"' && second_s[second_s.length() - 2] == '\"')
                    {
			tokenQueue.push(make_pair(TokenType::Identifier, "_"));
			tokenQueue.push(make_pair(TokenType::Separator, "\""));
			tokenQueue.push(make_pair(TokenType::Identifier, second_s.substr(2, second_s.length() - 4)));
			tokenQueue.push(make_pair(TokenType::Separator, "\""));
			tokenQueue.push(make_pair(TokenType::Identifier, "_"));
                    } 
                  }
				  else if (second_s[0] == '\"' && second_s[second_s.length() - 1] == '\"')
				  {
					  tokenQueue.push(make_pair(TokenType::Separator, "\""));
					  tokenQueue.push(make_pair(TokenType::Identifier, second_s.substr(1, second_s.length() - 2)));
					  tokenQueue.push(make_pair(TokenType::Separator, "\""));
				  }
                }
		tokenQueue.push(make_pair(TokenType::Separator, ")"));
	}
	else
	{
		throw "lack , or ; or ( or )";
	}

	if (s.find(';') != s.npos)
	{
		tokenQueue.push(make_pair(TokenType::Separator, ";"));
	}
	else if (token[0].find(";") != token[0].npos)
	{
		tokenQueue.push(make_pair(TokenType::Separator, ";"));
		token.erase(token.begin());
	}
	else
	{
		
		throw invalid_argument("should be end");
		
	}
	return token;


}

vector<string> PQLLexer::tokenizeFollows(vector<string> token) {
  int check = 1;
  int n0 = -1, n1 = -1, n2 = -1, n3, iter = 0;
  string s;
  // KEYWORD, "(" "," ")" ";"

   while (!token.empty() && check == 1)
   {
     if (token[iter].find(")")!= token[iter].npos)
     {
		 check = 0;
     }
	 iter++;
   }
   iter = iter - 1;
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
		if (s[i] == '(')
		{
			n0 = i;
		}
		if (s[i] == ',')
		{
			n1 = i;
		}
		if (s[i] == ')')
		{
			n2 = i;
		}
	}
			  if (n0 != -1 && n1 != -1 && n2 != -1) {
				  tokenQueue.push(make_pair(TokenType::Keyword, "follows"));
				  tokenQueue.push(make_pair(TokenType::Separator, "("));
				  tokenQueue.push(make_pair(TokenType::Identifier, s.substr(n0 + 1, n1 - 1 - n0)));
				  tokenQueue.push(make_pair(TokenType::Separator, ","));
				  tokenQueue.push(make_pair(TokenType::Identifier, s.substr(n1 + 1, n2 - 1 - n1)));
				  tokenQueue.push(make_pair(TokenType::Separator, ")"));
			  } else
			  {
				  throw invalid_argument("lack , or ; or ( or )");
			  }
            
  if(s.find(";") != s.npos)
  {
	  tokenQueue.push(make_pair(TokenType::Separator, ";"));
  } else if(token[0].find(";") != token[0].npos)
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
		throw invalid_argument("should be ; or pattern");
    }
  }
  return token;
         
  
}


vector<string> PQLLexer::tokenizeFollowsT(vector<string> token) {
	int check = 1;
	int n0 = -1, n1 = -1, n2 = -1, n3, iter = 0;
	string s;
	// KEYWORD, "(" "," ")" ";"

	while (!token.empty() && check == 1)
	{
		if (token[iter].find(")") != token[iter].npos)
		{
			check = 0;
		}
		iter++;
	}
	iter = iter - 1;
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
		if (s[i] == '(')
		{
			n0 = i;
		}
		if (s[i] == ',')
		{
			n1 = i;
		}
		if (s[i] == ')')
		{
			n2 = i;
		}
	}
	if (n0 != -1 && n1 != -1 && n2 != -1) {
		tokenQueue.push(make_pair(TokenType::Keyword, "follows*"));
		tokenQueue.push(make_pair(TokenType::Separator, "("));
		tokenQueue.push(make_pair(TokenType::Identifier, s.substr(n0 + 1, n1 - 1 - n0)));
		tokenQueue.push(make_pair(TokenType::Separator, ","));
		tokenQueue.push(make_pair(TokenType::Identifier, s.substr(n1 + 1, n2 - 1 - n1)));
		tokenQueue.push(make_pair(TokenType::Separator, ")"));
	}
	else
	{
		throw invalid_argument("lack , or ; or ( or )");
	}

	if (s.find(";") != s.npos)
	{
		tokenQueue.push(make_pair(TokenType::Separator, ";"));
	}
	else if (token[0].find(";") != token[0].npos)
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
			throw invalid_argument("should be ; or pattern");
		}
	}
	return token;


}


vector<string> PQLLexer::tokenizeParent(vector<string> token) {
	int check = 1;
	int n0 = -1, n1 = -1, n2 = -1, n3, iter = 0;
	string s;
	// KEYWORD, "(" "," ")" ";"

	while (!token.empty() && check == 1)
	{
		if (token[iter].find(")") != token[iter].npos)
		{
			check = 0;
		}
		iter++;
	}
	iter = iter - 1;
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
		if (s[i] == '(')
		{
			n0 = i;
		}
		if (s[i] == ',')
		{
			n1 = i;
		}
		if (s[i] == ')')
		{
			n2 = i;
		}
	}
	if (n0 != -1 && n1 != -1 && n2 != -1) {
		tokenQueue.push(make_pair(TokenType::Keyword, "parent"));
		tokenQueue.push(make_pair(TokenType::Separator, "("));
		tokenQueue.push(make_pair(TokenType::Identifier, s.substr(n0 + 1, n1 - 1 - n0)));
		tokenQueue.push(make_pair(TokenType::Separator, ","));
		tokenQueue.push(make_pair(TokenType::Identifier, s.substr(n1 + 1, n2 - 1 - n1)));
		tokenQueue.push(make_pair(TokenType::Separator, ")"));
	}
	else
	{
		throw invalid_argument("lack , or ; or ( or )");
	}

	if (s.find(";") != s.npos)
	{
		tokenQueue.push(make_pair(TokenType::Separator, ";"));
	}
	else if (token[0].find(";") != token[0].npos)
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
			throw invalid_argument("should be ; or pattern");
		}
	}
	return token;


}


vector<string> PQLLexer::tokenizeParentT(vector<string> token) {
	int check = 1;
	int n0 = -1, n1 = -1, n2 = -1, n3, iter = 0;
	string s;
	// KEYWORD, "(" "," ")" ";"

	while (!token.empty() && check == 1)
	{
		if (token[iter].find(")") != token[iter].npos)
		{
			check = 0;
		}
		iter++;
	}
	iter = iter - 1;
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
		if (s[i] == '(')
		{
			n0 = i;
		}
		if (s[i] == ',')
		{
			n1 = i;
		}
		if (s[i] == ')')
		{
			n2 = i;
		}
	}
	if (n0 != -1 && n1 != -1 && n2 != -1) {
		tokenQueue.push(make_pair(TokenType::Keyword, "parent*"));
		tokenQueue.push(make_pair(TokenType::Separator, "("));
		tokenQueue.push(make_pair(TokenType::Identifier, s.substr(n0 + 1, n1 - 1 - n0)));
		tokenQueue.push(make_pair(TokenType::Separator, ","));
		tokenQueue.push(make_pair(TokenType::Identifier, s.substr(n1 + 1, n2 - 1 - n1)));
		tokenQueue.push(make_pair(TokenType::Separator, ")"));
	}
	else
	{
		throw invalid_argument("lack , or ; or ( or )");
	}

	if (s.find(";") != s.npos)
	{
		tokenQueue.push(make_pair(TokenType::Separator, ";"));
	}
	else if (token[0].find(";") != token[0].npos)
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
			throw invalid_argument("should be ; or pattern");
		}
	}
	return token;


}

vector<string> PQLLexer::tokenizeUses(vector<string> token) {
	int check = 1;
	int n0 = -1, n1 = -1, n2 = -1, n3, iter = 0;
	string s;
	string second_s;
	// KEYWORD, "(" "," ")" ";"

	while (!token.empty() && check == 1)
	{
		if (token[iter].find(")") != token[iter].npos)
		{
			check = 0;
		}
		iter++;
	}
	iter = iter - 1;
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
		if (s[i] == '(')
		{
			n0 = i;
		}
		if (s[i] == ',')
		{
			n1 = i;
		}
		if (s[i] == ')')
		{
			n2 = i;
		}
	}
	if (n0 != -1 && n1 != -1 && n2 != -1) {
		tokenQueue.push(make_pair(TokenType::Keyword, "uses"));
		tokenQueue.push(make_pair(TokenType::Separator, "("));
		tokenQueue.push(make_pair(TokenType::Identifier, s.substr(n0 + 1, n1 - 1 - n0)));
		tokenQueue.push(make_pair(TokenType::Separator, ","));
		second_s = s.substr(n1 + 1, n2 - 1 - n1);
		if (second_s.length() == 1)
		{

			tokenQueue.push(make_pair(TokenType::Identifier, second_s));


		}
		else
		{
			if (second_s[0] == '\"' && second_s[second_s.length() - 1] == '\"')
			{
				tokenQueue.push(make_pair(TokenType::Separator, "\""));
				tokenQueue.push(make_pair(TokenType::Identifier, second_s.substr(1, second_s.length() - 2)));
				tokenQueue.push(make_pair(TokenType::Separator, "\""));
			}
		}
		tokenQueue.push(make_pair(TokenType::Separator, ")"));
	}
	else
	{
		throw invalid_argument("lack , or ; or ( or )");
	}

	if (s.find(";") != s.npos)
	{
		tokenQueue.push(make_pair(TokenType::Separator, ";"));
	}
	else if (token[0].find(";") != token[0].npos)
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
			throw invalid_argument("should be ; or pattern");
		}
	}
	return token;


}


vector<string> PQLLexer::tokenizeModifies(vector<string> token) {
	int check = 1;
	int n0 = -1, n1 = -1, n2 = -1, n3, iter = 0;
	string s;
	string second_s;
	// KEYWORD, "(" "," ")" ";"

	while (!token.empty() && check == 1)
	{
		if (token[iter].find(")") != token[iter].npos)
		{
			check = 0;
		}
		iter++;
	}
	iter = iter - 1;
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
		if (s[i] == '(')
		{
			n0 = i;
		}
		if (s[i] == ',')
		{
			n1 = i;
		}
		if (s[i] == ')')
		{
			n2 = i;
		}
	}
	if (n0 != -1 && n1 != -1 && n2 != -1) {
		tokenQueue.push(make_pair(TokenType::Keyword, "modifies"));
		tokenQueue.push(make_pair(TokenType::Separator, "("));
		tokenQueue.push(make_pair(TokenType::Identifier, s.substr(n0 + 1, n1 - 1 - n0)));
		tokenQueue.push(make_pair(TokenType::Separator, ","));
		second_s = s.substr(n1 + 1, n2 - 1 - n1);
		if (second_s.length() == 1)
		{

			tokenQueue.push(make_pair(TokenType::Identifier, second_s));
			

		}
		else
		{
                if (second_s[0] == '\"' && second_s[second_s.length() - 1] == '\"')
			{
				tokenQueue.push(make_pair(TokenType::Separator, "\""));
				tokenQueue.push(make_pair(TokenType::Identifier, second_s.substr(1, second_s.length() - 2)));
				tokenQueue.push(make_pair(TokenType::Separator, "\""));
			}
		}
		tokenQueue.push(make_pair(TokenType::Separator, ")"));
	}
	else
	{
		throw invalid_argument("lack , or ; or ( or )");
	}

	if (s.find(";") != s.npos)
	{
		tokenQueue.push(make_pair(TokenType::Separator, ";"));
	}
	else if (token[0].find(";") != token[0].npos)
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
			throw invalid_argument("should be ; or pattern");
		}
	}
	return token;


}
