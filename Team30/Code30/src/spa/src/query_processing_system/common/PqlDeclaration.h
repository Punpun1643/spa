#pragma once
#include <functional>
#include <memory>
#include <string>

#include "EntityType.h"

struct PqlDeclarationHash;

class PqlDeclaration {
  friend PqlDeclarationHash;

 private:
  std::string name;
  EntityType entity_type;

 public:
  PqlDeclaration(std::string name, EntityType entity_type);
  std::string getName() const;
  EntityType getEntityType() const;
  bool operator==(PqlDeclaration const& other) const;
};

struct PqlDeclarationHash {
  std::size_t operator()(PqlDeclaration const& obj) const {
    return std::hash<std::string>()(obj.name);
  }
};
