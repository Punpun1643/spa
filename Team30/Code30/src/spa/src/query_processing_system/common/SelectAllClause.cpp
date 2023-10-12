#include "SelectAllClause.h"

#include <utility>

#include "PqlDeclaration.h"

SelectAllClause::SelectAllClause(PqlDeclaration declaration)
    : declaration(std::move(declaration)) {}

std::unique_ptr<ClauseResult> SelectAllClause::evaluate(PKBQPSInterface& pkb) {
  EntityType entity_type = declaration.getEntityType();

  auto values = pkb.getEntitiesWithType(entity_type);

  return std::make_unique<ClauseResult>(declaration, *values);
}
