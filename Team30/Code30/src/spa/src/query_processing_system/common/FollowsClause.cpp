#include "FollowsClause.h"

FollowsClause::FollowsClause(std::unique_ptr<StmtRef> arg1,
                             std::unique_ptr<StmtRef> arg2):
arg1(std::move(arg1)), arg2(std::move(arg2)){
  // verify inputs are semantically valid.
}

std::unique_ptr<ClauseResult> FollowsClause::evaluate(PKB pkb) {
  return std::make_unique<ClauseResult>();
}
