#include "AttrRef.h"

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
  auto entity_type = decl.getEntityType();
  return (ATTR_TYPE_ALIASES.count(entity_type) == 1 &&
          ATTR_TYPE_ALIASES.at(entity_type) == attr_type);
}

std::string AttrRef::GetRepresentationFromDefault(
    PKBQPSInterface& pkb, std::string const& default_value) const {
  if (IsAttrTypeAnAlias()) {
    auto output = pkb.convertEntityAttribute(
        default_value, decl.getEntityType(), GetDefaultAttrType(), attr_type);
    assert(output.size() == 1);
    for (auto& elem : output) {
      return elem;
    }
    assert(false);  // should not get here.
  } else {
    return default_value;
  }
}

PqlDeclaration AttrRef::GetDecl() const { return decl; }

AttrType AttrRef::GetDefaultAttrType() const {
  return DEFAULT_ATTR_TYPE.at(decl.getEntityType());
}
