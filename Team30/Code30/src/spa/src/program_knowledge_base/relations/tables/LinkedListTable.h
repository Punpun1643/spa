#pragma once

#include <memory>
#include <queue>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "../../utils/LinkedList.h"
#include "RelationTable.h"

class LinkedListTable : public RelationTable {
  LinkedList relations = LinkedList();
  bool empty = true;

 public:
  LinkedListTable();
  ~LinkedListTable() = default;

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
