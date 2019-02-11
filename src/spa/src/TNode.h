#pragma once

#include <string>
#include <vector>

enum class TNodeType {
  Program,
  Procedure,
  StatementList,
  Assign,
  Call,
  Print,
  Read,
  If,
  Then,
  Else,
  While,
  Variable,
  Constant,
  SemiColon,
  OpenCurlyBrace,
  CloseCurlyBrace,
  Plus,
  Minus,
  Multiply,
  Divide,
  Mod,
  Greater,
  GreaterThanOrEqual,
  Lesser,
  LesserThanOrEqual,
  Equal,
  NotEqual,
  Not,
  And,
  Or
};

class TNode {
public:
  TNode(TNodeType, const std::string & = "");
  ~TNode();
  TNodeType getType();
  string getName();
  int getStatementNumber();
  vector<TNode *> getChildren();
  void setChildren(vector<TNode *>);
  TNodeType type;
  std::string name;
  std::vector<TNode *> children;
};
