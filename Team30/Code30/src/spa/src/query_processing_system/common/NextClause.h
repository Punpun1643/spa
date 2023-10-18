#pragma once
#include <memory>

#include "StmtRef.h"
#include "SuchThatClause.h"

class NextClause : public SuchThatClause {
 public:
  NextClause(std::unique_ptr<StmtRef> arg1, std::unique_ptr<StmtRef> arg2,
             bool is_starred);
};
