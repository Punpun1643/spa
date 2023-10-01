#include "PqlReference.h"

#include <cassert>
#include <utility>

#include "EntityType.h"

PqlReference::PqlReference()
    : ref_type(PqlRefType::WILD),
      ref_declaration(std::nullopt),
      ref_value(std::nullopt) {}

PqlReference::PqlReference(PqlDeclaration declaration)
    : ref_type(PqlRefType::DECLARATION),
      ref_declaration(std::move(declaration)),
      ref_value(std::nullopt) {}

PqlReference::PqlReference(std::string ref_value)
    : ref_type(PqlRefType::VALUE),
      ref_declaration(std::nullopt),
      ref_value(std::move(ref_value)) {}

PqlRefType PqlReference::getRefType() const { return ref_type; }

std::string PqlReference::getValue() const {
  assert(ref_type == PqlRefType::VALUE);
  return ref_value.value();
}

PqlDeclaration PqlReference::getDeclaration() const {
  assert(ref_type == PqlRefType::DECLARATION);
  return ref_declaration.value();
}

EntityType PqlReference::getDeclarationType() const {
  assert(ref_type == PqlRefType::DECLARATION);
  return ref_declaration->getEntityType();
}

PqlReference::~PqlReference() = default;
