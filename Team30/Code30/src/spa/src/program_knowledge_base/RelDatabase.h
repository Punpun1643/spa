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
  bool isEmpty(RelationType type);

  bool isRelated(RelationType type, std::string val1, std::string val2);
  bool hasRelations(RelationType type, std::string val);
  bool hasInverseRelations(RelationType type, std::string val);

  std::unordered_set<std::string> getAllWithRelations(
      RelationType type, std::shared_ptr<std::unordered_set<std::string>> vals);
  std::unordered_set<std::string> getAllWithInverseRelations(
      RelationType type, std::shared_ptr<std::unordered_set<std::string>> vals);

  std::unordered_set<std::string> getAllRelatedToValue(
      RelationType type, std::string val);
};