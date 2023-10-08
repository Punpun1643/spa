#include "UsesOrModifiesClause.h"

#include <string>
#include <utility>

#include "query_processing_system/exceptions/InvalidSemanticsException.h"

UsesOrModifiesClause::~UsesOrModifiesClause() = default;

UsesOrModifiesClause::UsesOrModifiesClause(
    std::unique_ptr<PqlReference const> arg1,
    std::unique_ptr<EntRef const> arg2, RelationType relation_type)
    : SuchThatClause(std::move(arg1), std::move(arg2), relation_type) {
  this->checkDeclarationArgEntityType(2, VALID_DECL_TYPES_FOR_ARG_2,
                                      INVALID_ARG_2_DECL_TYPE_MSG);
  this->checkArgReferenceType(1, VALID_REF_TYPES_FOR_ARG_1,
                              INVALID_ARG_1_REF_TYPE_MSG);
}

std::unique_ptr<ClauseResult> UsesOrModifiesClause::evaluateWildDeclaration(
    PKBQPSInterface& pkb) {
  throw InvalidSemanticsException(
      "1st argument of Uses/Modifies cannot be a wildcard");
}

std::unique_ptr<ClauseResult> UsesOrModifiesClause::evaluateWildWild(
    PKBQPSInterface& pkb) {
  throw InvalidSemanticsException(
      "1st argument of Uses/Modifies cannot be a wildcard");
}

std::unique_ptr<ClauseResult> UsesOrModifiesClause::evaluateWildValue(
    PKBQPSInterface& pkb) {
  throw InvalidSemanticsException(
      "1st argument of Uses/Modifies cannot be a wildcard");
}