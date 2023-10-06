#include "../../spa/src/query_processing_system/common/PqlDeclaration.h"
#include "../../spa/src/query_processing_system/evaluator/RelationalTable.h"
#include "../../spa/src/query_processing_system/evaluator/ArrayUtility.h"
#include "catch.hpp"


TEST_CASE("RelationalTable Tests") {
  // Declarations
  auto a = PqlDeclaration("a", EntityType::ASSIGN);
  auto b = PqlDeclaration("b", EntityType::PRINT);
  auto c = PqlDeclaration("constant", EntityType::CONSTANT);
  auto v = PqlDeclaration("variable", EntityType::VARIABLE);
  auto s = PqlDeclaration("stmt", EntityType::STMT);
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
    auto values = table.getTableCols({a});
    REQUIRE(ArrayUtility::flattenVector(values) == NUM_VEC);
    REQUIRE(table.getTableColNames() == std::vector<PqlDeclaration>({a}));

    // two decl constructor
    table = RelationalTable(a, s, NUM_VEC, WORD_VEC);
    REQUIRE(table.getNumCols() == 2);
    REQUIRE_FALSE(table.hasNoResults());
    auto a_values = table.getTableCols({a});
    auto s_values = table.getTableCols({s});
    REQUIRE(ArrayUtility::flattenVector(a_values) == NUM_VEC);
    REQUIRE(ArrayUtility::flattenVector(s_values) == WORD_VEC);
    REQUIRE_THAT(table.getTableColNames(),
                 Catch::UnorderedEquals(std::vector<PqlDeclaration>({a, s})));

    // empty lists
    table = RelationalTable(a, v, EMPTY_VEC, EMPTY_VEC);
    REQUIRE(table.getNumCols() == 2);
    REQUIRE(table.hasNoResults());
    REQUIRE(table.getTableCols({a}).empty());
    REQUIRE(table.getTableCols({v}).empty());
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
    auto s_values = table_1.getTableCols({s});
    REQUIRE_THAT(ArrayUtility::flattenVector(s_values), Catch::UnorderedEquals(S_VEC_1));

    auto table_2 = RelationalTable(a, b, A_VEC_2, B_VEC_2);
    auto table_2b = RelationalTable(a, c, A_VEC_2, C_VEC_2);
    auto table_2c = RelationalTable(b, s, B_VEC_2, S_VEC_2);

    table_2.join(table_2b);
    table_2.join(table_2c);


    REQUIRE(table_2.getNumCols() == 4);
    REQUIRE_FALSE(table_2.hasNoResults());
    auto b_values = table_2.getTableCols({b});
    REQUIRE_THAT(ArrayUtility::flattenVector(b_values), Catch::UnorderedEquals(B_VEC_2));

    table_1.join(table_2);


    REQUIRE(table_1.getNumCols() == 5);
    REQUIRE_FALSE(table_1.hasNoResults());
    auto a_values = table_1.getTableCols({a});
    REQUIRE_THAT(ArrayUtility::flattenVector(a_values), Catch::UnorderedEquals(A_VEC_OUT));
    b_values = table_1.getTableCols({b});
    REQUIRE_THAT(ArrayUtility::flattenVector(b_values), Catch::UnorderedEquals(B_VEC_OUT));
    auto c_values = table_1.getTableCols({c});
    REQUIRE_THAT(ArrayUtility::flattenVector(c_values), Catch::UnorderedEquals(C_VEC_OUT));
    auto v_values = table_1.getTableCols({v});
    REQUIRE_THAT(ArrayUtility::flattenVector(v_values), Catch::UnorderedEquals(V_VEC_OUT));
    s_values = table_1.getTableCols({s});
    REQUIRE_THAT(ArrayUtility::flattenVector(s_values), Catch::UnorderedEquals(S_VEC_OUT));

    // test join when result is empty table
    table_1 = RelationalTable(c, v, C_VEC_1, V_VEC_1);
    table_1b = RelationalTable(v, s, C_VEC_1, S_VEC_1);

    table_1.join(table_1b);
    REQUIRE(table_1.getNumCols() == 3);
    REQUIRE(table_1.hasNoResults());
    REQUIRE(table_1.getTableCols({c}).empty());
    REQUIRE(table_1.getTableCols({v}).empty());
    REQUIRE(table_1.getTableCols({s}).empty());
  }

  SECTION ("Test getTableCols") {
    std::vector<std::string> VEC_1 = {"1","2","3"};
    std::vector<std::string> VEC_2 = {"4","5","6"};
    std::vector<std::string> VEC_3 = {"7","8","9"};
    auto table = RelationalTable(a, b, VEC_1, VEC_2);
    auto table_2 = RelationalTable(b, c, VEC_2, VEC_3);
    table.join(table_2, true);

    // Get multiple columns
    std::vector<std::vector<std::string>> expected = {{"4", "1"}, {"5","2"}, {"6","3"}};
    REQUIRE(table.getTableCols({b, a}) == expected);

    // Get columns with repeated values

    // Get columns that are not in table

    // Passing an empty list to the table

    // SHOULD THIS THING DEDUP??? i think so.
  }
}
