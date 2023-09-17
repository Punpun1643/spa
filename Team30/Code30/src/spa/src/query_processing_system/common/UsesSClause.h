#pragma once
#include <memory>

#include "EntRef.h"
#include "StmtRef.h"
#include "SuchThatClause.h"
#include "EntityType.h"

class UsesSClause : public SuchThatClause {
 private:
  std::vector<EntityType> VALID_TYPES_FOR_ARG_1 = {ASSIGN, PRINT, IF, WHILE, CALL};
  EntityType VALID_TYPE_FOR_ARG_2 = VARIABLE;
 public:
  UsesSClause(std::unique_ptr<StmtRef> arg1, std::unique_ptr<EntRef> arg2);
};
