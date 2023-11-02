#include "Clause.h"
Clause::~Clause() = default;

void Clause::FlagAsNotClause() {
  is_not_clause = true;
}

bool Clause::IsNotClause() const {
  return is_not_clause;
}
