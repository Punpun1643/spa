#include "DictionaryTable.h"

DictionaryTable::DictionaryTable() : BaseTable() {}

void DictionaryTable::insert(string i, string j) {
  auto it = relations.find(i);

  if (it != relations.end()) {
    relations[i].insert(j);
  } else {
    relations[i] = {j};
  };
};

bool DictionaryTable::isRelated(string i, string j) {
  auto it = relations.find(i);
  if (it == relations.end()) {
    return false;
  }
  return relations[i].find(j) != relations[i].end();
}

//unique_ptr<vector<string>> DictionaryTable::isRelatedAll(
//    unordered_set<string> ls) {
//  for (string element : ls) {
//
//  }
//}