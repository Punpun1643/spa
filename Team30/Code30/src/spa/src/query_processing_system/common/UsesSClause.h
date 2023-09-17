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
 protected:
  std::unique_ptr<ClauseResult> evaluateWildDeclaration(PkbApi &pkb) override;
  std::unique_ptr<ClauseResult> evaluateWildValue(PkbApi &pkb) override;
  std::unique_ptr<ClauseResult> evaluateWildWild(PkbApi &pkb) override;

 public:
  UsesSClause(std::unique_ptr<StmtRef> arg1, std::unique_ptr<EntRef> arg2);
};
