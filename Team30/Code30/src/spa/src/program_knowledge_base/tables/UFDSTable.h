#pragma once

#include <unordered_map>

#include "../utils/UFDS.h"
#include "BaseTable.h"

using namespace std;

class UFDSTable : public BaseTable {
  UFDS relations = UFDS(100);

 public:
  UFDSTable();
  ~UFDSTable() = default;
  void insert(int i, int j) { relations.unionSets(i, j); };
  bool isRelated(int i, int j) { return i < j && relations.isSameSet(i, j); };
};