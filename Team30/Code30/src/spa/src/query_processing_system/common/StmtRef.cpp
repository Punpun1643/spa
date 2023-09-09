#include "StmtRef.h"

#include "EntityType.h"

StmtRef::StmtRef() {
  /* Initializes a wild statement reference */
}

StmtRef::StmtRef(std::shared_ptr<PqlDeclaration> declaration) {}
StmtRef::StmtRef(int stmt_num) {}
