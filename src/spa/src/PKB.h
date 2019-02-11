#pragma once

#include "Relation.h"
#include <map>
#include <set>
#include <string>

class PKB {
private:
  std::set<std::string> varTable;
  std::set<std::string> procTable;
  std::map<Entity, std::set<Entity>> followsTable;
  std::map<Entity, std::set<Entity>> followedByTable;
  std::map<Entity, std::set<Entity>> parentTable;
  std::map<Entity, std::set<Entity>> parentOfTable;
  std::map<Entity, std::set<Entity>> usesTable;
  std::map<Entity, std::set<Entity>> usedByTable;
  std::map<Entity, std::set<Entity>> modifiesTable;
  std::map<Entity, std::set<Entity>> modifiedByTable;

public:
  PKB();
  void insertVar(std::string);
  void insertProc(std::string);
  void setFollows(Entity, Entity);
  void setParent(Entity, Entity);
  void setUsed(Entity, Entity);
  void setModified(Entity, Entity);

  const std::set<std::string> getVarTable() const;
  const std::set<std::string> getProcTable() const;
};
