#pragma once

#include <stdio.h>

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

class EntityTable {
  static std::unordered_map<std::string, std::vector<int>> dataTable;

 public:
  EntityTable();
  bool insertData(std::string key, int value);
  std::vector<int> queryData(std::string key);
  std::vector<std::string> queryAllKeys();
};
