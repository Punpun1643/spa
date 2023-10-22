#include "PqlReference.h"

#include <utility>
#include <stdexcept>

#include "shared/types/EntityType.h"

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

PqlRefType PqlReference::GetRefType() const {
  return ref_type;
}

std::string PqlReference::GetValue() const {
  if (ref_type != PqlRefType::VALUE) {
    throw std::logic_error("PqlRef does not contain a value.");
  }
  return ref_value.value();
}

PqlDeclaration PqlReference::GetDeclaration() const {
  if (ref_type != PqlRefType::DECLARATION) {
    throw std::logic_error("PqlRef does not contain a declaration.");
  }
  return ref_declaration.value();
}

EntityType PqlReference::GetDeclarationType() const {
  if (ref_type != PqlRefType::DECLARATION) {
    throw std::logic_error("PqlRef does not contain a value.");
  }
  return ref_declaration->GetEntityType();
}

PqlReference::~PqlReference() = default;
