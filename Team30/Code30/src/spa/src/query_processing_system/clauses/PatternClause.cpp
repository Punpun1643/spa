#include "PatternClause.h"

#include <utility>

#include "query_processing_system/exceptions/InvalidSemanticsException.h"

PatternClause::~PatternClause() = default;

PatternClause::PatternClause(PqlDeclaration decl, EntRef ent_ref,
                             EntityType target_type)
    : decl(std::move(decl)), ent_ref(std::move(ent_ref)), target_type(target_type) {
  if (this->decl.GetEntityType() != target_type) {
    throw InvalidSemanticsException(
        "Given PatternClause decl differs from the target type.");
  }

  if (this->ent_ref.GetRefType() == PqlRefType::DECLARATION &&
      this->ent_ref.GetDeclarationType() != EntityType::VARIABLE) {
    throw InvalidSemanticsException(
        "If the entRef of a Pattern clause is a declaration, it must have the "
        "variable type");
  }
}

std::unique_ptr<ClauseResult> PatternClause::Evaluate(PKBQPSInterface& pkb) {
  switch (ent_ref.GetRefType()) {
    case (PqlRefType::DECLARATION): {
      auto values = EvaluateDeclRef(pkb);
      return std::make_unique<ClauseResult>(decl, ent_ref.GetDeclaration(),
                                            values);
    }
    case (PqlRefType::VALUE): {
      auto values = EvaluateValueRef(pkb);
      return std::make_unique<ClauseResult>(decl, values);
    }
    case (PqlRefType::WILD): {
      auto values = EvaluateWildRef(pkb);
      return std::make_unique<ClauseResult>(decl, values);
    }
    default:
      break;
  }
}
