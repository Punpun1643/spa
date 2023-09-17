#pragma once
#include <memory>

#include "../../program_knowledge_base/RelationType.h"
#include "StmtRef.h"
#include "SuchThatClause.h"

class FollowsClause : public SuchThatClause {
 public:
  FollowsClause(std::unique_ptr<StmtRef> arg1, std::unique_ptr<StmtRef> arg2);
};
