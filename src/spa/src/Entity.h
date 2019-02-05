#pragma once

#include <string>

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
  std::string name;
  EntityType type;
};
