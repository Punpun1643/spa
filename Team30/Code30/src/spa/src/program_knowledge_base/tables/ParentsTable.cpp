#pragma once
#include <iostream>
#include <string>
#include <vector>

#include "ParentsTable.h"

ParentsTable::ParentsTable() : BaseTable() {}

void ParentsTable::insert(int i, int j) {
  parents[i] = j;
  parentsS.unionSets(i, j);
}