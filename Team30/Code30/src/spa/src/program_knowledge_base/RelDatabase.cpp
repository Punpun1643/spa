#include "RelDatabase.h"

RelDatabase::RelDatabase() {
  relationships[RelationType::PARENT] =
      std::make_shared<DictionaryTable>(DictionaryTable());
  relationships[RelationType::FOLLOWS] =
      std::make_shared<DictionaryTable>(DictionaryTable());
  relationships[RelationType::FOLLOWS_STAR] =
      std::make_shared<LinkedListTable>(LinkedListTable());
  relationships[RelationType::PARENT_STAR] =
      std::make_shared<LinkedListTable>(LinkedListTable());

  relationships[RelationType::USES_S] =
      std::make_shared<DictionaryTable>(DictionaryTable());
  relationships[RelationType::USES_P] =
      std::make_shared<DictionaryTable>(DictionaryTable());
  relationships[RelationType::MODIFIES_S] =
      std::make_shared<DictionaryTable>(DictionaryTable());
  relationships[RelationType::MODIFIES_P] =
      std::make_shared<DictionaryTable>(DictionaryTable());

  relationships[RelationType::CALLS] =
      std::make_shared<DictionaryTable>(DictionaryTable());
  relationships[RelationType::CALLS_STAR] =
      std::make_shared<LinkedListTable>(LinkedListTable());
}

void RelDatabase::insert(RelationType type, std::string val1,
                         std::string val2) {
  relationships[type]->insert(val1, val2);
};

bool RelDatabase::isEmpty(RelationType type) {
  return relationships[type]->isEmpty();
};

bool RelDatabase::isRelated(RelationType type, std::string val1,
                            std::string val2) {
  return relationships[type]->isRelated(val1, val2);
};

bool RelDatabase::hasRelations(RelationType type, std::string val) {
  return relationships[type]->hasRelations(val);
};

bool RelDatabase::hasInverseRelations(RelationType type, std::string val) {
  return relationships[type]->hasInverseRelations(val);
};

std::unordered_set<std::string> RelDatabase::getAllWithRelations(
    RelationType type, std::shared_ptr<std::unordered_set<std::string>> vals) {
  return relationships[type]->getAllWithRelations(vals);
};

std::unordered_set<std::string> RelDatabase::getAllWithInverseRelations(
    RelationType type, std::shared_ptr<std::unordered_set<std::string>> vals) {
  return relationships[type]->getAllWithInverseRelations(vals);
};

std::unordered_set<std::string> RelDatabase::getAllRelatedToValue(
    RelationType type, std::string val) {
  return relationships[type]->getAllRelatedToValue(val);
}

std::unordered_set<std::string> RelDatabase::getAllInverseRelatedToValue(
    RelationType type, std::string val) {
  return relationships[type]->getAllInverseRelatedToValue(val);
}