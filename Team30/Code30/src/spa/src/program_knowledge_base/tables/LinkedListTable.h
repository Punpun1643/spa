#pragma once

#include <queue>
#include <unordered_map>
#include <unordered_set>

#include "../utils/LinkedList.h"
#include "BaseTable.h"

class LinkedListTable : public BaseTable {
  LinkedList relations = LinkedList();

 public:
  LinkedListTable();
  ~LinkedListTable() = default;

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
};