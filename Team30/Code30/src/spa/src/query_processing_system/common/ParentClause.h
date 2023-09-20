#pragma once
#include <memory>

#include "StmtRef.h"
#include "SuchThatClause.h"

class ParentClause : public SuchThatClause {
 public:
  ParentClause(std::unique_ptr<StmtRef> arg1, std::unique_ptr<StmtRef> arg2);
};
