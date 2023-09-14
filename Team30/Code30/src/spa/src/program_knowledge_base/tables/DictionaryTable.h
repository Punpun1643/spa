#pragma once

#include <unordered_map>
#include <unordered_set>

#include "../utils/UFDS.h"
#include "BaseTable.h"

using namespace std;

class DictionaryTable : public BaseTable {
  unordered_map<string, unordered_set<string>> relations = {};

 public:
  DictionaryTable();
  ~DictionaryTable() = default;
  void insert(string i, string j);
  bool isRelated(string i, string j);
  //unique_ptr<vector<string>> isRelatedAll(unordered_set<string> ls);
};