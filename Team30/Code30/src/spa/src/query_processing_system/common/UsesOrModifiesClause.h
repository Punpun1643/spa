#pragma once
#include <memory>
#include <string>

#include "EntRef.h"
#include "PqlReference.h"
#include "EntityType.h"
#include "SuchThatClause.h"

class UsesOrModifiesClause : public SuchThatClause {
 private:
  std::vector<EntityType> valid_decl_types_for_arg_1;
  std::string invalid_arg_1_decl_type_msg;

  std::string INVALID_ARG_2_DECL_TYPE_MSG =
      "If the 2nd arg of Uses/Modifies is a declaration, it must have the var type.";
  std::string INVALID_ARG_1_REF_TYPE_MSG =
      "1st argument of Uses/Modifies cannot be a wildcard";
  std::vector<EntityType> VALID_DECL_TYPES_FOR_ARG_2 = {EntityType::VARIABLE};
  std::vector<PqlRefType> VALID_REF_TYPES_FOR_ARG_1 = {PqlRefType::VALUE,
                                                       PqlRefType::DECLARATION};

 protected:
  std::unique_ptr<ClauseResult> evaluateWildDeclaration(PkbApi& pkb) override;
  std::unique_ptr<ClauseResult> evaluateWildValue(PkbApi& pkb) override;
  std::unique_ptr<ClauseResult> evaluateWildWild(PkbApi& pkb) override;
  UsesOrModifiesClause(std::unique_ptr<const PqlReference> arg1,
                       std::unique_ptr<const EntRef> arg2,
                       RelationType relation_type,
                       std::vector<EntityType> valid_decl_types_for_arg_1,
                       std::string invalid_arg_1_decl_type_msg);

 public:
  virtual ~UsesOrModifiesClause();
};
