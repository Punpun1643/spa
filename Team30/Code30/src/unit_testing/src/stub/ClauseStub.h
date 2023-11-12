#pragma once
#include "query_processing_system/clauses/Clause.h"

class ClauseStub : public Clause {
 private:
  std::unique_ptr<ClauseResult> result;

 public:
  explicit ClauseStub(std::unique_ptr<ClauseResult> result);
  std::unique_ptr<ClauseResult> Evaluate(PKBQPSInterface& pkb) override;
  std::optional<PqlDeclaration> GetFirstDeclaration() const override;
  std::optional<PqlDeclaration> GetSecondDeclaration() const override;
};
