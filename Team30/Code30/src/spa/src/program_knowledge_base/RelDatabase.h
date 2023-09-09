#pragma once

#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <unordered_map>

class RelDatabase
{
  std::vector<int> get(RelType type, int lineNum);
  bool insert(RelType type, int lineNum1, int lineNum2);
};