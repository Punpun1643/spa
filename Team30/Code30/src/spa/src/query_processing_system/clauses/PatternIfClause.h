#pragma once

#include "PatternContainerClause.h"

class PatternIfClause : public PatternContainerClause {
 public:
  PatternIfClause(PqlDeclaration if_decl, EntRef ent_ref);
};
