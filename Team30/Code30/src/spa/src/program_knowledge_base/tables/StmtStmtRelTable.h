#pragma once

#include <iostream>
#include <stdio.h>
#include <string>
#include <unordered_map>
#include <vector>

class StmtStmtRelTable {
  static std::unordered_map<int, std::vector<int>> dataTable;

  public:
  StmtStmtRelTable();
  bool insertData(int key, int value);
  std::vector<int> queryData(int key);
};