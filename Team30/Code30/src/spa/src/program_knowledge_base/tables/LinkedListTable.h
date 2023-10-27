#pragma once

#include <memory>
#include <queue>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "../utils/LinkedList.h"
#include "BaseTable.h"

class LinkedListTable : public BaseTable {
  LinkedList relations = LinkedList();

 public:
  LinkedListTable();
  ~LinkedListTable() = default;

  void Insert(std::string i, std::string j) override;
  bool IsEmpty() override;

  bool IsRelated(std::string i, std::string j) override;
  bool HasRelations(std::string val) override;
  bool HasInverseRelations(std::string val) override;

  std::unordered_set<std::string> GetAllWithRelations(
      std::shared_ptr<std::unordered_set<std::string>> vals) override;
  std::unordered_set<std::string> GetAllWithInverseRelations(
      std::shared_ptr<std::unordered_set<std::string>> vals) override;
  std::unordered_set<std::string> GetAllRelatedToValue(
      std::string val) override;
  std::unordered_set<std::string> GetAllInverseRelatedToValue(
      std::string val) override;
};
