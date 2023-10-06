#pragma once

#include <unordered_map>

#include "../utils/UFDS.h"
#include "BaseTable.h"

class UFDSTable : public BaseTable {
  UFDS relations = UFDS();
  bool empty = true;

 public:
  UFDSTable();
  ~UFDSTable() = default;
  void insert(std::string i, std::string j) override;
  bool isRelated(std::string i, std::string j) override;
  bool isEmpty() override;

  std::unordered_set<std::string> getAllRelated(
      std::shared_ptr<std::unordered_set<std::string>> vals) override;
  std::unordered_set<std::string> getAllInverseRelated(
      std::shared_ptr<std::unordered_set<std::string>> vals) override;
};