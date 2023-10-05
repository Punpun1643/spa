#pragma once
#include <memory>
#include <string>

#include "EntRef.h"
#include "EntityType.h"
#include "StmtRef.h"
#include "UsesOrModifiesClause.h"

class ModifiesSClause : public UsesOrModifiesClause {
 private:
  static std::vector<EntityType> const VALID_DECL_TYPES_FOR_ARG_1;
  static std::string const INVALID_ARG_1_DECL_TYPE_MSG;

 public:
  ModifiesSClause(std::unique_ptr<StmtRef> arg1, std::unique_ptr<EntRef> arg2);
};
