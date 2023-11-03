#include "Clause.h"
Clause::~Clause() = default;

void Clause::FlagAsNegated() {
  is_not_clause = true;
}

bool Clause::IsNegated() const {
  return is_not_clause;
}
