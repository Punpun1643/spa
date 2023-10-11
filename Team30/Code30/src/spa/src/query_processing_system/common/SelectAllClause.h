#pragma once
#include <memory>

#include "Clause.h"
#include "PqlDeclaration.h"

class SelectAllClause : public Clause {
 private:
  PqlDeclaration const declaration;

 public:
  explicit SelectAllClause(PqlDeclaration declaration);
  std::unique_ptr<ClauseResult> evaluate(PKBQPSInterface& pkb) override;
};
