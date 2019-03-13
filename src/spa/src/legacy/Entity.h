#pragma once

#include <string>
#include <tuple>

enum EntityType {
  Stmt,
  Read,
  Print,
  Call,
  While,
  If,
  Assign,
  Variable,
  Constant,
  Procedure
};

struct Entity {
  EntityType type{Stmt};
  std::string name;
  Entity() = default;
  bool operator<(const Entity &other) const {
    return std::tie(type, name) < std::tie(other.type, other.name);
  }
};

namespace std {
template <> struct hash<Entity> {
  std::size_t operator()(Entity const &e) const noexcept {
    return std::hash<std::string>{}(e.name);
  }
};
} // namespace std
