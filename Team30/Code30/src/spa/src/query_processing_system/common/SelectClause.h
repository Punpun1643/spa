#pragma once
#include <memory>

#include "Clause.h"
#include "PqlDeclaration.h"

class SelectClause : public Clause {
 private:
  PqlDeclaration const declaration;

 public:
  explicit SelectClause(PqlDeclaration declaration);
  PqlDeclaration getDeclaration() const;
  std::unique_ptr<ClauseResult> evaluate(PKBQPSInterface& pkb) override;
};
