#pragma once
#include <string>
#include <memory>
#include <functional>

#include "EntityType.h"

struct PqlDeclarationHash;

class PqlDeclaration {
  friend PqlDeclarationHash;
 private:
  const std::shared_ptr<const std::string> name;
  EntityType const entity_type;

 public:
  PqlDeclaration(std::shared_ptr<const std::string> name, EntityType entity_type);
  std::shared_ptr<const std::string> getName() const;
  EntityType getEntityType() const;
  bool operator==(const PqlDeclaration &other) const;
};

struct PqlDeclarationHash {
  std::size_t operator()(const PqlDeclaration& obj) const {
    return std::hash<std::string>()(*(obj.name));
  }
};
