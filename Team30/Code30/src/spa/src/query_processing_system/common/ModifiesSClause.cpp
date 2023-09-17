#include "ModifiesSClause.h"

#include <string>

#include "InvalidSemanticsException.h"

ModifiesSClause::ModifiesSClause(std::unique_ptr<StmtRef> arg1,
                                 std::unique_ptr<EntRef> arg2)
    : SuchThatClause(std::move(arg1), std::move(arg2),
                     RelationType::MODIFIES_S) {
  if (this->arg1->getRefType() == PqlRefType::DECLARATION &&
      std::find(VALID_TYPES_FOR_ARG_1.begin(), VALID_TYPES_FOR_ARG_1.end(),
                this->arg1->getDeclarationType()) ==
          VALID_TYPES_FOR_ARG_1.end()) {
    throw InvalidSemanticsException(
        "The 1st declaration arg of Modifies does not have the correct entity "
        "type.");
  }

  if (this->arg2->getRefType() == PqlRefType::DECLARATION &&
      this->arg2->getDeclarationType() != VALID_TYPE_FOR_ARG_2) {
    throw InvalidSemanticsException(
        "If the 2nd arg of Modifies is a declaration, it must have the var "
        "type.");
  }

  if (this->arg1->getRefType() == PqlRefType::WILD) {
    throw InvalidSemanticsException(
        "1st argument of Modifies cannot be a wildcard");
  }
}

std::unique_ptr<ClauseResult> ModifiesSClause::evaluateWildDeclaration(
    PkbApi& pkb) {
  throw InvalidSemanticsException("1st argument of Uses cannot be a wildcard");
}

std::unique_ptr<ClauseResult> ModifiesSClause::evaluateWildWild(PkbApi& pkb) {
  throw InvalidSemanticsException("1st argument of Uses cannot be a wildcard");
}

std::unique_ptr<ClauseResult> ModifiesSClause::evaluateWildValue(PkbApi& pkb) {
  throw InvalidSemanticsException("1st argument of Uses cannot be a wildcard");
}
