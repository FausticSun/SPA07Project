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

struct TNode {
  explicit TNode(const TNodeType type, const std::string &name = "",
                 std::vector<std::unique_ptr<TNode>> &&children = {})
      : type(type), name(name), children(children){};
  explicit TNode(const TNodeType type,
                 std::vector<std::unique_ptr<TNode>> &&children = {},
                 const std::string &name = "")
      : type(type), name(name), children(children){};
  TNodeType type;
  std::string name;
  std::vector<std::unique_ptr<TNode>> children;
};
