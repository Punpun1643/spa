#include "SelectClause.h"

#include <utility>

#include "PqlDeclaration.h"

SelectClause::SelectClause(PqlDeclaration declaration)
    : declaration(std::move(declaration)) {}

PqlDeclaration SelectClause::getDeclaration() const { return declaration; }

std::unique_ptr<ClauseResult> SelectClause::evaluate(PKBQPSInterface& pkb) {
  EntityType entity_type = declaration.getEntityType();

  auto values = pkb.getEntitiesWithType(entity_type);

  return std::make_unique<ClauseResult>(declaration, *values);
}
