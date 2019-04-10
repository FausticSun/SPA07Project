#pragma once
#include "PKB.h"
#include <deque>
#include <memory>
#include <string>

class SPA {
private:
  std::unique_ptr<PKB> pkb;

public:
  SPA();
  void parseSIMPLEFile(std::string filename);
  const std::deque<std::string>& evaluateQuery(std::string query) const;
};
