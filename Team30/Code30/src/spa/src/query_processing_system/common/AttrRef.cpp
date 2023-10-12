#include "AttrRef.h"

#include <utility>

#include "query_processing_system/exceptions/InvalidSemanticsException.h"

const std::unordered_map<EntityType, AttrType> AttrRef::DEFAULT_ATTR_TYPE = {
    {EntityType::PROCEDURE, AttrType::PROC_NAME},
    {EntityType::VARIABLE, AttrType::VAR_NAME},
    {EntityType::CONSTANT, AttrType::VALUE},
    {EntityType::STMT, AttrType::STMT_NUM},
    {EntityType::ASSIGN, AttrType::STMT_NUM},
    {EntityType::IF, AttrType::STMT_NUM},
    {EntityType::WHILE, AttrType::STMT_NUM},
    {EntityType::PRINT, AttrType::STMT_NUM},
    {EntityType::READ, AttrType::STMT_NUM},
    {EntityType::CALL, AttrType::STMT_NUM}
};

const std::unordered_map<EntityType, AttrType> AttrRef::ATTR_TYPE_ALIASES = {
    {EntityType::CALL, AttrType::PROC_NAME},
    {EntityType::READ, AttrType::VAR_NAME},
    {EntityType::PRINT, AttrType::VAR_NAME}
};

void AttrRef::checkTypeCombinationValidity() const {
  EntityType entity_type = decl.getEntityType();
  if (DEFAULT_ATTR_TYPE.at(entity_type) != attr_type && !isAttrTypeAnAlias()) {
    throw InvalidSemanticsException("Invalid combination of EntityType and AttrType");
  }
}

AttrRef::AttrRef(PqlDeclaration decl)
:decl(std::move(decl)) {
  this->attr_type = DEFAULT_ATTR_TYPE.at(this->decl.getEntityType());
  checkTypeCombinationValidity();
}

AttrRef::AttrRef(PqlDeclaration decl, AttrType attr_type)
: decl(std::move(decl)), attr_type(attr_type) {
  checkTypeCombinationValidity();
}

bool AttrRef::isAttrTypeAnAlias() const {
  auto entity_type = decl.getEntityType();
  return (ATTR_TYPE_ALIASES.count(entity_type) == 1 &&
          ATTR_TYPE_ALIASES.at(entity_type) == attr_type);
}

PqlDeclaration AttrRef::getDecl() const {
  return decl;
}

AttrType AttrRef::getAttrType() const {
  return attr_type;
}

EntityType AttrRef::getEntityType() const {
  return decl.getEntityType();
}

AttrType AttrRef::getDefaultAttrType() const {
  return DEFAULT_ATTR_TYPE.at(decl.getEntityType());
}
