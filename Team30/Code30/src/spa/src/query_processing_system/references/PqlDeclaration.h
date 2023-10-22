#pragma once
#include <functional>
#include <memory>
#include <string>

#include "shared/types/EntityType.h"

struct PqlDeclarationHash;

class PqlDeclaration {
  friend PqlDeclarationHash;

 private:
  std::string name;
  EntityType entity_type;

 public:
  PqlDeclaration(std::string name, EntityType entity_type);
  EntityType GetEntityType() const;
  bool operator==(PqlDeclaration const& other) const;
};

struct PqlDeclarationHash {
  std::size_t operator()(PqlDeclaration const& obj) const {
    return std::hash<std::string>()(obj.name);
  }
};
