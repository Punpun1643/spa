#include "../../stub/PkbQpsInterfaceStub.h"
#include "catch.hpp"
#include "query_processing_system/clauses/SelectAllClause.h"

TEST_CASE("Test SelectAll Clause") {
  PkbQpsInterfaceStub pkb = PkbQpsInterfaceStub();

  PqlDeclaration a = PqlDeclaration("assign", EntityType::ASSIGN);
  PqlDeclaration c = PqlDeclaration("const", EntityType::CONSTANT);

  SelectAllClause select_all = SelectAllClause(a);
  auto result = select_all.Evaluate(pkb);
  REQUIRE(pkb.last_entity_type_passed == a.GetEntityType());
  REQUIRE(result->GetNumDeclarations() == 1);
  REQUIRE(result->GetDeclarations() == std::vector<PqlDeclaration>{a});
  REQUIRE(result->GetSingleResultValues() == pkb.get_all_of_type_values);

  // Try a different example to be sure
  auto select_all_2 = SelectAllClause(c);
  result = select_all_2.Evaluate(pkb);
  REQUIRE(pkb.last_entity_type_passed == c.GetEntityType());
  REQUIRE(result->GetDeclarations() == std::vector<PqlDeclaration>{c});
  REQUIRE(result->GetSingleResultValues() == pkb.get_all_of_type_values);
}
