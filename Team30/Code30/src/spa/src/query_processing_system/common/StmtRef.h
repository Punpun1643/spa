#pragma once
#include <memory>

#include "EntityType.h"
#include "PqlDeclaration.h"

enum StmtRefType {
  WILD,
  NUMBER,
  DECLARATION
};


class StmtRef {
 private:
  StmtRefType ref_type;
  int stmt_num;
  std::shared_ptr<PqlDeclaration> declaration;

 public:
  StmtRef();

  explicit StmtRef(int stmt_num);

  explicit StmtRef(std::shared_ptr<PqlDeclaration> stmt_type);

  StmtRefType getStmtRefType();

  int getStmtNum();

  std::shared_ptr<PqlDeclaration> getDeclaration();
};
