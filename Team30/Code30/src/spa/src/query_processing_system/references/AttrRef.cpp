#include "AttrRef.h"

#include <string>
#include <utility>

#include "query_processing_system/exceptions/InvalidSemanticsException.h"

std::unordered_map<AttrType, AttrRefOutputType> const
    AttrRef::OUTPUT_TYPE_MAPPING = {
        {AttrType::PROC_NAME, AttrRefOutputType::NAME},
        {AttrType::VAR_NAME, AttrRefOutputType::NAME},
        {AttrType::VALUE, AttrRefOutputType::INTEGER},
        {AttrType::STMT_NUM, AttrRefOutputType::INTEGER},
};

void AttrRef::CheckTypeCombinationValidity() const {
  if (GetDefaultAttrType() != attr_type && !IsAttrTypeAnAlias()) {
    throw InvalidSemanticsException(
        "Invalid combination of EntityType and AttrType");
  }
}

AttrRef::AttrRef(PqlDeclaration decl) : decl(std::move(decl)) {
  this->attr_type = GetDefaultAttrType();
  CheckTypeCombinationValidity();
}

AttrRef::AttrRef(PqlDeclaration decl, AttrType attr_type)
    : decl(std::move(decl)), attr_type(attr_type) {
  CheckTypeCombinationValidity();
}

bool AttrRef::IsAttrTypeAnAlias() const {
  return EntityAttrPairings::IsAliasPair(decl.GetEntityType(), attr_type);
}

std::string AttrRef::GetAliasFromDefault(
    PKBQPSInterface& pkb, std::string const& default_value) const {
  /**
   * If called on an AttrRef that is an alias, converts the default value into
   * the alias type of the AttrRef. Otherwise, does nothing to the value.
   */
  if (IsAttrTypeAnAlias()) {
    return pkb.ConvertEntityValueToAlias(default_value, decl.GetEntityType(),
                                         attr_type);
  } else {
    return default_value;
  }
}

PqlDeclaration AttrRef::GetDecl() const {
  return decl;
}

AttrType AttrRef::GetDefaultAttrType() const {
  return EntityAttrPairings::GetDefaultAttrType(decl.GetEntityType());
}

AttrRefOutputType AttrRef::GetOutputType() const {
  return OUTPUT_TYPE_MAPPING.at(attr_type);
}

EntityType AttrRef::GetEntityType() const {
  return decl.GetEntityType();
}

AttrType AttrRef::GetAttrType() const {
  return attr_type;
}
