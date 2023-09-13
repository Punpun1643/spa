#include "RelDatabase.h"

#include "program_knowledge_base/tables/DictionaryTable.h"
#include "program_knowledge_base/tables/UFDSTable.h"

RelDatabase::RelDatabase() {
  relationships[RelationType::PARENT] =
      make_shared<DictionaryTable>(DictionaryTable());
  relationships[RelationType::FOLLOWS] =
      make_shared<DictionaryTable>(DictionaryTable());
  relationships[RelationType::FOLLOWS_STAR] =
      make_shared<UFDSTable>(UFDSTable());
  relationships[RelationType::PARENT_STAR] =
      make_shared<UFDSTable>(UFDSTable());
}

shared_ptr<BaseTable> RelDatabase::getTable(RelationType type) {
  return relationships[type];
}