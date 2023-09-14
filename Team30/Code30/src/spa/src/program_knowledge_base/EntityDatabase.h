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
  unordered_map<EntityType, shared_ptr<unordered_set<string>>> entities;
  shared_ptr<unordered_set<string>> statement_numbers;
  unordered_set<EntityType> entityTypesThatStoreStatementNumbers;

 public:
  EntityDatabase();
  ~EntityDatabase() = default;
  void insert(EntityType type, string value);
  shared_ptr<unordered_set<string>> get(EntityType type);
  shared_ptr<unordered_set<string>> getAllStatements();
};