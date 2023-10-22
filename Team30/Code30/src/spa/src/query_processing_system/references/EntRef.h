#pragma once

#include <memory>
#include <string>
#include <vector>

#include "PqlDeclaration.h"
#include "PqlReference.h"
#include "shared/types/EntityType.h"

class EntRef : public PqlReference {
 private:
  std::vector<EntityType> const VALID_ENT_TYPES = {
      EntityType::VARIABLE, EntityType::PROCEDURE, EntityType::CONSTANT};

 public:
  EntRef();

  explicit EntRef(std::string entity_value);

  explicit EntRef(PqlDeclaration const& declaration);
};
