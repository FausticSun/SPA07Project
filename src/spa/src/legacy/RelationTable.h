#pragma once
#include <map>
#include <set>
#include <string>
#include <vector>

class RelationTable {
private:
  std::map<std::string, std::set<std::string>> forwardMap;
  std::map<std::string, std::set<std::string>> reverseMap;

public:
  void setRelation(std::string, std::string);
  bool hasForwardRelation(std::string, std::string);
  bool hasReverseRelation(std::string, std::string);
  std::set<std::string> getForwardRelations(std::string);
  std::set<std::string> getReverseRelations(std::string);
  std::vector<std::vector<std::string>> getTable();
};
