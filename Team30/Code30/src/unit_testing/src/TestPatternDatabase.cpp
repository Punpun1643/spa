#include "../../spa/src/program_knowledge_base/PatternDatabase.h"
#include "catch.hpp"

TEST_CASE("Pattern Database insertion and retrieval") {
  PatternDatabase db = PatternDatabase();

  /*
   Line 3: x = a + b + 1;
   Line 4: y = b + c;
   Line 5: x = y + x;
  */

  db.insert("3", "x", std::unordered_set<std::string>({"a", "b", "1"}));
  db.insert("4", "y", std::unordered_set<std::string>({"b", "c"}));
  db.insert("5", "x", std::unordered_set<std::string>({"y", "x"}));

  REQUIRE(db.getStatementNumbersGivenLHS("x") ==
          std::unordered_set<std::string>({"3", "5"}));
  REQUIRE(db.getStatementNumbersGivenLHS("y") ==
          std::unordered_set<std::string>({"4"}));
  REQUIRE(db.getStatementNumbersGivenRHS("a") ==
          std::unordered_set<std::string>({"3"}));
  REQUIRE(db.getStatementNumbersGivenRHS("b") ==
          std::unordered_set<std::string>({"3", "4"}));
  REQUIRE(db.getStatementNumbersGivenRHS("c") ==
          std::unordered_set<std::string>({"4"}));
  REQUIRE(db.getStatementNumbersGivenRHS("x") ==
          std::unordered_set<std::string>({"5"}));
  REQUIRE(db.getStatementNumbersGivenRHS("y") ==
          std::unordered_set<std::string>({"5"}));
  REQUIRE(db.getStatementNumbersGivenRHS("1") ==
          std::unordered_set<std::string>({"3"}));

  REQUIRE(db.getVarGivenStatementNum("3") == "x");
  REQUIRE(db.getVarGivenStatementNum("4") == "y");
  REQUIRE(db.getVarGivenStatementNum("5") == "x");
}
