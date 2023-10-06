#pragma once

#include <memory>
#include <unordered_map>

#include "RelationType.h"
#include "tables/BaseTable.h"
#include "tables/DictionaryTable.h"
#include "tables/LinkedListTable.h"

class RelDatabase {
  std::unordered_map<RelationType, std::shared_ptr<BaseTable>> relationships =
      {};

 public:
  RelDatabase();
  ~RelDatabase() = default;
  void insert(RelationType type, std::string val1, std::string val2);
  bool isRelated(RelationType type, std::string val1, std::string val2);
  bool isEmpty(RelationType type);
};