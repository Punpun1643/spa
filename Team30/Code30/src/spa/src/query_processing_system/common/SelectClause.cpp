#include "SelectClause.h"
#include "PqlDeclaration.h"

SelectClause::SelectClause(std::shared_ptr<PqlDeclaration> declaration) {
  this->declaration = declaration;
}

std::shared_ptr<PqlDeclaration> SelectClause::getDeclaration() {
  return declaration;
}

std::unique_ptr<ClauseResult> SelectClause::evaluate(PkbApi& pkb) {
  EntityType entity_type = declaration->getEntityType();

  std::vector<std::string> possible_values =
      *pkb.getEntitiesWithType(entity_type);

  std::unique_ptr<ClauseResult> result = std::make_unique<ClauseResult>();
  result->insertData(*(this->declaration), possible_values);

  return result;
}
