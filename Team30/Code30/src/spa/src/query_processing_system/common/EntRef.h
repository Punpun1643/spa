#pragma once
#include <memory>
#include <vector>

#include "EntityType.h"
#include "PqlDeclaration.h"
#include "PqlReference.h"

class EntRef : public PqlReference {
 private:
  std::vector<EntityType> const VALID_ENT_TYPES = {
      EntityType::VARIABLE, EntityType::PROCEDURE, EntityType::CONSTANT};

 public:
  EntRef();

  explicit EntRef(std::string entity_value);

  explicit EntRef(PqlDeclaration const& declaration);
};
