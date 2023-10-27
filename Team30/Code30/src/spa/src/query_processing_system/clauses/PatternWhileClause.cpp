#include "PatternWhileClause.h"

#include <utility>

PatternWhileClause::PatternWhileClause(PqlDeclaration if_decl, EntRef ent_ref)
    : PatternContainerClause(std::move(if_decl), std::move(ent_ref), EntityType::WHILE) {}
