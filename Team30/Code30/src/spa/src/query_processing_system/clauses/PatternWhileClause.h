#pragma once

#include "PatternContainerClause.h"

class PatternWhileClause : public PatternContainerClause {
 public:
  PatternWhileClause(PqlDeclaration while_decl, EntRef ent_ref);
};
