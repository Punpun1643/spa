#include "../../spa/src/program_knowledge_base/tables/FollowsTable.h"
#include "../../spa/src/program_knowledge_base/tables/ParentsTable.h"
#include "catch.hpp"

TEST_CASE("Parents") {
  ParentsTable pt = ParentsTable();
  pt.insert(1, 4);
  pt.insert(4, 7);

  REQUIRE(pt.isParent(1, 7) == false);
  REQUIRE(pt.isParentS(1, 7));
}

TEST_CASE("Follows") {
  FollowsTable ft = FollowsTable();
  ft.insert(1, 4);
  ft.insert(4, 7);

  REQUIRE(ft.isFollows(1, 7) == false);
  REQUIRE(ft.isFollowsS(1, 7));
}
