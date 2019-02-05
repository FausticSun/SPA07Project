#pragma once
#include "PKB.h"

class SPA {
private:
  PKB PKB;

public:
  void parseSIMPLEFile(std::string filename);
  const std::list<std::string> evaluateQuery(std::string query) const;
};
