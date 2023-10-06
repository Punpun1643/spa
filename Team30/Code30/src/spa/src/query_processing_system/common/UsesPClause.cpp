#include "UsesPClause.h"

#include <string>

UsesPClause::UsesPClause(std::unique_ptr<EntRef> arg1,
                         std::unique_ptr<EntRef> arg2)
    : UsesOrModifiesClause(std::move(arg1), std::move(arg2),
                           RelationType::USES_P, VALID_DECL_TYPES_FOR_ARG_1,
                           INVALID_ARG_1_DECL_TYPE_MSG) {}

std::vector<EntityType> const UsesPClause::VALID_DECL_TYPES_FOR_ARG_1 = {
    EntityType::PROCEDURE};

std::string const UsesPClause::INVALID_ARG_1_DECL_TYPE_MSG =
    "If the 1st arg of UsesP is a declaration, it must have the procedure "
    "type.";
