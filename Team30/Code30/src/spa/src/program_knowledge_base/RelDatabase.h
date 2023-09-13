#pragma once

#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <unordered_map>
#include "RelationType.h"

#include "tables/StmtStmtRelTable.h"

class RelDatabase
{
  std::unordered_map<RelationType, StmtStmtRelTable*> relationships;

 public:
  RelDatabase();
  std::vector<int> get(RelationType type, int lineNum);
  bool insert(RelationType type, int lineNum1, int lineNum2);
};