#include "../../../../spa/src/query_processing_system/common/PqlDeclaration.h"
#include "../../../../spa/src/query_processing_system/evaluator/ArrayUtility.h"
#include "../../../../spa/src/query_processing_system/evaluator/RelationalTable.h"
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
    REQUIRE(table.GetNumCols() == 1);
    REQUIRE_FALSE(table.HasNoResults());
    auto values = table.GetTableCols({a});
    REQUIRE(ArrayUtility::FlattenVector(values) == NUM_VEC);
    REQUIRE(table.GetTableColNames() == std::vector<PqlDeclaration>({a}));

    // two decl constructor
    table = RelationalTable(a, s, NUM_VEC, WORD_VEC);
    REQUIRE(table.GetNumCols() == 2);
    REQUIRE_FALSE(table.HasNoResults());
    auto a_values = table.GetTableCols({a});
    auto s_values = table.GetTableCols({s});
    REQUIRE(ArrayUtility::FlattenVector(a_values) == NUM_VEC);
    REQUIRE_THAT(ArrayUtility::FlattenVector(s_values), Catch::UnorderedEquals(WORD_VEC));
    REQUIRE_THAT(table.GetTableColNames(),
                 Catch::UnorderedEquals(std::vector<PqlDeclaration>({a, s})));

    // empty lists
    table = RelationalTable(a, v, EMPTY_VEC, EMPTY_VEC);
    REQUIRE(table.GetNumCols() == 2);
    REQUIRE(table.HasNoResults());
    REQUIRE(table.GetTableCols({a}).empty());
    REQUIRE(table.GetTableCols({v}).empty());
  }

  SECTION ("Test GetTableCols") {
    std::vector<std::string> VEC_1 = {"1","2","3"};
    std::vector<std::string> VEC_2 = {"4","5","6"};
    std::vector<std::string> VEC_3 = {"7","8","9"};
    auto table = RelationalTable(a, b, VEC_1, VEC_2);
    auto table_2 = RelationalTable(b, c, VEC_2, VEC_3);
    table.Join(table_2);

    // Get multiple columns
    std::vector<std::vector<std::string>> expected = {{"4", "1"}, {"5","2"}, {"6","3"}};
    REQUIRE(table.GetTableCols({b, a}) == expected);

    // Get columns with repeated values
    expected = {{"4", "4", "1", "1", "1"}, {"5","5","2","2","2"}, {"6","6","3","3","3"}};
    REQUIRE(table.GetTableCols({b, b, a, a, a}) == expected);

    // Make sure de-duplication is done
    auto table_3 = RelationalTable(v, VEC_3);
    table.Join(table_3, true);
    REQUIRE(table.GetTableCols({b, b, a, a, a}) == expected);
  }

  SECTION("Test Join functionality") {
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
    std::vector<std::vector<std::string>> CSABV_OUTPUT = {
        {"1", "10", "2", "3", "a"},
        {"1", "10", "4", "5", "a"},
        {"1", "10", "2", "3", "g"},
        {"1", "10", "4", "5" ,"g"},
        {"2", "14", "5", "6" ,"c"},
        {"2", "14", "5", "6" ,"d"}
    };

    auto table_1 = RelationalTable(c, v, C_VEC_1, V_VEC_1);
    auto table_1b = RelationalTable(v, s, V_VEC_1, S_VEC_1);
    table_1.Join(table_1b);

    REQUIRE(table_1.GetNumCols() == 3);
    REQUIRE_FALSE(table_1.HasNoResults());
    auto v_values = table_1.GetTableCols({v});
    REQUIRE_THAT(ArrayUtility::FlattenVector(v_values), Catch::UnorderedEquals(V_VEC_1));

    auto table_2 = RelationalTable(a, b, A_VEC_2, B_VEC_2);
    auto table_2b = RelationalTable(a, c, A_VEC_2, C_VEC_2);
    auto table_2c = RelationalTable(b, s, B_VEC_2, S_VEC_2);

    table_2.Join(table_2b);
    table_2.Join(table_2c);

    REQUIRE(table_2.GetNumCols() == 4);
    REQUIRE_FALSE(table_2.HasNoResults());
    auto b_values = table_2.GetTableCols({b});
    REQUIRE_THAT(ArrayUtility::FlattenVector(b_values), Catch::UnorderedEquals(B_VEC_2));

    table_1.Join(table_2);

    REQUIRE(table_1.GetNumCols() == 5);
    REQUIRE_FALSE(table_1.HasNoResults());
    auto values = table_1.GetTableCols({c, s, a, b, v});
    REQUIRE_THAT(values, Catch::UnorderedEquals(CSABV_OUTPUT));

    // test join when result is empty table
    table_1 = RelationalTable(c, v, C_VEC_1, V_VEC_1);
    table_1b = RelationalTable(v, s, C_VEC_1, S_VEC_1);

    table_1.Join(table_1b);
    REQUIRE(table_1.GetNumCols() == 3);
    REQUIRE(table_1.HasNoResults());
    REQUIRE(table_1.GetTableCols({c}).empty());
    REQUIRE(table_1.GetTableCols({v}).empty());
    REQUIRE(table_1.GetTableCols({s}).empty());
  }
}
