#pragma once
#include <memory>

#include "EntityType.h"
#include "PqlDeclaration.h"
#include "PqlReference.h"

class StmtRef: public PqlReference {
 private:
  std::vector<EntityType> const VALID_STMT_TYPES = {STMT,  READ, PRINT, CALL,
                                                    WHILE, IF,   ASSIGN};

 public:
  StmtRef();

  explicit StmtRef(int stmt_num);

  explicit StmtRef(std::shared_ptr<const PqlDeclaration> declaration);
};
