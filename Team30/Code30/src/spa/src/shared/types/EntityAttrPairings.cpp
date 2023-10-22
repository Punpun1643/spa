#include "EntityAttrPairings.h"

std::unordered_map<EntityType, AttrType> const
    EntityAttrPairings::DEFAULT_ATTR_TYPE = {
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

std::unordered_map<EntityType, AttrType> const
    EntityAttrPairings::ATTR_TYPE_ALIASES = {
        {EntityType::CALL, AttrType::PROC_NAME},
        {EntityType::READ, AttrType::VAR_NAME},
        {EntityType::PRINT, AttrType::VAR_NAME}};

bool EntityAttrPairings::IsDefaultPair(EntityType ent_type,
                                       AttrType attr_type) {
  return EntityAttrPairings::DEFAULT_ATTR_TYPE.at(ent_type) == attr_type;
}

bool EntityAttrPairings::IsAliasPair(EntityType ent_type, AttrType attr_type) {
  return (EntityAttrPairings::ATTR_TYPE_ALIASES.count(ent_type) == 1 &&
          EntityAttrPairings::ATTR_TYPE_ALIASES.at(ent_type) == attr_type);
}

AttrType EntityAttrPairings::GetDefaultAttrType(EntityType ent_type) {
  return DEFAULT_ATTR_TYPE.at(ent_type);
}
