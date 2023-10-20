#include "AttrRef.h"

#include <cassert>
#include <utility>

#include "query_processing_system/exceptions/InvalidSemanticsException.h"

std::unordered_map<EntityType, AttrType> const AttrRef::DEFAULT_ATTR_TYPE = {
    {EntityType::PROCEDURE, AttrType::PROC_NAME},
    {EntityType::VARIABLE, AttrType::VAR_NAME},
    {EntityType::CONSTANT, AttrType::VALUE},
    {EntityType::STMT, AttrType::STMT_NUM},
    {EntityType::ASSIGN, AttrType::STMT_NUM},
    {EntityType::IF, AttrType::STMT_NUM},
    {EntityType::WHILE, AttrType::STMT_NUM},
    {EntityType::PRINT, AttrType::STMT_NUM},
    {EntityType::READ, AttrType::STMT_NUM},
    {EntityType::CALL, AttrType::STMT_NUM}};

std::unordered_map<EntityType, AttrType> const AttrRef::ATTR_TYPE_ALIASES = {
    {EntityType::CALL, AttrType::PROC_NAME},
    {EntityType::READ, AttrType::VAR_NAME},
    {EntityType::PRINT, AttrType::VAR_NAME}};

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
  auto entity_type = decl.GetEntityType();
  return (ATTR_TYPE_ALIASES.count(entity_type) == 1 &&
          ATTR_TYPE_ALIASES.at(entity_type) == attr_type);
}

bool AttrRef::IsDefaultAttribute(EntityType ent_type, AttrType attr_type) {
  return AttrRef::DEFAULT_ATTR_TYPE.at(ent_type) == attr_type;
}

std::string AttrRef::GetAliasFromDefault(
    PKBQPSInterface& pkb, std::string const& default_value) const {
  /**
   * If called on an AttrRef that is an alias, converts the default value into
   * the alias type of the AttrRef. Otherwise, does nothing to the value.
   */
  if (IsAttrTypeAnAlias()) {
    return pkb.ConvertEntityValueToAlias(default_value, decl.GetEntityType(),
                                         GetDefaultAttrType(), attr_type);
  } else {
    return default_value;
  }
}

PqlDeclaration AttrRef::GetDecl() const {
  return decl;
}

AttrType AttrRef::GetDefaultAttrType() const {
  return DEFAULT_ATTR_TYPE.at(decl.GetEntityType());
}

AttrRefOutputType AttrRef::GetOutputType() const {
  assert(OUTPUT_TYPE_MAPPING.count(attr_type) == 1);
  return OUTPUT_TYPE_MAPPING.at(attr_type);
}

EntityType AttrRef::GetEntityType() const {
  return decl.GetEntityType();
}

AttrType AttrRef::GetAttrType() const {
  return attr_type;
}
