#include "StmtRef.h"

#include <cassert>

#include "query_processing_system/exceptions/InvalidSemanticsException.h"

StmtRef::StmtRef() : PqlReference() {}

StmtRef::StmtRef(int stmt_num) : PqlReference(std::to_string(stmt_num)) {
  assert(stmt_num >= 1);  // should have been caught at syntax checker stage
}

StmtRef::StmtRef(std::shared_ptr<PqlDeclaration const> declaration)
    : PqlReference(declaration) {
  EntityType declaration_type = declaration->getEntityType();
  if (std::find(VALID_STMT_TYPES.begin(), VALID_STMT_TYPES.end(),
                declaration_type) == VALID_STMT_TYPES.end()) {
    throw InvalidSemanticsException("Declaration is not a statement type");
  }
}
