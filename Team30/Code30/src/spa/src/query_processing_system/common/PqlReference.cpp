#include "PqlReference.h"

#include <utility>

#include "EntityType.h"
#include "InvalidSemanticsException.h"

PqlReference::PqlReference(): ref_type(WILD), ref_value(std::nullopt) {}

PqlReference::PqlReference(std::shared_ptr<const PqlDeclaration> declaration):
 ref_type(DECLARATION), ref_declaration(std::move(declaration)),
 ref_value(std::nullopt) {}

PqlReference::PqlReference(std::string ref_value)
    : ref_type(VALUE), ref_value(std::move(ref_value)) {}

PqlRefType PqlReference::getRefType() const { return ref_type; }

std::string PqlReference::getValue() const {
  if (ref_type == VALUE) {
    return ref_value.value();
  } else {
    throw std::runtime_error("PqlReference does not contain a value.");
  }
}

std::shared_ptr<const PqlDeclaration> PqlReference::getDeclaration() const {
  if (ref_type == DECLARATION) {
    return ref_declaration;
  } else {
    throw std::runtime_error("PqlReference does not contain a PQL declaration.");
  }
}

EntityType PqlReference::getDeclarationType() const {
  if (ref_type == DECLARATION) {
    return ref_declaration->getEntityType();
  } else {
    throw std::runtime_error("PqlReference does not contain a PQL declaration.");
  }
}

PqlReference::~PqlReference() = default;
