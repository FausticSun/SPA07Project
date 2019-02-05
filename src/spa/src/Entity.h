#pragma once

#include <string>
#include <tuple>

enum EntityType {
  STMT,
  READ,
  PRINT,
  CALL,
  WHILE,
  IF,
  ASSIGN,
  VARIABLE,
  CONSTANT,
  PROCEDURE
};

struct Entity {
  EntityType type{STMT};
  std::string name;
  Entity() = default;
  bool operator<(const Entity &other) const {
    return std::tie(type, name) < std::tie(other.type, other.name);
  }
};
