#pragma once
#include "Token.h"
#include <string>
#include <vector>

class Lexer {
public:
  std::vector<Token *> tokenize(std::string line);
};
