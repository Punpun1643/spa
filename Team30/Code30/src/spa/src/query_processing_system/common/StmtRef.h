#pragma once
#include <memory>

#include "EntityType.h"
#include "PqlDeclaration.h"

class StmtRef {
 private:

public:
  StmtRef();

  explicit StmtRef(int stmt_num);

  explicit StmtRef(std::shared_ptr<PqlDeclaration> stmt_type);
};
