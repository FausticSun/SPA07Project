#include "RelationTable.h"

void RelationTable::setRelation(std::string s, std::string t) {
  this->forwardMap[s].insert(t);
  this->reverseMap[t].insert(s);
}

bool RelationTable::hasForwardRelation(std::string s, std::string t) {
  return this->forwardMap.find(s) == std::map::end
             ? false
             : this->forwardMap[s].find(t) != std::set::end;
}

bool RelationTable::hasReverseRelation(std::string s, std::string t) {
  return this->reverseMap.find(s) == std::map::end
             ? false
             : this->reverseMap[s].find(t) != std::set::end;
}

std::set<std::string> RelationTable::getForwardRelation(std::string s) {
  return this->forwardMap.find(s) == std::map::end ? {} : this->forwardMap[s];
}

std::set<std::string> RelationTable::getReverseRelation(std::string s) {
  return this->reverseMap.find(s) == std::map::end ? {} : this->reverseMap[s];
}

std::vector<std::vector<std::string>> RelationTable::getTable() {
  std::vector<std::vector<std::string>> table;
  for (auto sIt = this->forwardMap.begin(); sIt != this->forwardMap.end();
       ++sIt) {
    for (auto tIt = sIt->second.begin(); tIt != sIt->second.end(); tIt++) {
      table.emplace_back(sIt->first, (*tIt));
    }
  }
  return table;
}
