#pragma once
#include <memory>

#include "Clause.h"
#include "PqlDeclaration.h"

class SelectClause : public Clause {
 private:
  std::shared_ptr<PqlDeclaration> declaration;
 public:
  explicit SelectClause(std::shared_ptr<PqlDeclaration> declaration);
};