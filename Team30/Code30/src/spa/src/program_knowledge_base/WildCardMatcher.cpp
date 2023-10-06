#include "WildCardMatcher.h"

WildCardMatcher::WildCardMatcher() {
  lhs_map[RelationType::FOLLOWS] = EntityType::STMT;
  lhs_map[RelationType::FOLLOWS_STAR] = EntityType::STMT;
  lhs_map[RelationType::PARENT] = EntityType::STMT;
  lhs_map[RelationType::PARENT_STAR] = EntityType::STMT;
  lhs_map[RelationType::USES_P] = EntityType::PROCEDURE;
  lhs_map[RelationType::USES_S] = EntityType::STMT;
  lhs_map[RelationType::MODIFIES_P] = EntityType::PROCEDURE;
  lhs_map[RelationType::MODIFIES_S] = EntityType::STMT;

  lhs_map[RelationType::CALLS] = EntityType::PROCEDURE;
  lhs_map[RelationType::CALLS_STAR] = EntityType::PROCEDURE;
  lhs_map[RelationType::NEXT] = EntityType::STMT;
  lhs_map[RelationType::NEXT_STAR] = EntityType::STMT;
  lhs_map[RelationType::AFFECTS] = EntityType::ASSIGN;

  rhs_map[RelationType::FOLLOWS] = EntityType::STMT;
  rhs_map[RelationType::FOLLOWS_STAR] = EntityType::STMT;
  rhs_map[RelationType::PARENT] = EntityType::STMT;
  rhs_map[RelationType::PARENT_STAR] = EntityType::STMT;
  rhs_map[RelationType::USES_P] = EntityType::VARIABLE;
  rhs_map[RelationType::USES_S] = EntityType::VARIABLE;
  rhs_map[RelationType::MODIFIES_P] = EntityType::VARIABLE;
  rhs_map[RelationType::MODIFIES_S] = EntityType::VARIABLE;

  rhs_map[RelationType::CALLS] = EntityType::PROCEDURE;
  rhs_map[RelationType::CALLS_STAR] = EntityType::PROCEDURE;
  rhs_map[RelationType::NEXT] = EntityType::STMT;
  rhs_map[RelationType::NEXT_STAR] = EntityType::STMT;
  rhs_map[RelationType::AFFECTS] = EntityType::ASSIGN;
};

EntityType WildCardMatcher::translateLHSWild(
    RelationType type) {
  return lhs_map[type];
};

EntityType WildCardMatcher::translateRHSWild(
    RelationType type) {
  return rhs_map[type];
};
