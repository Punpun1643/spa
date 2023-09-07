#include "FollowsClause.h"

FollowsClause::FollowsClause(std::unique_ptr<StmtRef> arg1, std::unique_ptr<StmtRef> arg2) {
  this->arg1 = std::move(arg1);
  this->arg2 = std::move(arg2);
  // verify inputs are semantically valid.
}

ClauseResult FollowsClause::evaluate(PKB pkb) {
  return ClauseResult();
}
