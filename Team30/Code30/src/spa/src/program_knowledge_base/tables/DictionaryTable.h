#pragma once

#include <unordered_map>
#include <unordered_set>

#include "../utils/UFDS.h"
#include "BaseTable.h"

using namespace std;

class DictionaryTable : public BaseTable {
  unordered_map<int, unordered_set<int>> relations = {};

 public:
  DictionaryTable();
  ~DictionaryTable() = default;
  void insert(int i, int j);
  bool isRelated(int i, int j);
};