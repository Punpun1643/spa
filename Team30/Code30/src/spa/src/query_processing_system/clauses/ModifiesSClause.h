#pragma once
#include <memory>
#include <string>

#include "UsesOrModifiesClause.h"
#include "query_processing_system/references/EntRef.h"
#include "query_processing_system/references/StmtRef.h"

class ModifiesSClause : public UsesOrModifiesClause {
 public:
  ModifiesSClause(std::unique_ptr<StmtRef> arg1, std::unique_ptr<EntRef> arg2);
};
