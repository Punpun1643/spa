#pragma once

#include <memory>
#include <string>
#include <vector>

#include "UsesOrModifiesClause.h"
#include "query_processing_system/references/EntRef.h"
#include "shared/types/EntityType.h"

class ModifiesPClause : public UsesOrModifiesClause {
 private:
  static std::vector<EntityType> const VALID_DECL_TYPES_FOR_ARG_1;
  static std::string const INVALID_ARG_1_DECL_TYPE_MSG;

 public:
  ModifiesPClause(std::unique_ptr<EntRef> arg1, std::unique_ptr<EntRef> arg2);
};
