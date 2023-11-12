#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Clause.h"
#include "query_processing_system/references/PqlReference.h"
#include "shared/types/RelationType.h"

class SuchThatClause : public Clause {
  // Template Method design pattern
 private:
  RelationType const relation_type;

 protected:
  std::unique_ptr<PqlReference const> arg1;
  std::unique_ptr<PqlReference const> arg2;
  SuchThatClause(std::unique_ptr<PqlReference const> arg1,
                 std::unique_ptr<PqlReference const> arg2,
                 RelationType relation_type);

  virtual std::unique_ptr<ClauseResult> EvaluateWildWild(PKBQPSInterface& pkb);
  virtual std::unique_ptr<ClauseResult> EvaluateValueWild(PKBQPSInterface& pkb);
  virtual std::unique_ptr<ClauseResult> EvaluateWildValue(PKBQPSInterface& pkb);
  virtual std::unique_ptr<ClauseResult> EvaluateValueValue(
      PKBQPSInterface& pkb);
  virtual std::unique_ptr<ClauseResult> EvaluateDeclarationWild(
      PKBQPSInterface& pkb);
  virtual std::unique_ptr<ClauseResult> EvaluateWildDeclaration(
      PKBQPSInterface& pkb);
  virtual std::unique_ptr<ClauseResult> EvaluateDeclarationValue(
      PKBQPSInterface& pkb);
  virtual std::unique_ptr<ClauseResult> EvaluateValueDeclaration(
      PKBQPSInterface& pkb);
  virtual std::unique_ptr<ClauseResult> EvaluateDeclarationDeclaration(
      PKBQPSInterface& pkb);

  void CheckDeclarationArgEntityType(
      int arg_num, std::vector<EntityType> const& allowed_types,
      std::string const& error_msg);
  void CheckArgReferenceType(int arg_num,
                             std::vector<PqlRefType> const& allowed_types,
                             std::string const& error_msg);

 public:
  std::unique_ptr<ClauseResult> Evaluate(PKBQPSInterface& pkb) override;
  virtual ~SuchThatClause();
};
