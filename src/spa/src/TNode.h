#pragma once

#include <memory>
#include <string>
#include <utility>
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
  explicit TNode(const TNodeType type, std::string name = "",
                 std::vector<std::unique_ptr<TNode>> children = {})
      : type(type), name(std::move(name)), children(std::move(children)){};
  explicit TNode(const TNodeType type,
                 std::vector<std::unique_ptr<TNode>> children = {},
                 std::string name = "")
      : type(type), name(std::move(name)), children(std::move(children)){};
  TNodeType type;
  std::string name;
  std::vector<std::unique_ptr<TNode>> children;
};
