#include "ParentStarClause.h"

#include <string>

ParentStarClause::ParentStarClause(std::unique_ptr<StmtRef> arg1,
                                   std::unique_ptr<StmtRef> arg2)
    : SuchThatClause(std::move(arg1), std::move(arg2),
                     RelationType::PARENT_STAR) {}
