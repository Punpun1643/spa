#pragma once
#include <memory>
#include <vector>

#include "EntityType.h"
#include "PqlDeclaration.h"
#include "PqlReference.h"

class StmtRef : public PqlReference {
 private:
  std::vector<EntityType> const VALID_STMT_TYPES = {
      EntityType::STMT,  EntityType::READ, EntityType::PRINT, EntityType::CALL,
      EntityType::WHILE, EntityType::IF,   EntityType::ASSIGN};

 public:
  StmtRef();

  explicit StmtRef(int stmt_num);

  explicit StmtRef(const PqlDeclaration& declaration);
};
