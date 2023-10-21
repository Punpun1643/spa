#pragma once

#include <memory>
#include <string>
#include <vector>

#include "EntRef.h"
#include "EntityType.h"
#include "PqlReference.h"
#include "SuchThatClause.h"

class UsesOrModifiesClause : public SuchThatClause {
 private:
  std::string INVALID_ARG_2_DECL_TYPE_MSG =
      "If the 2nd arg of Uses/Modifies is a declaration, it must have the var "
      "type.";
  std::string INVALID_ARG_1_REF_TYPE_MSG =
      "1st argument of Uses/Modifies cannot be a wildcard";
  std::vector<EntityType> VALID_DECL_TYPES_FOR_ARG_2 = {EntityType::VARIABLE};
  std::vector<PqlRefType> VALID_REF_TYPES_FOR_ARG_1 = {PqlRefType::VALUE,
                                                       PqlRefType::DECLARATION};

 protected:
  std::unique_ptr<ClauseResult> EvaluateWildDeclaration(
      PKBQPSInterface& pkb) override;
  std::unique_ptr<ClauseResult> EvaluateWildValue(
      PKBQPSInterface& pkb) override;
  std::unique_ptr<ClauseResult> EvaluateWildWild(PKBQPSInterface& pkb) override;
  UsesOrModifiesClause(std::unique_ptr<PqlReference const> arg1,
                       std::unique_ptr<EntRef const> arg2,
                       RelationType relation_type);

 public:
  virtual ~UsesOrModifiesClause();
};
