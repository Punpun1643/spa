#include "RelDatabase.h"

#include "program_knowledge_base/tables/DictionaryTable.h"
#include "program_knowledge_base/tables/UFDSTable.h"

RelDatabase::RelDatabase() {
  relationships[RelationType::PARENT] =
      make_shared<BaseTable>(DictionaryTable());
  relationships[RelationType::FOLLOWS] =
      make_shared<BaseTable>(DictionaryTable());
  relationships[RelationType::FOLLOWS_STAR] =
      make_shared<BaseTable>(UFDSTable());
  relationships[RelationType::PARENT_STAR] =
      make_shared<BaseTable>(UFDSTable());
  ;
}

shared_ptr<BaseTable> RelDatabase::getTable(RelationType type) {
  return relationships[type];
}