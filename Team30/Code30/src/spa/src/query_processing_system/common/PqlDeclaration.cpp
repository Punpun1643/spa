#include "PqlDeclaration.h"

// Needs to be hashable so that it can be put into a dict.
PqlDeclaration::PqlDeclaration(std::shared_ptr<std::string> name,
                               EntityType entity_type)
    : name(name), entity_type(entity_type) {}

EntityType PqlDeclaration::getEntityType() const { return entity_type; }

std::shared_ptr<std::string const> PqlDeclaration::getName() const {
  return name;
}

bool PqlDeclaration::operator==(PqlDeclaration const& other) const {
  return (*name == *(other.name) && entity_type == other.entity_type);
}
