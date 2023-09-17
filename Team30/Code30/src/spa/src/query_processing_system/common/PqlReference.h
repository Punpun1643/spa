#pragma once
#include <memory>
#include <string>

#include "EntityType.h"
#include "PqlDeclaration.h"

enum PqlRefType { WILD, VALUE, DECLARATION };

class PqlReference {
 private:
  const PqlRefType ref_type;
  const std::optional<std::string> ref_value;
  const std::shared_ptr<const PqlDeclaration> ref_declaration; // can be null

 protected:
  PqlReference();

  explicit PqlReference(std::string ref_value);

  explicit PqlReference(std::shared_ptr<const PqlDeclaration> declaration);

 public:
  virtual ~PqlReference();

  PqlRefType getRefType() const;

  std::string getValue() const;

  std::shared_ptr<const PqlDeclaration> getDeclaration() const;

  EntityType getDeclarationType() const;
};
