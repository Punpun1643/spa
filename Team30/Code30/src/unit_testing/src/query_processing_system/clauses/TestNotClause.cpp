#include "../../stub/PkbQpsInterfaceStub.h"
#include "../../stub/ClauseStub.h"

#include "catch.hpp"
#include "query_processing_system/clauses/NotClauseDecorator.h"


TEST_CASE("Test NotClause functionality") {
  // Assumes that ClauseResult is functional
  auto pkb = PkbQpsInterfaceStub();
  auto true_result = std::make_unique<ClauseResult>(true);
  auto false_result = std::make_unique<ClauseResult>(false);

  SECTION("Test boolean result negation") {
    std::unique_ptr<Clause> true_clause = std::make_unique<ClauseStub>(std::move(true_result));
    auto not_clause = NotClauseDecorator(std::move(true_clause));
    auto result = not_clause.Evaluate(pkb);
    REQUIRE(result->IsBooleanResult());
    REQUIRE_FALSE(result->GetBooleanClauseValue());

    std::unique_ptr<Clause> false_clause = std::make_unique<ClauseStub>(std::move(false_result));
    not_clause = NotClauseDecorator(std::move(false_clause));
    result = not_clause.Evaluate(pkb);
    REQUIRE(result->IsBooleanResult());
    REQUIRE(result->GetBooleanClauseValue());
  }
  SECTION("Test single decl negation") {

  }
  SECTION("Test double decl negation") {

  }
}
