#include "../../spa/src/query_processing_system/common/PqlDeclaration.h"
#include "../../spa/src/query_processing_system/evaluator/RelationalTable.h"
#include "catch.hpp"

TEST_CASE("RelationalTable Tests") {
  // Declarations
  auto a =
      PqlDeclaration(std::make_shared<std::string>("a"), EntityType::ASSIGN);
  auto b =
      PqlDeclaration(std::make_shared<std::string>("b"), EntityType::PRINT);
  auto c = PqlDeclaration(std::make_shared<std::string>("constant"),
                          EntityType::CONSTANT);
  auto v = PqlDeclaration(std::make_shared<std::string>("variable"),
                          EntityType::VARIABLE);
  auto s =
      PqlDeclaration(std::make_shared<std::string>("stmt"), EntityType::STMT);
  std::vector<std::string> EMPTY_VEC = {};

  SECTION("Test basic functionality") {
    std::vector<std::string> NUM_VEC = {"1", "2", "6", "7", "8", "9"};
    std::vector<std::string> WORD_VEC = {"cat",      "dog",   "fox",
                                         "elephant", "tiger", "owl"};
    assert(NUM_VEC.size() == WORD_VEC.size());

    // one decl constructor
    auto table = RelationalTable(a, NUM_VEC);
    REQUIRE(table.getNumCols() == 1);
    REQUIRE_FALSE(table.hasNoResults());
    REQUIRE(table.getTableCol(a) == NUM_VEC);
    REQUIRE(table.getTableColNames() == std::vector<PqlDeclaration>({a}));

    // two decl constructor
    table = RelationalTable(a, s, NUM_VEC, WORD_VEC);
    REQUIRE(table.getNumCols() == 2);
    REQUIRE_FALSE(table.hasNoResults());
    REQUIRE(table.getTableCol(a) == NUM_VEC);
    REQUIRE(table.getTableCol(s) == WORD_VEC);
    REQUIRE_THAT(table.getTableColNames(),
                 Catch::UnorderedEquals(std::vector<PqlDeclaration>({a, s})));

    // empty lists
    table = RelationalTable(a, v, EMPTY_VEC, EMPTY_VEC);
    REQUIRE(table.getNumCols() == 2);
    REQUIRE(table.hasNoResults());
    REQUIRE(table.getTableCol(a).empty());
    REQUIRE(table.getTableCol(v).empty());
  }

  SECTION("Test join functionality") {
    // TABLE 1
    std::vector<std::string> C_VEC_1 = {"1", "1", "2", "2", "2", "6", "1"};
    std::vector<std::string> S_VEC_1 = {"10", "12", "14", "14",
                                        "18", "20", "10"};
    std::vector<std::string> V_VEC_1 = {"a", "b", "c", "d", "e", "f", "g"};

    // TABLE 2
    std::vector<std::string> C_VEC_2 = {"5", "1", "1", "1", "2", "15"};
    std::vector<std::string> S_VEC_2 = {"12", "10", "19", "10", "14", "4"};
    std::vector<std::string> A_VEC_2 = {"1", "2", "3", "4", "5", "6"};
    std::vector<std::string> B_VEC_2 = {"2", "3", "4", "5", "6", "7"};

    // OUTPUT TABLE
    std::vector<std::string> C_VEC_OUT = {"1", "1", "1", "1", "2", "2"};
    std::vector<std::string> S_VEC_OUT = {"10", "10", "10", "10", "14", "14"};
    std::vector<std::string> A_VEC_OUT = {"2", "4", "2", "4", "5", "5"};
    std::vector<std::string> B_VEC_OUT = {"3", "5", "3", "5", "6", "6"};
    std::vector<std::string> V_VEC_OUT = {"a", "a", "g", "g", "c", "d"};

    auto table_1 = RelationalTable(c, v, C_VEC_1, V_VEC_1);
    auto table_1b = RelationalTable(v, s, V_VEC_1, S_VEC_1);
    table_1.join(table_1b);

    REQUIRE(table_1.getNumCols() == 3);
    REQUIRE_FALSE(table_1.hasNoResults());
    REQUIRE_THAT(table_1.getTableCol(s), Catch::UnorderedEquals(S_VEC_1));

    auto table_2 = RelationalTable(a, b, A_VEC_2, B_VEC_2);
    auto table_2b = RelationalTable(a, c, A_VEC_2, C_VEC_2);
    auto table_2c = RelationalTable(b, s, B_VEC_2, S_VEC_2);

    table_2.join(table_2b);
    table_2.join(table_2c);

    REQUIRE(table_2.getNumCols() == 4);
    REQUIRE_FALSE(table_2.hasNoResults());
    REQUIRE_THAT(table_2.getTableCol(b), Catch::UnorderedEquals(B_VEC_2));

    table_1.join(table_2);

    REQUIRE(table_1.getNumCols() == 5);
    REQUIRE_FALSE(table_1.hasNoResults());
    REQUIRE_THAT(table_1.getTableCol(a), Catch::UnorderedEquals(A_VEC_OUT));
    REQUIRE_THAT(table_1.getTableCol(b), Catch::UnorderedEquals(B_VEC_OUT));
    REQUIRE_THAT(table_1.getTableCol(c), Catch::UnorderedEquals(C_VEC_OUT));
    REQUIRE_THAT(table_1.getTableCol(v), Catch::UnorderedEquals(V_VEC_OUT));
    REQUIRE_THAT(table_1.getTableCol(s), Catch::UnorderedEquals(S_VEC_OUT));

    // test join when result is empty table
    table_1 = RelationalTable(c, v, C_VEC_1, V_VEC_1);
    table_1b = RelationalTable(v, s, C_VEC_1, S_VEC_1);

    table_1.join(table_1b);
    REQUIRE(table_1.getNumCols() == 3);
    REQUIRE(table_1.hasNoResults());
    REQUIRE(table_1.getTableCol(c).empty());
    REQUIRE(table_1.getTableCol(v).empty());
    REQUIRE(table_1.getTableCol(s).empty());
  }
}
