#pragma once
#include <memory>

#include "SuchThatClause.h"
#include "query_processing_system/references/StmtRef.h"

class AffectsClause : public SuchThatClause {
 public:
  AffectsClause(std::unique_ptr<StmtRef> arg1, std::unique_ptr<StmtRef> arg2);
};
