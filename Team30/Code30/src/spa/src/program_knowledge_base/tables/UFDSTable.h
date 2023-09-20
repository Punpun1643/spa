#pragma once

#include <unordered_map>

#include "../utils/UFDS.h"
#include "BaseTable.h"

class UFDSTable : public BaseTable {
  UFDS relations = UFDS(100);
  bool empty = true;

 public:
  UFDSTable();
  ~UFDSTable() = default;
  void insert(std::string i, std::string j) override;
  bool isRelated(std::string i, std::string j) override;
  bool isEmpty() override;
};