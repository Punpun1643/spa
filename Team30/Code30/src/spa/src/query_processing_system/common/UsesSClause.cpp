#include "UsesSClause.h"

#include <string>

UsesSClause::UsesSClause(std::unique_ptr<StmtRef> arg1,
                         std::unique_ptr<EntRef> arg2)
    : UsesOrModifiesClause(std::move(arg1), std::move(arg2),
                           RelationType::USES_S) {}
