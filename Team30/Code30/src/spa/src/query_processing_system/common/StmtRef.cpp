#include "StmtRef.h"

#include "EntityType.h"

StmtRef::StmtRef() {
  ref_type = WILD;
}

StmtRef::StmtRef(std::shared_ptr<PqlDeclaration> declaration) {
  ref_type = DECLARATION;
  this->declaration = declaration;
}

StmtRef::StmtRef(int stmt_num) {
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
