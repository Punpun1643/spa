#pragma once
#include <memory>

#include "StmtRef.h"
#include "SuchThatClause.h"

class ParentStarClause : public SuchThatClause {
 public:
  ParentStarClause(std::unique_ptr<StmtRef> arg1,
                   std::unique_ptr<StmtRef> arg2);
};
