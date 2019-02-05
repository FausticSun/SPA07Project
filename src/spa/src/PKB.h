#pragma once

#include "Relation.h"
#include <set>
#include <string>

class PKB {
private:
  std::set<std::string> varTable;
  std::set<std::string> procTable;
  std::set<Relation> relTable;

public:
  PKB();
  void setPKB(const std::set<std::string> &variableTable = {},
              const std::set<std::string> &procedureTable = {},
              const std::set<Relation> &relationTable = {});
  const std::set<std::string> getVarTable() const;
  const std::set<std::string> getProcTable() const;
  const std::set<Relation> getRelTable() const;
};
