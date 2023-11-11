#include "DictionaryTable.h"

#include <memory>
#include <string>

DictionaryTable::DictionaryTable() : RelationTable() {}

void DictionaryTable::Insert(std::string const& i, std::string const& j) {
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

bool DictionaryTable::IsRelated(std::string const& i, std::string const& j) {
  if (relations.count(i) == 0) {
    return false;
  }
  return relations.at(i).find(j) != relations[i].end();
}

bool DictionaryTable::HasRelations(std::string const& val) {
  return relations.find(val) != relations.end();
}

bool DictionaryTable::HasInverseRelations(std::string const& val) {
  return inverse_relations.find(val) != inverse_relations.end();
}

std::unordered_set<std::string> DictionaryTable::GetAllWithRelations(
    std::unordered_set<std::string> const& vals) {
  std::unordered_set<std::string> output;
  for (std::string val : vals) {
    if (HasRelations(val)) {
      output.insert(val);
    }
  }
  return output;
}

std::unordered_set<std::string> DictionaryTable::GetAllWithInverseRelations(
    std::unordered_set<std::string> const& vals) {
  std::unordered_set<std::string> output;
  for (std::string val : vals) {
    if (HasInverseRelations(val)) {
      output.insert(val);
    }
  }
  return output;
}

std::unordered_set<std::string> DictionaryTable::GetAllRelatedToValue(
    std::string const& val) {
  if (relations.count(val) == 0) {
    return std::unordered_set<std::string>();
  }
  return relations.at(val);
}

std::unordered_set<std::string> DictionaryTable::GetAllInverseRelatedToValue(
    std::string const& val) {
  if (inverse_relations.count(val) == 0) {
    return std::unordered_set<std::string>();
  }
  return inverse_relations.at(val);
}
