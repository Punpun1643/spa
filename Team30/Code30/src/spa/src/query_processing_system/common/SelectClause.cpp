#include "SelectClause.h"

#include <utility>
#include <iostream>

#include "PqlDeclaration.h"

SelectClause::SelectClause(std::shared_ptr<PqlDeclaration> declaration)
    : declaration(std::move(declaration)) {}

std::shared_ptr<PqlDeclaration> SelectClause::getDeclaration() {
  return declaration;
}

std::unique_ptr<ClauseResult> SelectClause::evaluate(PkbApi& pkb) {
  EntityType entity_type = declaration->getEntityType();

  auto values = pkb.getEntitiesWithType(entity_type);

  return std::make_unique<ClauseResult>(*declaration, std::move(values));
}
