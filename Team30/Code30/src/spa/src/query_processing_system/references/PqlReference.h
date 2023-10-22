#pragma once
#include <memory>
#include <optional>
#include <string>

#include "PqlDeclaration.h"
#include "shared/types/EntityType.h"

enum class PqlRefType { WILD, VALUE, DECLARATION };

class PqlReference {
 private:
  PqlRefType const ref_type;
  std::optional<std::string> const ref_value;
  std::optional<PqlDeclaration> const ref_declaration;  // can be null

 protected:
  PqlReference();

  explicit PqlReference(std::string ref_value);

  explicit PqlReference(PqlDeclaration declaration);

 public:
  virtual ~PqlReference();

  PqlRefType GetRefType() const;

  std::string GetValue() const;

  PqlDeclaration GetDeclaration() const;

  EntityType GetDeclarationType() const;
};
