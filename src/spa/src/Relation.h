#pragma once

#include "Entity.h"

enum RelationType { FOLLOWS, PARENT, USES, MODIFIES };

struct Relation {
  RelationType type{FOLLOWS};
  Entity leftEntity;
  Entity rightEntity;
  Relation() = default;
};
