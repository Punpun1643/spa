#pragma once

#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

class PatternTable {
 public:
  PatternTable();
  ~PatternTable() = default;

  virtual void Insert(std::string i, std::string j) = 0;
  virtual bool IsEmpty() = 0;
};
