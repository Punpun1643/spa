#pragma once
#include <memory>
#include <optional>
#include <string>

#include "EntityType.h"
#include "PqlDeclaration.h"

enum class PqlRefType { WILD, VALUE, DECLARATION };

class PqlReference {
 private:
  PqlRefType const ref_type;
  std::optional<std::string> const ref_value;
  std::shared_ptr<PqlDeclaration const> const ref_declaration;  // can be null

 protected:
  PqlReference();

  explicit PqlReference(std::string ref_value);

  explicit PqlReference(std::shared_ptr<PqlDeclaration const> declaration);

 public:
  virtual ~PqlReference();

  PqlRefType getRefType() const;

  std::string getValue() const;

  std::shared_ptr<PqlDeclaration const> getDeclaration() const;

  EntityType getDeclarationType() const;
};
