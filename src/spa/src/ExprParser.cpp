#include "ExprParser.h"

using Lexer::Token;
using Lexer::TokenType;

namespace Parser {
bool ExprTokens::isArithmeticOp(Token t) {
  return ArithmeticOperators.find(t) != ArithmeticOperators.end();
}

bool ExprTokens::isRelationalOp(Token t) {
  return RelationalOperators.find(t) != RelationalOperators.end();
}

std::list<Token> parseExpr(std::list<Token> &tokens) {
  // Ensures there are parentheses after ! and surrounding && and ||
  for (auto it = tokens.begin(); it != tokens.end(); ++it) {
    if ((*it) == ExprTokens::Not || (*it) == ExprTokens::And ||
        (*it) == ExprTokens::Or) {
      if (std::next(it) == tokens.end() ||
          *std::next(it) != ExprTokens::LeftParentheses) {
        throw std::logic_error(it->value + " must be followed by (");
      }
    }
    if ((*it) == ExprTokens::And || (*it) == ExprTokens::Or) {
      if (std::prev(it) == tokens.begin() ||
          *std::prev(it) != ExprTokens::RightParentheses) {
        throw std::logic_error(it->value + " must be preceded by )");
      }
    }
  }

  // Implements the shunting-yard algorithm
  std::list<Token> input = tokens;
  std::list<Token> queue; // Output queue
  std::list<Token> stack; // Operator stack
  // While there are more tokens to read
  while (!input.empty()) {
    // Read a token
    Token t = input.front();
    input.pop_front();
    // Push number/identifier to output queue
    if (t.type == TokenType::Number || t.type == TokenType::Identifier) {
      queue.push_back(t);
    }
    // Push function to operator stack
    // In this case only ! is a "function"
    else if (t == ExprTokens::Not) {
      stack.push_back(t);
    }
    // Handle operator token
    else if (t.type == TokenType::Operator) {
      // While (operator at the top of the stack is not a parenthesis)
      // and ((function is at the top of the stack)
      // or (operator at the top of the stack has greater or equal
      // precedence))
      while (!stack.empty() && stack.back() != ExprTokens::LeftParentheses &&
             (stack.back() == ExprTokens::Not ||
              precedenceMap[stack.back()] >= precedenceMap[t])) {
        // Pop the operator stack into the queue
        queue.push_back(stack.back());
        stack.pop_back();
      }
      // Push the operator into the operator stack
      stack.push_back(t);
    } // Push left parentheses into the operator stack
    else if (t == ExprTokens::LeftParentheses) {
      stack.push_back(t);
    }
    // Handle right parentheses
    else if (t == ExprTokens::RightParentheses) {
      // Pop the operator stack into the queue until stack is empty or left
      // parentheses is encountered
      while (!stack.empty() && stack.back() != ExprTokens::LeftParentheses) {
        queue.push_back(stack.back());
        stack.pop_back();
      }
      // If stack runs out without finding a left parentheses, then there are
      // mismatched parentheses
      if (stack.empty()) {
        throw std::logic_error("Expression contains mismatched parentheses");
      }
      // Discard the left parentheses
      if (stack.back() == ExprTokens::LeftParentheses) {
        stack.pop_back();
      }
    } else {
      throw std::logic_error("Unexpected token while parsing expression");
    }
  }
  while (!stack.empty()) {
    // Pop the entire operator stack into the queue
    Token t = stack.back();
    stack.pop_back();
    // If there are any parentheses left in the stack, then there are mismatched
    // parentheses
    if (t == ExprTokens::LeftParentheses) {
      throw std::logic_error("Expression contains mismatched parentheses");
    }
    queue.push_back(t);
  }
  return queue;
}

std::string tokensToString(const std::list<Token> &tokens) {
  std::string str = " ";
  for (auto t : tokens) {
    str.append(t.value + " ");
  }
  return str;
}
} // namespace Parser
