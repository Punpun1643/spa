#pragma once
#include <memory>
#include <string>

#include "EntRef.h"
#include "EntityType.h"
#include "StmtRef.h"
#include "SuchThatClause.h"

class UsesSClause : public SuchThatClause {
 private:
  std::vector<EntityType> VALID_ENTITY_TYPES_FOR_ARG_1 = {EntityType::STMT, EntityType::ASSIGN, EntityType::PRINT, EntityType::READ,
                                                          EntityType::IF,   EntityType::WHILE,  EntityType::CALL};
  std::string INVALID_ARG_1_ENTITY_TYPE_MSG =
      "The 1st declaration arg of Uses does not have the correct entity.";
  std::string INVALID_ARG_2_ENTITY_TYPE_MSG =
      "If the 2nd arg of Uses is a declaration, it must have the var type.";
  std::string INVALID_ARG_1_REF_TYPE_MSG =
      "1st argument of Uses cannot be a wildcard";
  std::vector<EntityType> VALID_ENTITY_TYPES_FOR_ARG_2 = {EntityType::VARIABLE};
  std::vector<PqlRefType> VALID_REF_TYPES_FOR_ARG_1 = {PqlRefType::VALUE, PqlRefType::DECLARATION};

 protected:
  std::unique_ptr<ClauseResult> evaluateWildDeclaration(PkbApi& pkb) override;
  std::unique_ptr<ClauseResult> evaluateWildValue(PkbApi& pkb) override;
  std::unique_ptr<ClauseResult> evaluateWildWild(PkbApi& pkb) override;

 public:
  UsesSClause(std::unique_ptr<StmtRef> arg1, std::unique_ptr<EntRef> arg2);
};
