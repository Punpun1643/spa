#pragma once

#include "AttrType.h"
#include "PqlDeclaration.h"
#include "EntityType.h"
#include "program_knowledge_base/PKBQPSInterface.h"

#include <unordered_map>
#include <unordered_set>

enum class AttrRefOutputType {NAME, INTEGER};

class AttrRef {
 private:
  PqlDeclaration decl;
  AttrType attr_type;
  static const std::unordered_map<EntityType, AttrType> DEFAULT_ATTR_TYPE;
  static const std::unordered_map<EntityType, AttrType> ATTR_TYPE_ALIASES;
  static const std::unordered_map<AttrType, AttrRefOutputType> OUTPUT_TYPE_MAPPING;

  void CheckTypeCombinationValidity() const;
  AttrType GetDefaultAttrType() const;

 public:
  explicit AttrRef(PqlDeclaration decl);

  AttrRef(PqlDeclaration decl, AttrType attr_type);

  std::string GetAliasFromDefault(PKBQPSInterface& pkb, std::string const& default_value) const;

//  std::unordered_set<std::string> GetDefaultsFromAlias(PKBQPSInterface& pkb, std::string const& alias_value) const;

  bool IsAttrTypeAnAlias() const;

  AttrRefOutputType GetOutputType() const;

  PqlDeclaration GetDecl() const;

  AttrType GetAttrType() const;

  EntityType GetEntityType() const;
};
