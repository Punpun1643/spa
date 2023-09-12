#include "FollowsTable.h"

FollowsTable::FollowsTable() : BaseTable() {}

void FollowsTable::insert(int i, int j) {
  follows[i] = j;
  followsS.unionSets(i, j);
}