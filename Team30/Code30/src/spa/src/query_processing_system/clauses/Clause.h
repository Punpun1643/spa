#pragma once
#include <memory>
#include <string>
#include <unordered_set>

#include "../../program_knowledge_base/PKBQPSInterface.h"
#include "ClauseResult.h"

class Clause {
 private:
  bool is_negated = false;

 public:
  virtual std::unique_ptr<ClauseResult> Evaluate(PKBQPSInterface& pkb) = 0;
  virtual std::unique_ptr<ClauseResult> EvaluateOnCondition(
      PKBQPSInterface& pkb, std::unordered_set<std::string>& d1_values,
      std::unordered_set<std::string>& d2_values);
  virtual ~Clause();

  void FlagAsNegated();
  bool IsNegated() const;

  virtual bool SupportsConditionalEvaluation() const;
  virtual std::optional<PqlDeclaration> GetFirstDeclaration() const = 0;
  virtual std::optional<PqlDeclaration> GetSecondDeclaration() const = 0;
};
