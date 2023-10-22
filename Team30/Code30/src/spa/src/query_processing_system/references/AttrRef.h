#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>

#include "PqlDeclaration.h"
#include "program_knowledge_base/PKBQPSInterface.h"
#include "shared/types/AttrType.h"
#include "shared/types/EntityAttrPairings.h"
#include "shared/types/EntityType.h"

enum class AttrRefOutputType { NAME, INTEGER };

class AttrRef {
 private:
  PqlDeclaration decl;
  AttrType attr_type;
  static std::unordered_map<AttrType, AttrRefOutputType> const
      OUTPUT_TYPE_MAPPING;

  void CheckTypeCombinationValidity() const;
  AttrType GetDefaultAttrType() const;

 public:
  explicit AttrRef(PqlDeclaration decl);

  AttrRef(PqlDeclaration decl, AttrType attr_type);

  std::string GetAliasFromDefault(PKBQPSInterface& pkb,
                                  std::string const& default_value) const;

  bool IsAttrTypeAnAlias() const;

  AttrRefOutputType GetOutputType() const;

  PqlDeclaration GetDecl() const;

  AttrType GetAttrType() const;

  EntityType GetEntityType() const;
};
