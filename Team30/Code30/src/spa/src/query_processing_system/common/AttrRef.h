#pragma once

#include "AttrType.h"
#include "PqlDeclaration.h"
#include "EntityType.h"
#include "program_knowledge_base/PKBQPSInterface.h"

#include <unordered_map>

class AttrRef {
 private:
  PqlDeclaration decl;
  AttrType attr_type;
  static const std::unordered_map<EntityType, AttrType> DEFAULT_ATTR_TYPE;
  static const std::unordered_map<EntityType, AttrType> ATTR_TYPE_ALIASES;

  void checkTypeCombinationValidity() const;
  AttrType getDefaultAttrType() const;

 public:
  explicit AttrRef(PqlDeclaration decl);

  AttrRef(PqlDeclaration decl, AttrType attr_type);

  std::string getRepresentationFromDefault(PKBQPSInterface& pkb, std::string const& default_value) const;

  bool isAttrTypeAnAlias() const;

  PqlDeclaration getDecl() const;
};
