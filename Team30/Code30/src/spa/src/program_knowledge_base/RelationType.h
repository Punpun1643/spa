#pragma once

enum class RelationType {
  FOLLOWS,
  FOLLOWS_STAR,
  PARENT,
  PARENT_STAR,
  USES_P,
  USES_S,
  MODIFIES_P,
  MODIFIES_S,
  CALLS,
  CALLS_STAR,
  NEXT,
  NEXT_STAR,
  AFFECTS,
};
