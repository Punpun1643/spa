#pragma once

#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

#include "PatternClause.h"

class PatternContainerClause : public PatternClause {
 private:
  std::vector<std::pair<std::string, std::string>> EvaluateDeclRef(
      PKBQPSInterface& pkb) override;
  std::vector<std::string> EvaluateValueRef(PKBQPSInterface& pkb) override;
  std::vector<std::string> EvaluateWildRef(PKBQPSInterface& pkb) override;
  std::vector<std::pair<std::string, std::string>> EvaluateDeclRef(
      PKBQPSInterface& pkb, std::unordered_set<std::string>& decl_1_subset,
      std::unordered_set<std::string>& decl_2_subset) override;

 protected:
  PatternContainerClause(PqlDeclaration container_decl, EntRef ent_ref,
                         EntityType target_type);

 public:
  virtual ~PatternContainerClause();
};
