#pragma once
#include <memory>

#include "StmtRef.h"
#include "SuchThatClause.h"

class FollowsStarClause : public SuchThatClause {
 public:
  FollowsStarClause(std::unique_ptr<StmtRef> arg1, std::unique_ptr<StmtRef> arg2);
};
