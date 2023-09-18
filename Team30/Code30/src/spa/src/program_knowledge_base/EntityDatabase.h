#pragma once

#include <stdio.h>

#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "../query_processing_system/common/EntityType.h"

class EntityDatabase {
  std::unordered_map<EntityType,
                     std::shared_ptr<std::unordered_set<std::string>>>
      entities;
  std::unordered_set<EntityType> statementTypes;

 public:
  EntityDatabase();
  ~EntityDatabase() = default;
  void insert(EntityType type, std::string value);
  std::shared_ptr<std::unordered_set<std::string>> get(EntityType type);
};