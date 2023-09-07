#pragma once

#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <unordered_map>

class EntityTable {
  static std::unordered_map<std::string, std::vector<int>> dataTable;
  bool insertData(std::string key, int value);
  std::vector<int> queryData(std::string key);
};
