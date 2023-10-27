#include "PatternIfClause.h"

#include <utility>

PatternIfClause::PatternIfClause(PqlDeclaration if_decl, EntRef ent_ref)
    : PatternContainerClause(std::move(if_decl), std::move(ent_ref), EntityType::IF) {}
