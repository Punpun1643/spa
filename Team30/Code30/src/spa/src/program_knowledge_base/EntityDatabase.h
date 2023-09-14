#pragma once

#include <stdio.h>

#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "../query_processing_system/common/EntityType.h"

using namespace std;

class EntityDatabase {
  unordered_map<EntityType, unordered_set<string>> entities;
  unordered_set<string> statement_numbers;
  unordered_set<EntityType> entityTypesThatStoreStatementNumbers;

 public:
  EntityDatabase();
  ~EntityDatabase() = default;
  void insert(EntityType type, string value);
  unordered_set<string> get(EntityType type);
  unordered_set<string> getAllStatements();
};