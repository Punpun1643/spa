#include "../../stub/ClauseStub.h"
#include "../../stub/PkbQpsInterfaceStub.h"
#include "catch.hpp"
#include "query_processing_system/clauses/NotClauseDecorator.h"

TEST_CASE("Test NotClause functionality") {
  // Assumes that ClauseResult is functional
  auto pkb = PkbQpsInterfaceStub();

  auto a = PqlDeclaration("a", EntityType::ASSIGN);
  auto constant = PqlDeclaration("con", EntityType::CONSTANT);
  auto v = PqlDeclaration("var", EntityType::VARIABLE);
  auto p = PqlDeclaration("proc", EntityType::PROCEDURE);

  SECTION("Test boolean result negation") {
    auto true_result = std::make_unique<ClauseResult>(true);
    std::shared_ptr<Clause> true_clause =
        std::make_shared<ClauseStub>(std::move(true_result));
    auto not_clause = NotClauseDecorator(std::move(true_clause));
    auto result = not_clause.Evaluate(pkb);
    REQUIRE(result->IsBooleanResult());
    REQUIRE_FALSE(result->GetBooleanClauseValue());

    auto false_result = std::make_unique<ClauseResult>(false);
    std::shared_ptr<Clause> false_clause =
        std::make_shared<ClauseStub>(std::move(false_result));
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

    auto clause_result =
        std::make_unique<ClauseResult>(constant, value_subset_1);
    std::shared_ptr<Clause> clause =
        std::make_shared<ClauseStub>(std::move(clause_result));
    auto not_clause = NotClauseDecorator(std::move(clause));
    auto result = not_clause.Evaluate(pkb);
    REQUIRE_FALSE(result->IsBooleanResult());
    REQUIRE(result->GetNumDeclarations() == 1);
    REQUIRE(result->GetDeclarations() == std::vector<PqlDeclaration>{constant});
    REQUIRE_THAT(result->GetValues(constant),
                 Catch::UnorderedEquals(value_subset_2));

    clause_result = std::make_unique<ClauseResult>(a, value_subset_2);
    clause = std::make_shared<ClauseStub>(std::move(clause_result));
    not_clause = NotClauseDecorator(std::move(clause));
    result = not_clause.Evaluate(pkb);
    REQUIRE(result->GetDeclarations() == std::vector<PqlDeclaration>{a});
    REQUIRE_THAT(result->GetValues(a), Catch::UnorderedEquals(value_subset_1));
  }

  SECTION("Test double decl negation") {
    std::vector<std::string> all_values = {"cat", "dog", "fox"};
    std::vector<std::pair<std::string, std::string>> value_subset_1 = {
        std::make_pair("cat", "dog"), std::make_pair("fox", "dog"),
        std::make_pair("fox", "cat"), std::make_pair("fox", "fox")};
    std::vector<std::pair<std::string, std::string>> value_subset_2 = {
        std::make_pair("cat", "cat"), std::make_pair("cat", "fox"),
        std::make_pair("dog", "cat"), std::make_pair("dog", "fox"),
        std::make_pair("dog", "dog")};

    pkb.get_all_of_type_values = all_values;
    auto clause_result =
        std::make_unique<ClauseResult>(constant, p, value_subset_1);
    std::shared_ptr<Clause> clause =
        std::make_shared<ClauseStub>(std::move(clause_result));
    auto not_clause = NotClauseDecorator(std::move(clause));
    auto result = not_clause.Evaluate(pkb);
    REQUIRE(result->GetNumDeclarations() == 2);
    REQUIRE_THAT(
        result->GetDeclarations(),
        Catch::UnorderedEquals(std::vector<PqlDeclaration>{constant, p}));

    std::vector<std::pair<std::string, std::string>> paired_result = {};
    auto decl_values_1 = result->GetValues(constant);
    auto decl_values_2 = result->GetValues(p);
    REQUIRE(decl_values_1.size() == decl_values_2.size());
    for (int i = 0; i < decl_values_1.size(); i++) {
      paired_result.emplace_back(decl_values_1[i], decl_values_2[i]);
    }
    REQUIRE_THAT(value_subset_2, Catch::UnorderedEquals(paired_result));
  }
}
