#include "PqlDeclaration.h"

#include <utility>

// Needs to be hashable so that it can be put into a dict.
PqlDeclaration::PqlDeclaration(std::string name, EntityType entity_type)
    : name(std::move(name)), entity_type(entity_type) {}

EntityType PqlDeclaration::GetEntityType() const {
  return entity_type;
}

bool PqlDeclaration::operator==(PqlDeclaration const& other) const {
  return (name == other.name && entity_type == other.entity_type);
}
