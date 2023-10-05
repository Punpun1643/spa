#include "EntRef.h"

#include <utility>

#include "query_processing_system/exceptions/InvalidSemanticsException.h"

EntRef::EntRef() : PqlReference() {}

EntRef::EntRef(std::string entity_value)
    : PqlReference(std::move(entity_value)) {
  // Assumes that entity_value is a valid identifier
}

EntRef::EntRef(PqlDeclaration const& declaration) : PqlReference(declaration) {
  EntityType declaration_type = declaration.getEntityType();
  if (std::find(VALID_ENT_TYPES.begin(), VALID_ENT_TYPES.end(),
                declaration_type) == VALID_ENT_TYPES.end()) {
    throw InvalidSemanticsException("Declaration is not an entity type");
  }
}
