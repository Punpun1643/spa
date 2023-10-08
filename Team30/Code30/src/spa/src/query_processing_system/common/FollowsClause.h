#pragma once
#include <memory>

#include "StmtRef.h"
#include "SuchThatClause.h"

class FollowsClause : public SuchThatClause {
 public:
  FollowsClause(std::unique_ptr<StmtRef> arg1, std::unique_ptr<StmtRef> arg2,
                bool is_starred);
};
