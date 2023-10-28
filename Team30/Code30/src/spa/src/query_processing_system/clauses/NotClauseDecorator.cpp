#include "NotClauseDecorator.h"

#include <stdexcept>

#include "SelectAllClause.h"

NotClauseDecorator::NotClauseDecorator(std::unique_ptr<Clause> clause_to_negate)
  : clause_to_negate(std::move(clause_to_negate)) {}

std::vector<std::string> NotClauseDecorator::NegateSingleDeclValues(PKBQPSInterface& pkb, PqlDeclaration const& decl, std::unordered_set<std::string> const& value_set) {
  auto select_all_clause = SelectAllClause(decl);
  std::vector<std::string> all_values = select_all_clause.Evaluate(pkb)->GetValues(decl);

  std::vector<std::string> complement_values;
  for (auto& value: all_values) {
    if (value_set.count(value) == 0) {
      complement_values.push_back(value);
    }
  }
  return complement_values;
}

std::vector<std::pair<std::string, std::string>> NotClauseDecorator::NegateDoubleDeclValues(PKBQPSInterface& pkb,
                                                                    PqlDeclaration const& decl_1,
                                                                    PqlDeclaration const& decl_2,
                                                                    std::unordered_set<std::pair<std::string, std::string>, PairHash> const& value_set) {

}

std::unique_ptr<ClauseResult> NotClauseDecorator::Evaluate(PKBQPSInterface& pkb) {
  auto results = clause_to_negate->Evaluate(pkb);
  switch (results->GetNumDeclarations()) {
    case(0): {
      bool value = results->GetBooleanClauseValue();
      return std::make_unique<ClauseResult>(!value);
    }
    case(1): {
      PqlDeclaration decl = results->GetDeclarations()[0];
      std::vector<std::string> values = results->GetValues(decl);
      std::unordered_set<std::string> value_set(values.begin(), values.end());

      auto complement_values = NegateSingleDeclValues(pkb, decl, value_set);
      return std::make_unique<ClauseResult>(decl, complement_values);
    }
    case(2): {
      PqlDeclaration decl_1 = results->GetDeclarations()[0];
      PqlDeclaration decl_2 = results->GetDeclarations()[1];

      // method to get paired results?
      std::vector<std::string> value_vec_1 = results->GetValues(decl_1);
      std::vector<std::string> value_vec_2 = results->GetValues(decl_2);
      if (value_vec_1.size() != value_vec_2.size()) {
        throw std::logic_error("Value vectors have different lengths.");
      }
      //
      std::unordered_set<std::pair<std::string, std::string>, PairHash> value_set;
      for (int i = 0; i < value_vec_1.size(); i++) {
        value_set.emplace(value_vec_1[i], value_vec_2[i]);
      }
      auto complement_values = NegateDoubleDeclValues(pkb, decl_1, decl_2, value_set);

    }
    default:
      throw std::logic_error("ClauseResult has an unexpected number of declarations.");
  }
}
