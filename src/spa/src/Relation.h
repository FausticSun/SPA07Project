#pragma once

#include "Entity.h"

enum RelationType { FOLLOWS, PARENT, USES, MODIFIES };

struct Relation {
  RelationType type;
  Entity leftEntity;
  Entity rightEntity;
};
