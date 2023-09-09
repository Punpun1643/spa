#include "StmtRef.h"

#include "EntityType.h"
#include "InvalidSemanticsException.h"

StmtRef::StmtRef(): ref_type(WILD) {}

StmtRef::StmtRef(std::shared_ptr<PqlDeclaration> declaration) {
  EntityType declaration_type = declaration->getEntityType();
  if (std::find(VALID_STMT_TYPES.begin(), VALID_STMT_TYPES.end(),
                declaration_type) == VALID_STMT_TYPES.end()) {
    throw InvalidSemanticsException("Not a statement type");
  }

  ref_type = DECLARATION;
  this->declaration = declaration;
}

StmtRef::StmtRef(int stmt_num) {
  if (stmt_num <= 0) {
    throw InvalidSemanticsException("Statement number cannot be less than 1.");
  }

  ref_type = NUMBER;
  this->stmt_num = stmt_num;
}

StmtRefType StmtRef::getStmtRefType() {
  return ref_type;
}

int StmtRef::getStmtNum() {
  if (ref_type == NUMBER) {
      return stmt_num;
  } else {
      throw std::runtime_error("StmtRef does not contain a statement number.");
  }
}

std::shared_ptr<PqlDeclaration> StmtRef::getDeclaration() {
  if (ref_type == DECLARATION) {
      return declaration;
  } else {
      throw std::runtime_error("StmtRef does not contain a PQL declaration.");
  }
}

EntityType StmtRef::getDeclarationType() {
  if (ref_type == DECLARATION) {
      return declaration->getEntityType();
  } else {
      throw std::runtime_error("StmtRef does not contain a PQL declaration.");
  }
}
