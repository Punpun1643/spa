#include "PatternClause.h"

#include <utility>

#include "query_processing_system/exceptions/InvalidSemanticsException.h"

PatternClause::~PatternClause() = default;

PatternClause::PatternClause(PqlDeclaration decl, EntRef ent_ref,
                             EntityType target_type)
    : decl(std::move(decl)),
      ent_ref(std::move(ent_ref)),
      target_type(target_type) {
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
                                            std::move(values));
    }
    case (PqlRefType::VALUE): {
      auto values = EvaluateValueRef(pkb);
      return std::make_unique<ClauseResult>(decl, std::move(values));
    }
    case (PqlRefType::WILD): {
      auto values = EvaluateWildRef(pkb);
      return std::make_unique<ClauseResult>(decl, std::move(values));
    }
    default:
      break;
  }
}

std::unique_ptr<ClauseResult> PatternClause::EvaluateOnCondition(
    PKBQPSInterface& pkb, std::unordered_set<std::string>& decl_1_subset,
    std::unordered_set<std::string>& decl_2_subset) {
  if (ent_ref.GetRefType() != PqlRefType::DECLARATION) {
    return Evaluate(pkb);
  }
  auto values = EvaluateDeclRef(pkb, decl_1_subset, decl_2_subset);
  return std::make_unique<ClauseResult>(decl, ent_ref.GetDeclaration(),
                                        std::move(values));
}

int PatternClause::GetNumDeclarations() const {
  if (ent_ref.GetRefType() == PqlRefType::DECLARATION) {
    return 2;
  } else {
    return 1;
  }
}

std::vector<PqlDeclaration> PatternClause::GetDeclarations() const {
  std::vector<PqlDeclaration> output = {decl};
  if (ent_ref.GetRefType() == PqlRefType::DECLARATION) {
    output.push_back(ent_ref.GetDeclaration());
  }
  return output;
}
