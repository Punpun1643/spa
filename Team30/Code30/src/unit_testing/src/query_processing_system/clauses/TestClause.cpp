#include "../../stub/ClauseStub.h"
#include "catch.hpp"
#include "query_processing_system/clauses/Clause.h"

TEST_CASE("test clause negation") {
  std::shared_ptr<Clause> clause =
      std::make_shared<ClauseStub>(std::make_unique<ClauseResult>(true));
  REQUIRE_FALSE(clause->IsNegated());
  clause->FlagAsNegated();
  REQUIRE(clause->IsNegated());
}
