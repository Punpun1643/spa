#include "query_processing_system/common/SelectAllClause.h"
#include "../../stub/PkbQpsInterfaceStub.h"
#include "catch.hpp"

TEST_CASE("Test SelectAll Clause") {
  PkbQpsInterfaceStub pkb = PkbQpsInterfaceStub();

  PqlDeclaration a = PqlDeclaration("assign", EntityType::ASSIGN);
  PqlDeclaration c = PqlDeclaration("const", EntityType::CONSTANT);

  SelectAllClause select_all = SelectAllClause(a);
  auto result = select_all.evaluate(pkb);
  REQUIRE(pkb.last_entity_type_passed == a.getEntityType());
  REQUIRE(result->getNumDeclarations() == 1);
  REQUIRE(result->getDeclarations() == std::vector<PqlDeclaration>{a});
  REQUIRE(*(result->getValues(a)) == pkb.getAllOfTypeValues);

  // Try a different example to be sure
  auto select_all_2 = SelectAllClause(c);
  result = select_all_2.evaluate(pkb);
  REQUIRE(pkb.last_entity_type_passed == c.getEntityType());
  REQUIRE(result->getDeclarations() == std::vector<PqlDeclaration>{c});
  REQUIRE(*(result->getValues(c)) == pkb.getAllOfTypeValues);
}