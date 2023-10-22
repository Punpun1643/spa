#pragma once
#include <memory>

#include "SuchThatClause.h"
#include "query_processing_system/references/StmtRef.h"

class ParentClause : public SuchThatClause {
 public:
  ParentClause(std::unique_ptr<StmtRef> arg1, std::unique_ptr<StmtRef> arg2,
               bool is_starred);
};
