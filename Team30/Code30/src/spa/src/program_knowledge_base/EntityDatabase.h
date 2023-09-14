#pragma once

#include <stdio.h>

#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "../query_processing_system/common/EntityType.h"
#include "utils/EntityValueType.h"

using namespace std;

class EntityDatabase {
  unordered_map<EntityType, unordered_set<string>> entities;
  unordered_map<EntityValueType, unordered_set<string>> entities_by_value_type;
  unordered_map<EntityType, EntityValueType> entityToValueType;

 public:
  EntityDatabase();
  ~EntityDatabase() = default;
  void insert(EntityType type, string value);
  unordered_set<string> get(EntityType type);
  unordered_set<string> getByValueType(EntityValueType type);
  EntityValueType getEntityValueType(EntityType type);
};