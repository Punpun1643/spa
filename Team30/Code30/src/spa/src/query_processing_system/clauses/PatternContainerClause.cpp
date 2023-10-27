#include "PatternContainerClause.h"

#include <utility>

#include "query_processing_system/exceptions/InvalidSemanticsException.h"

std::vector<std::string> PatternContainerClause::EvaluateWildRef(
    PKBQPSInterface& pkb) {
  return pkb.GetContainerStmtsWithControlVar(target_type);
}

std::vector<std::string> PatternContainerClause::EvaluateValueRef(
    PKBQPSInterface& pkb) {
  return pkb.GetContainerStmtsWithGivenControlVar(target_type,
                                                  ent_ref.GetValue());
}

std::vector<std::pair<std::string, std::string>>
PatternContainerClause::EvaluateDeclRef(PKBQPSInterface& pkb) {
  return pkb.GetContainerStmtControlVarPairs(target_type);
}

PatternContainerClause::PatternContainerClause(PqlDeclaration container_decl,
                                               EntRef ent_ref,
                                               EntityType target_type)
    : PatternClause(std::move(container_decl), std::move(ent_ref), target_type) {
  if (target_type != EntityType::WHILE && target_type != EntityType::IF) {
    throw InvalidSemanticsException(
        "Target type for PatternContainer must be either a while or if stmt");
  }
}

PatternContainerClause::~PatternContainerClause() = default;
