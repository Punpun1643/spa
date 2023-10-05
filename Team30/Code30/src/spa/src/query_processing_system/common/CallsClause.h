#pragma once
#include "EntRef.h"
#include "SuchThatClause.h"
#include "EntityType.h"

class CallsClause: public SuchThatClause {
 private:
  static const std::vector<EntityType> ALLOWED_DECL_TYPES;
  static const std::string INVALID_DECL_TYPE_ERROR_MSG;
 public:
  CallsClause(std::unique_ptr<EntRef> arg1, std::unique_ptr<EntRef> arg2, bool is_starred);
};
