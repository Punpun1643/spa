#pragma once

#include <unordered_map>
#include <unordered_set>

#include "../program_knowledge_base/RelationType.h"
#include "shared/types/EntityType.h"

class WildCardMatcher {
  std::unordered_map<RelationType, EntityType> lhs_map;
  std::unordered_map<RelationType, EntityType> rhs_map;

 public:
  WildCardMatcher();
  ~WildCardMatcher() = default;
  EntityType translateLHSWild(RelationType type);
  EntityType translateRHSWild(RelationType type);
};
