#include "ModifiesSClause.h"

#include <string>

ModifiesSClause::ModifiesSClause(std::unique_ptr<StmtRef> arg1,
                                 std::unique_ptr<EntRef> arg2)
    : UsesOrModifiesClause(std::move(arg1), std::move(arg2),
                           RelationType::MODIFIES_S) {}
