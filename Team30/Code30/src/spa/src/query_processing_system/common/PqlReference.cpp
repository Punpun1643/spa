#include "PqlReference.h"

#include <utility>

#include "EntityType.h"

PqlReference::PqlReference() : ref_type(WILD), ref_value(std::nullopt) {}

PqlReference::PqlReference(std::shared_ptr<PqlDeclaration const> declaration)
    : ref_type(DECLARATION),
      ref_declaration(std::move(declaration)),
      ref_value(std::nullopt) {}

PqlReference::PqlReference(std::string ref_value)
    : ref_type(VALUE), ref_value(std::move(ref_value)) {}

PqlRefType PqlReference::getRefType() const { return ref_type; }

std::string PqlReference::getValue() const {
  assert(ref_type == VALUE);
  return ref_value.value();
}

std::shared_ptr<PqlDeclaration const> PqlReference::getDeclaration() const {
  assert(ref_type == DECLARATION);
  return ref_declaration;
}

EntityType PqlReference::getDeclarationType() const {
  assert(ref_type == DECLARATION);
  return ref_declaration->getEntityType();
}

PqlReference::~PqlReference() = default;
