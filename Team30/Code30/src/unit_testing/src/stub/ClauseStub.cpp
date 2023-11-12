#include "ClauseStub.h"

ClauseStub::ClauseStub(std::unique_ptr<ClauseResult> result)
    : result(std::move(result)) {}

std::unique_ptr<ClauseResult> ClauseStub::Evaluate(PKBQPSInterface& pkb) {
  return std::move(result);
}

std::optional<PqlDeclaration> ClauseStub::GetFirstDeclaration() const {
  return std::nullopt;
}

std::optional<PqlDeclaration> ClauseStub::GetSecondDeclaration() const {
  return std::nullopt;
}
