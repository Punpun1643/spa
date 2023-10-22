#include "StmtRef.h"

#include <cassert>

#include "query_processing_system/exceptions/InvalidSemanticsException.h"

StmtRef::StmtRef() : PqlReference() {}

StmtRef::StmtRef(int stmt_num) : PqlReference(std::to_string(stmt_num)) {}

StmtRef::StmtRef(PqlDeclaration const& declaration)
    : PqlReference(declaration) {
  EntityType declaration_type = declaration.GetEntityType();
  if (std::find(VALID_STMT_TYPES.begin(), VALID_STMT_TYPES.end(),
                declaration_type) == VALID_STMT_TYPES.end()) {
    throw InvalidSemanticsException("Declaration is not a statement type");
  }
}
