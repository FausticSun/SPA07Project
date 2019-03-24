#include "RelationTable.h"

void RelationTable::setRelation(std::string s, std::string t) {
  this->forwardMap[s].insert(t);
  this->reverseMap[t].insert(s);
}

bool RelationTable::hasForwardRelation(std::string s, std::string t) {
  return this->forwardMap.find(s) == this->forwardMap.end()
             ? false
             : this->forwardMap[s].find(t) != this->forwardMap[s].end();
}

bool RelationTable::hasReverseRelation(std::string s, std::string t) {
  return this->reverseMap.find(s) == this->reverseMap.end()
             ? false
             : this->reverseMap[s].find(t) != this->reverseMap[s].end();
}

std::set<std::string> RelationTable::getForwardRelations(std::string s) {
  return this->forwardMap.find(s) == this->forwardMap.end()
             ? std::set<std::string>()
             : this->forwardMap[s];
}

std::set<std::string> RelationTable::getReverseRelations(std::string s) {
  return this->reverseMap.find(s) == this->reverseMap.end()
             ? std::set<std::string>()
             : this->reverseMap[s];
}

std::vector<std::vector<std::string>> RelationTable::getTable() {
  std::vector<std::vector<std::string>> table;
  for (auto sIt = this->forwardMap.begin(); sIt != this->forwardMap.end();
       ++sIt) {
    for (auto tIt = sIt->second.begin(); tIt != sIt->second.end(); tIt++) {
      table.push_back({sIt->first, (*tIt)});
    }
  }
  return table;
}
