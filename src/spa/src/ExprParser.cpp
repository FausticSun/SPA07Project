#include "ExprParser.h"

namespace Parser {
bool ExprTokens::isArithmeticOp(Token t) {
  return ArithmeticOperators.find(t) != ArithmeticOperators.end();
}

bool ExprTokens::isRelationalOp(Token t) {
  return RelationalOperators.find(t) != RelationalOperators.end();
}

std::list<Token> parseExpr(std::list<Token> &tokens) { return tokens; }

std::string tokensToString(const std::list<Token> &tokens) {
  std::string str;
  for (auto t : tokens) {
    str.append(t.value);
  }
  return str;
}
} // namespace Parser
