#pragma once

#include "AttrType.h"
#include "PqlDeclaration.h"
#include "EntityType.h"

class AttrRef {
 private:
  PqlDeclaration decl;
  AttrType attr_type;
  static const std::unordered_map<EntityType, AttrType> DEFAULT_ATTR_TYPE;
  static const std::unordered_map<EntityType, AttrType> ATTR_TYPE_ALIASES;

  void checkTypeCombinationValidity() const;

 public:
  explicit AttrRef(PqlDeclaration decl);

  AttrRef(PqlDeclaration decl, AttrType attr_type);

  AttrType getDefaultAttrType() const;

  AttrType getAttrType() const;

  EntityType getEntityType() const;

  bool isAttrTypeAnAlias() const;

  PqlDeclaration getDecl() const;
};
