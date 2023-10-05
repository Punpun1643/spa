#include "UsesSClause.h"

#include <string>

UsesSClause::UsesSClause(std::unique_ptr<StmtRef> arg1,
                         std::unique_ptr<EntRef> arg2)
    : UsesOrModifiesClause(std::move(arg1), std::move(arg2),
                           RelationType::USES_S,
                           VALID_DECL_TYPES_FOR_ARG_1, INVALID_ARG_1_DECL_TYPE_MSG) {}

const std::vector<EntityType> UsesSClause::VALID_DECL_TYPES_FOR_ARG_1 =
  {EntityType::STMT, EntityType::ASSIGN, EntityType::PRINT, EntityType::READ,
   EntityType::IF,   EntityType::WHILE,  EntityType::CALL};

const std::string UsesSClause::INVALID_ARG_1_DECL_TYPE_MSG =
    "The 1st declaration arg of Uses_S does not have a stmt entity type.";
