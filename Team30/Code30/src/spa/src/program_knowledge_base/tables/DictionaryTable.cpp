#include "DictionaryTable.h"

DictionaryTable::DictionaryTable() : BaseTable() {}

void DictionaryTable::insert(std::string i, std::string j) {
  auto it = relations.find(i);

  if (it != relations.end()) {
    relations[i].insert(j);
  } else {
    relations[i] = {j};
  };
};

bool DictionaryTable::isRelated(std::string i, std::string j) {
  auto it = relations.find(i);
  if (it == relations.end()) {
    return false;
  }
  return relations[i].find(j) != relations[i].end();
}