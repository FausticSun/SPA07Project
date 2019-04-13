#pragma once
#include "PKB.h"
#include <deque>
#include <list>
#include <memory>
#include <string>

class SPA {
private:
  std::unique_ptr<PKB> pkb;

public:
  SPA();
  void parseSIMPLEFile(const std::string &filename);
  void evaluateQuery(const std::string &query,
                     std::list<std::string> &results) const;
};
