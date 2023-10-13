#pragma once

#include "AttrType.h"
#include "PqlDeclaration.h"
#include "EntityType.h"
#include "program_knowledge_base/PKBQPSInterface.h"

#include <unordered_map>

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

  std::string GetRepresentationFromDefault(PKBQPSInterface& pkb, std::string const& default_value) const;

  bool IsAttrTypeAnAlias() const;

  AttrRefOutputType getOutputType() const;

  PqlDeclaration GetDecl() const;
};
