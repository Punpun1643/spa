#include "PqlReference.h"

#include <cassert>
#include <utility>

#include "EntityType.h"

PqlReference::PqlReference()
    : ref_type(PqlRefType::WILD), ref_value(std::nullopt) {}

PqlReference::PqlReference(std::shared_ptr<PqlDeclaration const> declaration)
    : ref_type(PqlRefType::DECLARATION),
      ref_declaration(std::move(declaration)),
      ref_value(std::nullopt) {}

PqlReference::PqlReference(std::string ref_value)
    : ref_type(PqlRefType::VALUE), ref_value(std::move(ref_value)) {}

PqlRefType PqlReference::getRefType() const { return ref_type; }

std::string PqlReference::getValue() const {
  assert(ref_type == PqlRefType::VALUE);
  return ref_value.value();
}

std::shared_ptr<PqlDeclaration const> PqlReference::getDeclaration() const {
  assert(ref_type == PqlRefType::DECLARATION);
  return ref_declaration;
}

EntityType PqlReference::getDeclarationType() const {
  assert(ref_type == PqlRefType::DECLARATION);
  return ref_declaration->getEntityType();
}

PqlReference::~PqlReference() = default;
