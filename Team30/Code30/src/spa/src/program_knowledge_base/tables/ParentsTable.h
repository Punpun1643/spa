#pragma once

#include <unordered_map>
#include "BaseTable.h"

using namespace std;

class ParentsTable : public BaseTable {
  unordered_map<int, int> parents = {};
  UFDS parentsS = UFDS(100);

 public:
  ParentsTable();
  ~ParentsTable() = default;
  void insert(int i, int j);
  bool isParent(int i, int j) { return parents[i] == j; };
  bool isParentS(int i, int j) { return i < j && parentsS.isSameSet(i, j); };
};