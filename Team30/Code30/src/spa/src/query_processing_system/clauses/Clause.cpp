#include "Clause.h"
Clause::~Clause() = default;

void Clause::FlagAsNegated() {
  is_negated = true;
}

bool Clause::IsNegated() const {
  return is_negated;
}
