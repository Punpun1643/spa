#include "NextClause.h"

#include <string>

NextClause::NextClause(std::unique_ptr<StmtRef> arg1,
                       std::unique_ptr<StmtRef> arg2, bool is_starred)
    : SuchThatClause(
          std::move(arg1), std::move(arg2),
          (is_starred ? RelationType::NEXT_STAR : RelationType::NEXT)) {}
