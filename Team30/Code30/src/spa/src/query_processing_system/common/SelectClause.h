#pragma once
#include "PqlDeclaration.h"
#include <memory>

class SelectClause {
 private:
  std::shared_ptr<PqlDeclaration> declaration;
 public:
  explicit SelectClause(std::shared_ptr<PqlDeclaration> declaration);
};
