#pragma once

#include "Entity.h"
#include <tuple>

enum RelationType { FOLLOWS, PARENT, USES, MODIFIES };

struct Relation {
  RelationType type{FOLLOWS};
  Entity leftEntity;
  Entity rightEntity;
  Relation() = default;
  bool operator<(const Relation &other) const {
    return std::tie(type, leftEntity, rightEntity) <
           std::tie(other.type, other.leftEntity, other.rightEntity);
  }
};
