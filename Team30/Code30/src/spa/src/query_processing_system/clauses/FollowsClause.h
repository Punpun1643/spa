#pragma once
#include <memory>

#include "SuchThatClause.h"
#include "query_processing_system/references/StmtRef.h"

class FollowsClause : public SuchThatClause {
 public:
  FollowsClause(std::unique_ptr<StmtRef> arg1, std::unique_ptr<StmtRef> arg2,
                bool is_starred);
};
