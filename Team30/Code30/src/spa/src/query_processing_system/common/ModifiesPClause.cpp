#include "ModifiesPClause.h"

#include <string>

ModifiesPClause::ModifiesPClause(std::unique_ptr<EntRef> arg1,
                                 std::unique_ptr<EntRef> arg2)
    : UsesOrModifiesClause(std::move(arg1), std::move(arg2),
                           RelationType::MODIFIES_P) {
  this->checkDeclarationArgEntityType(1, VALID_DECL_TYPES_FOR_ARG_1,
                                      INVALID_ARG_1_DECL_TYPE_MSG);
}

std::vector<EntityType> const ModifiesPClause::VALID_DECL_TYPES_FOR_ARG_1 = {
    EntityType::PROCEDURE};

std::string const ModifiesPClause::INVALID_ARG_1_DECL_TYPE_MSG =
    "If the 1st arg of ModifiesP is a declaration, it must have the procedure "
    "type.";
