#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

#include "../../spa/src/program_knowledge_base/utils/UFDS.h"
#include "catch.hpp"

TEST_CASE("Basic checks") {
  UFDS ufds = UFDS();

  REQUIRE(ufds.isSameSet(1, 2) == false);
  REQUIRE(ufds.isSameSet(2, 7) == false);

  REQUIRE(ufds.findSet(7) == 7);
  REQUIRE(ufds.findSet(4) == 4);
  REQUIRE(ufds.sizeOf(3) == 1);
}

TEST_CASE("Check transitive property") {
  UFDS ufds = UFDS();

  ufds.unionSets(1, 2);
  ufds.unionSets(3, 4);

  REQUIRE(ufds.isSameSet(1, 2));
  REQUIRE(ufds.sizeOf(3) == 2);

  ufds.unionSets(1, 4);

  REQUIRE(ufds.isSameSet(1, 3));
  REQUIRE(ufds.isSameSet(2, 4));
  REQUIRE(ufds.sizeOf(1) == 4);
}

TEST_CASE("Check correct dynamic UFDS expansion") {
  UFDS ufds = UFDS();

  ufds.unionSets(1, 2);
  ufds.unionSets(2, 7);

  REQUIRE(ufds.isSameSet(1, 7));
  REQUIRE(ufds.sizeOf(5) == 1);
  REQUIRE(ufds.sizeOf(7) == 3);
  REQUIRE(ufds.sizeOf(11) == 1);
}