#include "CallsClause.h"

#include <memory>
#include <string>
#include <utility>
#include <vector>

std::vector<EntityType> const CallsClause::ALLOWED_DECL_TYPES = {
    EntityType::PROCEDURE};
std::string const CallsClause::INVALID_DECL_TYPE_ERROR_MSG =
    "Declarations in Calls/* clauses must be of type procedure";

CallsClause::CallsClause(std::unique_ptr<EntRef> arg1,
                         std::unique_ptr<EntRef> arg2, bool is_starred)
    : SuchThatClause(
          std::move(arg1), std::move(arg2),
          (is_starred ? RelationType::CALLS_STAR : RelationType::CALLS)) {
  this->CheckDeclarationArgEntityType(1, ALLOWED_DECL_TYPES,
                                      INVALID_DECL_TYPE_ERROR_MSG);
  this->CheckDeclarationArgEntityType(2, ALLOWED_DECL_TYPES,
                                      INVALID_DECL_TYPE_ERROR_MSG);
}
