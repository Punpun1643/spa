#include "PqlDeclaration.h"

// Needs to be hashable so that it can be put into a dict.
PqlDeclaration::PqlDeclaration(std::shared_ptr<const std::string> name, EntityType entity_type)
    :entity_type(entity_type) {
  this->name = std::move(name);
}