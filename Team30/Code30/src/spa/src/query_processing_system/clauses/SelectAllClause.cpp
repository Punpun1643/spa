#include "SelectAllClause.h"

#include <utility>

#include "query_processing_system/references/PqlDeclaration.h"

SelectAllClause::SelectAllClause(PqlDeclaration declaration)
    : declaration(std::move(declaration)) {}

std::unique_ptr<ClauseResult> SelectAllClause::Evaluate(PKBQPSInterface& pkb) {
  EntityType entity_type = declaration.GetEntityType();

  auto values = pkb.GetEntitiesWithType(entity_type);

  return std::make_unique<ClauseResult>(declaration, values);
}
