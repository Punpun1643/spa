#include "catch.hpp"
#include "query_processing_system/evaluator/IntermediateResultsTable.h"
#include "../../spa/src/query_processing_system/common/ClauseResult.h"


TEST_CASE("Intermediate Results Table Tests") {
  // Declarations
  auto a = PqlDeclaration(std::make_shared<std::string>("a"), EntityType::ASSIGN);
  auto b = PqlDeclaration(std::make_shared<std::string>("b"), EntityType::PRINT);
  auto c = PqlDeclaration(std::make_shared<std::string>("constant"), EntityType::CONSTANT);
  auto v = PqlDeclaration(std::make_shared<std::string>("variable"), EntityType::VARIABLE);
  auto s = PqlDeclaration(std::make_shared<std::string>("stmt"), EntityType::STMT);

  auto TRUE_CLAUSE = ClauseResult(true);
  auto FALSE_CLAUSE = ClauseResult(false);

  auto LIST = std::vector<std::string>({"1","2","3"});
  auto LIST_PTR = std::make_unique<std::vector<std::string>>(LIST);

  auto SINGLE_CLAUSE = ClauseResult(a, std::move(LIST_PTR));

  auto irt = IntermediateResultsTable();

  SECTION("Test only boolean clauses") {
    irt.addClauseResult(TRUE_CLAUSE);
    irt.addClauseResult(TRUE_CLAUSE);
    irt.addClauseResult(FALSE_CLAUSE);
    REQUIRE(irt.hasNoResults());
    irt.addClauseResult(FALSE_CLAUSE);
    REQUIRE(irt.hasNoResults());
  }

  SECTION("Test boolean + other clause types") {
    irt.addClauseResult(SINGLE_CLAUSE);
    REQUIRE_FALSE(irt.hasNoResults());
    irt.addClauseResult(TRUE_CLAUSE);
    REQUIRE_FALSE(irt.hasNoResults());
    irt.addClauseResult(FALSE_CLAUSE);
    REQUIRE(irt.hasNoResults());
    irt.addClauseResult(SINGLE_CLAUSE);
    REQUIRE(irt.hasNoResults());
  }

}