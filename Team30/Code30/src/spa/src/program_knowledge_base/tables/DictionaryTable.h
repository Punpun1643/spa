#pragma once

#include <unordered_map>
#include <unordered_set>

#include "../utils/UFDS.h"
#include "BaseTable.h"

class DictionaryTable : public BaseTable {
  std::unordered_map<std::string, std::unordered_set<std::string>> relations =
      {};
  std::unordered_map<std::string, std::unordered_set<std::string>>
      inverse_relations = {};

 public:
  DictionaryTable();
  ~DictionaryTable() = default;

  void insert(std::string i, std::string j) override;
  bool isEmpty() override;

  bool isRelated(std::string i, std::string j) override;
  bool hasRelations(std::string val) override;
  bool hasInverseRelations(std::string val) override;

  std::unordered_set<std::string> getAllWithRelations(
      std::shared_ptr<std::unordered_set<std::string>> vals) override;
  std::unordered_set<std::string> getAllWithInverseRelations(
      std::shared_ptr<std::unordered_set<std::string>> vals) override;
  std::unordered_set<std::string> getAllRelatedToValue(
      std::string val) override;
  std::unordered_set<std::string> getAllInverseRelatedToValue(
      std::string val) override;
};
