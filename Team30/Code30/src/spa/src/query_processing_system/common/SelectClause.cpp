#include "SelectClause.h"

#include <utility>

#include "PqlDeclaration.h"

SelectClause::SelectClause(std::shared_ptr<PqlDeclaration> declaration)
    : declaration(declaration) {}

std::shared_ptr<PqlDeclaration> SelectClause::getDeclaration() {
  return declaration;
}

std::unique_ptr<ClauseResult> SelectClause::evaluate(PkbApi& pkb) {
  EntityType entity_type = declaration->getEntityType();

  std::vector<std::string> values = *pkb.getEntitiesWithType(entity_type);

  std::unique_ptr<ClauseResult> result =
      std::make_unique<ClauseResult>(*declaration, values);

  return result;
}
