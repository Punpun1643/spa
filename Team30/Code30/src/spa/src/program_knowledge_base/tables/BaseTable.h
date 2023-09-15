#pragma once

#include <memory>
#include <string>
#include <vector>
#include <unordered_set>

using namespace std;

class BaseTable {
 public:
  BaseTable();
  ~BaseTable() = default;

  virtual void insert(string i, string j) = 0;
  virtual bool isRelated(string i, string j) = 0;
  //virtual unique_ptr<vector<string>> isRelatedAll(unordered_set<string> ls) = 0;
};