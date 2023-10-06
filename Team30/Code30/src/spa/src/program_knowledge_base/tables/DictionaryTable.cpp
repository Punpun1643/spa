#include "DictionaryTable.h"

DictionaryTable::DictionaryTable() : BaseTable() {}

void DictionaryTable::insert(std::string i, std::string j) {
  auto it = relations.find(i);
  if (it != relations.end()) {
    relations[i].insert(j);
  } else {
    relations[i] = {j};
  };

  it = inverse_relations.find(j);
  if (it != inverse_relations.end()) {
    inverse_relations[j].insert(i);
  } else {
    inverse_relations[j] = {i};
  };
};

bool DictionaryTable::isRelated(std::string i, std::string j) {
  auto it = relations.find(i);
  if (it == relations.end()) {
    return false;
  }
  return relations[i].find(j) != relations[i].end();
}

bool DictionaryTable::isEmpty() { return relations.size() == 0; }

std::unordered_set<std::string> DictionaryTable::getAllRelated(
    std::shared_ptr<std::unordered_set<std::string>> vals) {
  std::unordered_set<std::string> output;
  for (std::string val : *vals) {
    if (relations.find(val) != relations.end()) {
      output.insert(val);
    }
  }
  return output;
};

std::unordered_set<std::string> DictionaryTable::getAllInverseRelated(
    std::shared_ptr<std::unordered_set<std::string>> vals) {
  std::unordered_set<std::string> output;
  for (std::string val : *vals) {
    if (inverse_relations.find(val) != inverse_relations.end()) {
      output.insert(val);
    }
  }
  return output;
};