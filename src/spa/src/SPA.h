#pragma once
#include "PKB.h"
#include <list>
#include <string>

class SPA {
private:
  PKB pkb;

public:
  void parseSIMPLEFile(std::string filename);
  const std::list<std::string> evaluateQuery(std::string query) const;
};
