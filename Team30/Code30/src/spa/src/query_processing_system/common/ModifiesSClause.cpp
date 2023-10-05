#include "ModifiesSClause.h"

#include <string>

ModifiesSClause::ModifiesSClause(std::unique_ptr<StmtRef> arg1,
                         std::unique_ptr<EntRef> arg2)
    : UsesOrModifiesClause(std::move(arg1), std::move(arg2),
                           RelationType::MODIFIES_S,
                           VALID_DECL_TYPES_FOR_ARG_1, INVALID_ARG_1_DECL_TYPE_MSG) {}

const std::vector<EntityType> ModifiesSClause::VALID_DECL_TYPES_FOR_ARG_1 =
    {EntityType::STMT, EntityType::ASSIGN, EntityType::PRINT, EntityType::READ,
     EntityType::IF,   EntityType::WHILE,  EntityType::CALL};

const std::string ModifiesSClause::INVALID_ARG_1_DECL_TYPE_MSG =
    "The 1st declaration arg of Modifies_S does not have a stmt entity type.";
