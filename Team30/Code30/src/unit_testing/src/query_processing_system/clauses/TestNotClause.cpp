#include "../../stub/PkbQpsInterfaceStub.h"
#include "../../stub/ClauseStub.h"

#include "catch.hpp"
#include "query_processing_system/clauses/NotClauseDecorator.h"


TEST_CASE("Test NotClause functionality") {
  // Assumes that ClauseResult is functional
  auto pkb = PkbQpsInterfaceStub();

  auto a = PqlDeclaration("a", EntityType::ASSIGN);
  auto constant = PqlDeclaration("con", EntityType::CONSTANT);

  SECTION("Test boolean result negation") {
    auto true_result = std::make_unique<ClauseResult>(true);
    std::unique_ptr<Clause> true_clause = std::make_unique<ClauseStub>(std::move(true_result));
    auto not_clause = NotClauseDecorator(std::move(true_clause));
    auto result = not_clause.Evaluate(pkb);
    REQUIRE(result->IsBooleanResult());
    REQUIRE_FALSE(result->GetBooleanClauseValue());

    auto false_result = std::make_unique<ClauseResult>(false);
    std::unique_ptr<Clause> false_clause = std::make_unique<ClauseStub>(std::move(false_result));
    not_clause = NotClauseDecorator(std::move(false_clause));
    result = not_clause.Evaluate(pkb);
    REQUIRE(result->IsBooleanResult());
    REQUIRE(result->GetBooleanClauseValue());
  }

  SECTION("Test single decl negation") {
    std::vector<std::string> all_values = {"12", "16", "5", "2", "9", "402"};
    std::vector<std::string> value_subset_1 = {"2", "16", "5"};
    std::vector<std::string> value_subset_2 = {"12", "9", "402"};

    pkb.get_all_of_type_values = all_values;

    auto clause_result = std::make_unique<ClauseResult>(constant, value_subset_1);
    std::unique_ptr<Clause> clause = std::make_unique<ClauseStub>(std::move(clause_result));
    auto not_clause = NotClauseDecorator(std::move(clause));
    auto result = not_clause.Evaluate(pkb);
    REQUIRE_FALSE(result->IsBooleanResult());
    REQUIRE(result->GetNumDeclarations() == 1);
    REQUIRE(result->GetDeclarations() == std::vector<PqlDeclaration>{constant});
    REQUIRE_THAT(result->GetValues(constant), Catch::UnorderedEquals(value_subset_2));

    clause_result = std::make_unique<ClauseResult>(a, value_subset_2);
    clause = std::make_unique<ClauseStub>(std::move(clause_result));
    not_clause = NotClauseDecorator(std::move(clause));
    result = not_clause.Evaluate(pkb);
    REQUIRE(result->GetDeclarations() == std::vector<PqlDeclaration>{a});
    REQUIRE_THAT(result->GetValues(a), Catch::UnorderedEquals(value_subset_1));
  }

  SECTION("Test double decl negation") {

  }
}
