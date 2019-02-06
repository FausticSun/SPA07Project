#pragma once

#include <queue>
#include <string>

using namespace std;

/*DeclarationType {
        Variable,
        Procedure

};*/

/*{
        Follows,
        Follows*,
        Parent,
        Parent*,
        UsesS,
        UsesP,
        ModifiesS,
        ModifiesP
};*/
enum class TokenType {
  Identifier,
  Keyword,
  Separator,
  Operator,
  Literal,
};

class PQLParser {
public:
  PQLParser(string input);
  queue<pair<TokenType, string>> getDeclarationQueue();
  queue<pair<TokenType, string>> getSelectQueue();

private:
  queue<pair<TokenType, string>> declarationQueue;
  queue<pair<TokenType, string>> selectQueue;
  void Tokenize(string input);
  vector<string> vectorize(string);
  void tokenizeVariable(vector<string>);
  void tokenizeProcedure(vector<string>);
  void tokenizeSelect(vector<string>);
  void tokenizePattern(vector<string>);
  void tokenizeParent(vector<string>);
  void tokenizeParentS(vector<string>);
  void tokenizeFollows(vector<string>);
  void tokenizeFollowsS(vector<string>);
  void tokenizeModifies(vector<string>);
  void tokenizeUses(vector<string>);
};
