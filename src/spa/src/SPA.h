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
  void parseSIMPLEFile(const std::string &filename);
  std::deque<std::string> evaluateQuery(const std::string &query) const;
};
