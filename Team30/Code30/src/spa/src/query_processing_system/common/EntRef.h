#pragma once
#include <memory>

#include "EntityType.h"
#include "PqlDeclaration.h"
#include "PqlReference.h"

class EntRef : public PqlReference {
 private:
  std::vector<EntityType> const VALID_ENT_TYPES = {VARIABLE, PROCEDURE,
                                                   CONSTANT};

 public:
  EntRef();

  explicit EntRef(std::string entity_value);

  explicit EntRef(std::shared_ptr<PqlDeclaration const> declaration);
};
