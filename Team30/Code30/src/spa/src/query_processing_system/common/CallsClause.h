#pragma once
#include "EntRef.h"
#include "EntityType.h"
#include "SuchThatClause.h"

class CallsClause : public SuchThatClause {
 private:
  static std::vector<EntityType> const ALLOWED_DECL_TYPES;
  static std::string const INVALID_DECL_TYPE_ERROR_MSG;

 public:
  CallsClause(std::unique_ptr<EntRef> arg1, std::unique_ptr<EntRef> arg2,
              bool is_starred);
};
