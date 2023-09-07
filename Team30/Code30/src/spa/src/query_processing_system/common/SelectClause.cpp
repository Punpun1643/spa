#include "SelectClause.h"

#include <utility>
#include "PqlDeclaration.h"

SelectClause::SelectClause(std::shared_ptr<PqlDeclaration> declaration):
 declaration(std::move(declaration)) {};
