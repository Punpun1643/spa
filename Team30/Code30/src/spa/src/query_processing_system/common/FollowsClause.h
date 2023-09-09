#pragma once
#include <memory>

#include "StmtRef.h"
#include "SuchThatClause.h"

class FollowsClause : public SuchThatClause {
 private:
  std::unique_ptr<StmtRef> arg1;
  std::unique_ptr<StmtRef> arg2;

 public:
  FollowsClause(std::unique_ptr<StmtRef> arg1, std::unique_ptr<StmtRef> arg2);
  std::unique_ptr<ClauseResult> evaluate(PKB pkb);
};
