#include "StmtRef.h"

#include "InvalidSemanticsException.h"

StmtRef::StmtRef(): PqlReference() {}

StmtRef::StmtRef(int stmt_num)
    : PqlReference(std::to_string(stmt_num)) {
  if (stmt_num <= 0) {
    throw InvalidSemanticsException("Statement number cannot be less than 1.");
  }
}

StmtRef::StmtRef(std::shared_ptr<const PqlDeclaration> declaration)
    :PqlReference(declaration) {
  EntityType declaration_type = declaration->getEntityType();
  if (std::find(VALID_STMT_TYPES.begin(), VALID_STMT_TYPES.end(),
                declaration_type) == VALID_STMT_TYPES.end()) {
    throw InvalidSemanticsException("Declaration is not a statement type");
  }
}
