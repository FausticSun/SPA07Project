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
  std::string getName();
  std::vector<TNode *> getChildren();
  void setChildren(std::vector<TNode *>);

private:
  TNodeType type;
  std::string name;
  std::vector<TNode *> children;
};
