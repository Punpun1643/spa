#include "AffectsClause.h"

#include <string>

AffectsClause::AffectsClause(std::unique_ptr<StmtRef> arg1,
                             std::unique_ptr<StmtRef> arg2)
    : SuchThatClause(std::move(arg1), std::move(arg2), RelationType::AFFECTS) {}
