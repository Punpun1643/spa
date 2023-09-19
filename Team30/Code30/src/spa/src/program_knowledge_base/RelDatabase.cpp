#include "RelDatabase.h"

#include "program_knowledge_base/tables/DictionaryTable.h"
#include "program_knowledge_base/tables/UFDSTable.h"

RelDatabase::RelDatabase() {
  relationships[RelationType::PARENT] =
      std::make_shared<DictionaryTable>(DictionaryTable());
  relationships[RelationType::FOLLOWS] =
      std::make_shared<DictionaryTable>(DictionaryTable());
  relationships[RelationType::FOLLOWS_STAR] =
      std::make_shared<UFDSTable>(UFDSTable());
  relationships[RelationType::PARENT_STAR] =
      std::make_shared<UFDSTable>(UFDSTable());
  relationships[RelationType::USES_S] =
      std::make_shared<DictionaryTable>(DictionaryTable());
  relationships[RelationType::MODIFIES_S] =
      std::make_shared<DictionaryTable>(DictionaryTable());
}

std::shared_ptr<BaseTable> RelDatabase::getTable(RelationType type) {
  return relationships[type];
}
