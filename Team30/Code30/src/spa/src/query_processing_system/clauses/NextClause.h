#pragma once
#include <memory>

#include "SuchThatClause.h"
#include "query_processing_system/references/StmtRef.h"

class NextClause : public SuchThatClause {
 public:
  NextClause(std::unique_ptr<StmtRef> arg1, std::unique_ptr<StmtRef> arg2,
             bool is_starred);
};
