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
  bool isRelated(std::string i, std::string j) override;
  bool isEmpty() override;

  std::unordered_set<std::string> getAllRelated(
      std::shared_ptr<std::unordered_set<std::string>> vals) override;
  std::unordered_set<std::string> getAllInverseRelated(
      std::shared_ptr<std::unordered_set<std::string>> vals) override;
};