#include "DictionaryTable.h"

#include <memory>
#include <string>

DictionaryTable::DictionaryTable() : RelationTable() {}

void DictionaryTable::Insert(std::string i, std::string j) {
  auto it = relations.find(i);
  if (it != relations.end()) {
    relations[i].insert(j);
  } else {
    relations[i] = {j};
  }

  it = inverse_relations.find(j);
  if (it != inverse_relations.end()) {
    inverse_relations[j].insert(i);
  } else {
    inverse_relations[j] = {i};
  }
}

bool DictionaryTable::IsEmpty() {
  return relations.size() == 0;
}

bool DictionaryTable::IsRelated(std::string i, std::string j) {
  auto it = relations.find(i);
  if (it == relations.end()) {
    return false;
  }
  return relations[i].find(j) != relations[i].end();
}

bool DictionaryTable::HasRelations(std::string val) {
  return relations.find(val) != relations.end() && relations[val].size() > 0;
}

bool DictionaryTable::HasInverseRelations(std::string val) {
  return inverse_relations.find(val) != inverse_relations.end() &&
         inverse_relations[val].size() > 0;
}

std::unordered_set<std::string> DictionaryTable::GetAllWithRelations(
    std::shared_ptr<std::unordered_set<std::string>> vals) {
  std::unordered_set<std::string> output;
  for (std::string val : *vals) {
    if (HasRelations(val)) {
      output.insert(val);
    }
  }
  return output;
}

std::unordered_set<std::string> DictionaryTable::GetAllWithInverseRelations(
    std::shared_ptr<std::unordered_set<std::string>> vals) {
  std::unordered_set<std::string> output;
  for (std::string val : *vals) {
    if (HasInverseRelations(val)) {
      output.insert(val);
    }
  }
  return output;
}

std::unordered_set<std::string> DictionaryTable::GetAllRelatedToValue(
    std::string val) {
  return relations[val];
}

std::unordered_set<std::string> DictionaryTable::GetAllInverseRelatedToValue(
    std::string val) {
  return inverse_relations[val];
}
