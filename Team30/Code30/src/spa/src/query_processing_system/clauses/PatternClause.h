#pragma once

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "Clause.h"
#include "query_processing_system/references/EntRef.h"

class PatternClause : public Clause {
 private:
  PqlDeclaration decl;

 protected:
  EntRef ent_ref;
  EntityType target_type;

  PatternClause(PqlDeclaration decl, EntRef ent_ref, EntityType target_type);
  virtual std::vector<std::pair<std::string, std::string>> EvaluateDeclRef(
      PKBQPSInterface& pkb) = 0;
  virtual std::vector<std::string> EvaluateValueRef(PKBQPSInterface& pkb) = 0;
  virtual std::vector<std::string> EvaluateWildRef(PKBQPSInterface& pkb) = 0;
  virtual std::vector<std::pair<std::string, std::string>> EvaluateDeclRef(
      PKBQPSInterface& pkb, std::unordered_set<std::string>& decl_1_subset,
      std::unordered_set<std::string>& decl_2_subset) = 0;

 public:
  virtual ~PatternClause();
  std::unique_ptr<ClauseResult> Evaluate(PKBQPSInterface& pkb) override;
  std::unique_ptr<ClauseResult> EvaluateOnCondition(
      PKBQPSInterface& pkb, std::unordered_set<std::string>& decl_1_subset,
      std::unordered_set<std::string>& decl_2_subset) override;
  bool SupportsConditionalEvaluation() const override;
  std::optional<PqlDeclaration> GetFirstDeclaration() const override;
  std::optional<PqlDeclaration> GetSecondDeclaration() const override;
};
