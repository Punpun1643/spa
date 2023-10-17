#pragma once
#include <memory>

#include "StmtRef.h"
#include "SuchThatClause.h"

class AffectsClause : public SuchThatClause {
 public:
  AffectsClause(std::unique_ptr<StmtRef> arg1, std::unique_ptr<StmtRef> arg2);
};
