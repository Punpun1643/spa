#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Clause.h"
#include "ConditionalClause.h"
#include "query_processing_system/references/PqlReference.h"
#include "shared/types/RelationType.h"

class SuchThatClause : public ConditionalClause {
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
  virtual std::unique_ptr<ClauseResult> EvaluateDeclarationDeclaration(
      PKBQPSInterface& pkb,
      std::unordered_set<std::string> const& decl_1_subset,
      std::unordered_set<std::string> const& decl_2_subset);

  void CheckDeclarationArgEntityType(
      int arg_num, std::vector<EntityType> const& allowed_types,
      std::string const& error_msg);
  void CheckArgReferenceType(int arg_num,
                             std::vector<PqlRefType> const& allowed_types,
                             std::string const& error_msg);

 public:
  std::unique_ptr<ClauseResult> Evaluate(PKBQPSInterface& pkb) override;
  std::unique_ptr<ClauseResult> EvaluateOnCondition(PKBQPSInterface& pkb,
                                                            std::unordered_set<std::string> const& decl_1_subset,
                                                            std::unordered_set<std::string> const& decl_2_subset) override;
  int GetNumDeclarations() const override;
  std::vector<PqlDeclaration> GetDeclarations() const override;
  virtual ~SuchThatClause();
};
