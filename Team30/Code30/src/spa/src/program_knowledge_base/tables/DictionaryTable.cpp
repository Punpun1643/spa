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

bool DictionaryTable::isEmpty() { return relations.size() == 0; }

bool DictionaryTable::isRelated(std::string i, std::string j) {
  auto it = relations.find(i);
  if (it == relations.end()) {
    return false;
  }
  return relations[i].find(j) != relations[i].end();
}

bool DictionaryTable::hasRelations(std::string val) {
  return relations.find(val) != relations.end();
};

bool DictionaryTable::hasInverseRelations(std::string val) {
  return inverse_relations.find(val) != inverse_relations.end();
}

std::unordered_set<std::string> DictionaryTable::getAllWithRelations(
    std::shared_ptr<std::unordered_set<std::string>> vals) {
  std::unordered_set<std::string> output;
  for (std::string val : *vals) {
    if (hasRelations(val)) {
      output.insert(val);
    }
  }
  return output;
};

std::unordered_set<std::string> DictionaryTable::getAllWithInverseRelations(
    std::shared_ptr<std::unordered_set<std::string>> vals) {
  std::unordered_set<std::string> output;
  for (std::string val : *vals) {
    if (hasInverseRelations(val)) {
      output.insert(val);
    }
  }
  return output;
};

std::unordered_set<std::string> DictionaryTable::getAllRelatedToValue(
    std::string val){
  return relations[val];
};

std::unordered_set<std::string> DictionaryTable::getAllInverseRelatedToValue(
    std::string val) {
  return inverse_relations[val];
};