#pragma once
#include <memory>
#include <vector>
#include <utility>
#include <string>
#include <unordered_set>

#include "Clause.h"
#include "program_knowledge_base/utils/PairHash.h"

class NotClauseDecorator : public Clause {
 private:
  std::unique_ptr<Clause> clause_to_negate;
  static std::vector<std::string> NegateSingleDeclValues(
      PKBQPSInterface& pkb, PqlDeclaration const& decl,
      std::unordered_set<std::string> const& value_set);
  static std::vector<std::pair<std::string, std::string>>
  NegateDoubleDeclValues(PKBQPSInterface& pkb, PqlDeclaration const& decl_1,
                         PqlDeclaration const& decl_2,
                         std::unordered_set<std::pair<std::string, std::string>,
                                            PairHash> const& value_set);

 public:
  explicit NotClauseDecorator(std::unique_ptr<Clause> clause_to_negate);
  std::unique_ptr<ClauseResult> Evaluate(PKBQPSInterface& pkb) override;
};
