#pragma once
#include <memory>
#include <string>

#include "EntRef.h"
#include "StmtRef.h"
#include "UsesOrModifiesClause.h"

class UsesSClause : public UsesOrModifiesClause {
 public:
  UsesSClause(std::unique_ptr<StmtRef> arg1, std::unique_ptr<EntRef> arg2);
};
