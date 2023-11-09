#include "PatternWhileClause.h"

#include <utility>

PatternWhileClause::PatternWhileClause(PqlDeclaration while_decl,
                                       EntRef ent_ref)
    : PatternContainerClause(std::move(while_decl), std::move(ent_ref),
                             EntityType::WHILE) {}
