#pragma once
#include <unordered_map>

#include "../utils/UFDS.h"
#include "BaseTable.h"

using namespace std;

class ParentsTable : public BaseTable {
 protected:
  unordered_map<int, int> parents = {};
  UFDS parentsS = UFDS(100);

 public:
  explicit ParentsTable();
  ~ParentsTable() = default;
  void insert(int i, int j);
  bool isParent(int i, int j) { return ParentsTable::parents[i] == j; };
  bool isParentS(int i, int j) { return i < j && parentsS.isSameSet(i, j); };
};