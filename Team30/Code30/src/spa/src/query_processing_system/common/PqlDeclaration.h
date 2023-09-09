#pragma once
#include <memory>
#include <string>

#include "EntityType.h"

struct PqlDeclarationHash;

class PqlDeclaration {
  friend PqlDeclarationHash;

 private:
  std::shared_ptr<std::string const> name;
  EntityType const entity_type;

 public:
  PqlDeclaration(std::shared_ptr<std::string const> name,
                 EntityType entity_type);
};

struct PqlDeclarationHash {
  std::size_t operator()(PqlDeclaration const& obj) const {
    return std::hash<std::string>()(*(obj.name));
  }
};
