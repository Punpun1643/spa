#pragma once

#include <unordered_map>
#include <unordered_set>

#include "../utils/UFDS.h"
#include "BaseTable.h"

class DictionaryTable : public BaseTable {
  std::unordered_map<std::string, std::unordered_set<std::string>> relations =
      {};

 public:
  DictionaryTable();
  ~DictionaryTable() = default;
  void insert(std::string i, std::string j) override;
  bool isRelated(std::string i, std::string j) override;
};