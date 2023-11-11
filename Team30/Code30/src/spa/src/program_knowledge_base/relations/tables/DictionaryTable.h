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

  void Insert(std::string const& i, std::string const& j) override;
  bool IsEmpty() override;

  bool IsRelated(std::string const& i, std::string const& j) override;
  bool HasRelations(std::string const& val) override;
  bool HasInverseRelations(std::string const& val) override;

  std::unordered_set<std::string> GetAllWithRelations(
      std::unordered_set<std::string> const& vals) override;
  std::unordered_set<std::string> GetAllWithInverseRelations(
      std::unordered_set<std::string> const& vals) override;
  std::unordered_set<std::string> GetAllRelatedToValue(
      std::string val) override;
  std::unordered_set<std::string> GetAllInverseRelatedToValue(
      std::string val) override;
};
