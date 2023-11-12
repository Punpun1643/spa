#include "Clause.h"
Clause::~Clause() = default;

void Clause::FlagAsNegated() {
  is_negated = true;
}

bool Clause::IsNegated() const {
  return is_negated;
}

std::unique_ptr<ClauseResult> Clause::EvaluateOnCondition(PKBQPSInterface& pkb, std::unordered_set<std::string>& d1_values, std::unordered_set<std::string>& d2_values) {
  throw std::logic_error("Conditional Evaluation not supported");
}

bool Clause::SupportsConditionalEvaluation() const {
  return false;
}
