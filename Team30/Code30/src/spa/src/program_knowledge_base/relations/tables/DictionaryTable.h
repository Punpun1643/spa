#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "RelationTable.h"

class DictionaryTable : public RelationTable {
  std::unordered_map<std::string, std::unordered_set<std::string>> relations =
      {};
  std::unordered_map<std::string, std::unordered_set<std::string>>
      inverse_relations = {};

 public:
  DictionaryTable();
  ~DictionaryTable() = default;

  void Insert(std::string i, std::string j) override;
  bool IsEmpty() override;

  bool IsRelated(std::string i, std::string j) override;
  bool HasRelations(std::string val) override;
  bool HasInverseRelations(std::string val) override;

  std::unordered_set<std::string> GetAllWithRelations(
      std::unordered_set<std::string> vals) override;
  std::unordered_set<std::string> GetAllWithInverseRelations(
      std::unordered_set<std::string> vals) override;
  std::unordered_set<std::string> GetAllRelatedToValue(
      std::string val) override;
  std::unordered_set<std::string> GetAllInverseRelatedToValue(
      std::string val) override;
};
