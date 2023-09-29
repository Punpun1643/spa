#pragma once

#include <memory>
#include <string>
#include <vector>
#include <unordered_set>

class BaseTable {
 public:
  BaseTable();
  ~BaseTable() = default;

  virtual void insert(std::string i, std::string j) = 0;
  virtual bool isRelated(std::string i, std::string j) = 0;
  virtual bool isEmpty() = 0;
};