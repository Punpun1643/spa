#pragma once

#include <unordered_map>

#include "BaseTable.h"

using namespace std;

class FollowsTable : public BaseTable {
  unordered_map<int, int> follows = {};
  UFDS followsS = UFDS(100);

 public:
  FollowsTable();
  ~FollowsTable() = default;
  void insert(int i, int j);
  bool isFollows(int i, int j) { return follows[i] == j; };
  bool isFollowsS(int i, int j) { return i < j && followsS.isSameSet(i, j); };
};