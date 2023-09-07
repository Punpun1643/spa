#pragma once
#include "SuchThatClause.h"
#include "StmtRef.h"
#include <memory>

class FollowsClause: SuchThatClause{
 private:
  std::unique_ptr<StmtRef> arg1;
  std::unique_ptr<StmtRef> arg2;
 public:
  FollowsClause(std::unique_ptr<StmtRef> arg1, std::unique_ptr<StmtRef> arg2);
  ClauseResult evaluate(PKB pkb);
};

