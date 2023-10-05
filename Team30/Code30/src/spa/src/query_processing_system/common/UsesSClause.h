#pragma once
#include <memory>
#include <string>

#include "EntRef.h"
#include "EntityType.h"
#include "StmtRef.h"
#include "UsesOrModifiesClause.h"

class UsesSClause : public UsesOrModifiesClause {
 private:
  static const std::vector<EntityType> VALID_DECL_TYPES_FOR_ARG_1;
  static const std::string INVALID_ARG_1_DECL_TYPE_MSG;

 public:
  UsesSClause(std::unique_ptr<StmtRef> arg1, std::unique_ptr<EntRef> arg2);
};
