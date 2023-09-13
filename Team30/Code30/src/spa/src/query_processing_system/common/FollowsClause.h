#pragma once
#include <memory>

#include "StmtRef.h"
#include "SuchThatClause.h"
#include "../../program_knowledge_base/RelationType.h"


class FollowsClause : public SuchThatClause {
 private:
  std::unique_ptr<StmtRef> arg1;
  std::unique_ptr<StmtRef> arg2;
  RelationType relation_type = FOLLOWS;

 public:
  FollowsClause(std::unique_ptr<StmtRef> arg1, std::unique_ptr<StmtRef> arg2);
  std::unique_ptr<ClauseResult> evaluate(PkbApi& pkb) override;
};
