#pragma once

#include "PatternClause.h"

class PatternContainerClause : public PatternClause {
 private:
  EntityType target_type;
  std::vector<std::pair<std::string, std::string>> EvaluateDeclRef(
      PKBQPSInterface& pkb) override;
  std::vector<std::string> EvaluateValueRef(PKBQPSInterface& pkb) override;
  std::vector<std::string> EvaluateWildRef(PKBQPSInterface& pkb) override;

 protected:
  PatternContainerClause(PqlDeclaration container_decl, EntRef ent_ref,
                         EntityType target_type);

 public:
  virtual ~PatternContainerClause();
};
