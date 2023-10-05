#include "UsesSClause.h"

#include <string>

#include "query_processing_system/exceptions/InvalidSemanticsException.h"

UsesSClause::UsesSClause(std::unique_ptr<StmtRef> arg1,
                         std::unique_ptr<EntRef> arg2)
    : SuchThatClause(std::move(arg1), std::move(arg2), RelationType::USES_S) {
  this->checkDeclarationArgEntityType(1, VALID_ENTITY_TYPES_FOR_ARG_1,
                                      INVALID_ARG_1_ENTITY_TYPE_MSG);
  this->checkDeclarationArgEntityType(2, VALID_ENTITY_TYPES_FOR_ARG_2,
                                      INVALID_ARG_2_ENTITY_TYPE_MSG);
  this->checkArgReferenceType(1, VALID_REF_TYPES_FOR_ARG_1,
                              INVALID_ARG_1_REF_TYPE_MSG);
}

std::unique_ptr<ClauseResult> UsesSClause::evaluateWildDeclaration(
    PKBQPSInterface& pkb) {
  throw InvalidSemanticsException("1st argument of Uses cannot be a wildcard");
}

std::unique_ptr<ClauseResult> UsesSClause::evaluateWildWild(
    PKBQPSInterface& pkb) {
  throw InvalidSemanticsException("1st argument of Uses cannot be a wildcard");
}

std::unique_ptr<ClauseResult> UsesSClause::evaluateWildValue(
    PKBQPSInterface& pkb) {
  throw InvalidSemanticsException("1st argument of Uses cannot be a wildcard");
}
