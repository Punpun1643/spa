#include "../../../../spa/src/query_processing_system/evaluator/RelationalTable.h"
#include "catch.hpp"
#include "query_processing_system/references/PqlDeclaration.h"
#include "shared/ArrayUtility.h"

std::vector<std::pair<std::string, std::string>> MakeVectorOfPairsUtil(
    std::vector<std::string> const& v1, std::vector<std::string> const& v2) {
  std::vector<std::pair<std::string, std::string>> output = {};
  assert(v1.size() == v2.size());
  for (int i = 0; i < v1.size(); i++) {
    output.emplace_back(v1[i], v2[i]);
  }
  return output;
}

TEST_CASE("RelationalTable Tests") {
  // Declarations
  auto a = PqlDeclaration("a", EntityType::ASSIGN);
  auto b = PqlDeclaration("b", EntityType::PRINT);
  auto c = PqlDeclaration("constant", EntityType::CONSTANT);
  auto v = PqlDeclaration("variable", EntityType::VARIABLE);
  auto s = PqlDeclaration("stmt", EntityType::STMT);
  std::vector<std::pair<std::string, std::string>> EMPTY_VEC = {};

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
    auto combined_vec = MakeVectorOfPairsUtil(NUM_VEC, WORD_VEC);
    table = RelationalTable(a, s, combined_vec);
    REQUIRE(table.GetNumCols() == 2);
    REQUIRE_FALSE(table.HasNoResults());
    auto a_values = table.GetTableCols({a});
    auto s_values = table.GetTableCols({s});
    REQUIRE(ArrayUtility::FlattenVector(a_values) == NUM_VEC);
    REQUIRE_THAT(ArrayUtility::FlattenVector(s_values),
                 Catch::UnorderedEquals(WORD_VEC));
    REQUIRE_THAT(table.GetTableColNames(),
                 Catch::UnorderedEquals(std::vector<PqlDeclaration>({a, s})));

    table.Clear();
    REQUIRE(table.GetNumCols() == 0);
    REQUIRE(table.HasNoResults());

    // empty lists
    table = RelationalTable(a, v, EMPTY_VEC);
    REQUIRE(table.GetNumCols() == 2);
    REQUIRE(table.HasNoResults());
    REQUIRE(table.GetTableCols({a}).empty());
    REQUIRE(table.GetTableCols({v}).empty());
  }

  SECTION("Test GetTableCols") {
    std::vector<std::string> VEC_1 = {"1", "2", "3"};
    std::vector<std::string> VEC_2 = {"4", "5", "6"};
    std::vector<std::string> VEC_3 = {"7", "8", "9"};
    auto combined_vec = MakeVectorOfPairsUtil(VEC_1, VEC_2);
    auto table = RelationalTable(a, b, combined_vec);
    combined_vec = MakeVectorOfPairsUtil(VEC_2, VEC_3);
    auto table_2 = RelationalTable(b, c, combined_vec);
    table.Join(table_2);

    // Get multiple columns
    std::vector<std::vector<std::string>> expected = {
        {"4", "1"}, {"5", "2"}, {"6", "3"}};
    REQUIRE(table.GetTableCols({b, a}) == expected);

    // Get columns with repeated values
    expected = {{"4", "4", "1", "1", "1"},
                {"5", "5", "2", "2", "2"},
                {"6", "6", "3", "3", "3"}};
    REQUIRE(table.GetTableCols({b, b, a, a, a}) == expected);

    // Make sure de-duplication is done
    auto table_3 = RelationalTable(v, VEC_3);
    table.Join(table_3, true);
    REQUIRE(table.GetTableCols({b, b, a, a, a}) == expected);
  }

  SECTION("Test cross-product join") {
    std::vector<std::string> VEC_1 = {"1", "2"};
    std::vector<std::string> VEC_2 = {"a", "b"};
    std::vector<std::string> VEC_3 = {"x", "y"};

    std::vector<std::vector<std::string>> EXP_OUTPUT = {
        {"1", "a", "x"}, {"1", "a", "y"}, {"2", "b", "x"}, {"2", "b", "y"}};
    auto combined_vec = MakeVectorOfPairsUtil(VEC_1, VEC_2);
    auto table = RelationalTable(a, b, combined_vec);
    auto table_2 = RelationalTable(c, VEC_3);
    REQUIRE_THROWS(table.Join(table_2));  // disable cross-product by default
    table.Join(table_2, true);
    REQUIRE(table.GetNumCols() == 3);
    REQUIRE_THAT(table.GetTableCols({
                     a,
                     b,
                     c,
                 }),
                 Catch::UnorderedEquals(EXP_OUTPUT));
  }

  SECTION("Test non cross-product join functionality") {
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
        {"1", "10", "2", "3", "a"}, {"1", "10", "4", "5", "a"},
        {"1", "10", "2", "3", "g"}, {"1", "10", "4", "5", "g"},
        {"2", "14", "5", "6", "c"}, {"2", "14", "5", "6", "d"}};

    auto table_1 =
        RelationalTable(c, v, MakeVectorOfPairsUtil(C_VEC_1, V_VEC_1));
    auto table_1b =
        RelationalTable(v, s, MakeVectorOfPairsUtil(V_VEC_1, S_VEC_1));
    table_1.Join(table_1b);

    REQUIRE(table_1.GetNumCols() == 3);
    REQUIRE_FALSE(table_1.HasNoResults());
    auto v_values = table_1.GetTableCols({v});
    REQUIRE_THAT(ArrayUtility::FlattenVector(v_values),
                 Catch::UnorderedEquals(V_VEC_1));

    auto table_2 =
        RelationalTable(a, b, MakeVectorOfPairsUtil(A_VEC_2, B_VEC_2));
    auto table_2b =
        RelationalTable(a, c, MakeVectorOfPairsUtil(A_VEC_2, C_VEC_2));
    auto table_2c =
        RelationalTable(b, s, MakeVectorOfPairsUtil(B_VEC_2, S_VEC_2));

    table_2.Join(table_2b);
    table_2.Join(table_2c);

    REQUIRE(table_2.GetNumCols() == 4);
    REQUIRE_FALSE(table_2.HasNoResults());
    auto b_values = table_2.GetTableCols({b});
    REQUIRE_THAT(ArrayUtility::FlattenVector(b_values),
                 Catch::UnorderedEquals(B_VEC_2));

    table_1.Join(table_2);

    REQUIRE(table_1.GetNumCols() == 5);
    REQUIRE_FALSE(table_1.HasNoResults());
    auto values = table_1.GetTableCols({c, s, a, b, v});
    REQUIRE_THAT(values, Catch::UnorderedEquals(CSABV_OUTPUT));

    // test join when result is empty table
    table_1 = RelationalTable(c, v, MakeVectorOfPairsUtil(C_VEC_1, V_VEC_1));
    table_1b = RelationalTable(v, s, MakeVectorOfPairsUtil(C_VEC_1, S_VEC_1));

    table_1.Join(table_1b);
    REQUIRE(table_1.GetNumCols() == 3);
    REQUIRE(table_1.HasNoResults());
    REQUIRE(table_1.GetTableCols({c}).empty());
    REQUIRE(table_1.GetTableCols({v}).empty());
    REQUIRE(table_1.GetTableCols({s}).empty());
  }

  SECTION("Test Delete") {
    // TABLE
    std::vector<std::string> C_VEC = {"1", "1", "2", "2", "2", "6", "1"};
    std::vector<std::string> S_VEC = {"10", "12", "14", "14", "18", "20", "10"};
    std::vector<std::string> V_VEC = {"a", "b", "c", "d", "e", "f", "g"};

    auto table_1 = RelationalTable(s, v, MakeVectorOfPairsUtil(S_VEC, V_VEC));
    auto table_1b = RelationalTable(c, v, MakeVectorOfPairsUtil(C_VEC, V_VEC));
    table_1.Join(table_1b);

    SECTION("Test single decl delete") {
      // OUTPUT TABLE
      std::vector<std::vector<std::string>> EXP_OUTPUT = {
          {"2", "14", "c"}, {"2", "14", "d"}, {"2", "18", "e"}};

      table_1.Delete(c, {"1", "6", "4", "124"});
      REQUIRE(table_1.GetNumCols() == 3);
      auto values = table_1.GetTableCols({c, s, v});
      REQUIRE_THAT(values, Catch::UnorderedEquals(EXP_OUTPUT));

      // Delete everything
      table_1.Delete(c, {"2", "1", "0"});
      REQUIRE(table_1.GetNumCols() == 3);
      REQUIRE(table_1.HasNoResults());

      REQUIRE_THROWS(table_1.Delete(a, {"a"}));
    }

    SECTION("Test double declaration delete") {
      std::vector<std::vector<std::string>> EXP_OUTPUT = {
          {"2", "14", "c"}, {"2", "14", "d"}, {"1", "12", "b"}};

      std::unordered_set<std::pair<std::string, std::string>, PairHash>
          to_delete;
      to_delete.emplace("1", "10");
      to_delete.emplace("6", "20");
      to_delete.emplace("2", "18");
      to_delete.emplace("1", "11");
      to_delete.emplace("3", "14");
      table_1.Delete(c, s, to_delete);
      REQUIRE(table_1.GetNumCols() == 3);
      auto values = table_1.GetTableCols({c, s, v});
      REQUIRE_THAT(values, Catch::UnorderedEquals(EXP_OUTPUT));

      // Delete everything
      to_delete.emplace("14", "c");
      to_delete.emplace("14", "d");
      to_delete.emplace("12", "b");

      table_1.Delete(s, v, to_delete);
      REQUIRE(table_1.HasNoResults());

      REQUIRE_THROWS(table_1.Delete(v, a, to_delete));
      REQUIRE_THROWS(table_1.Delete(a, c, to_delete));
      REQUIRE_THROWS(table_1.Delete(a, b, to_delete));
    }

    SECTION("Test delete that doesn't do anything") {
      table_1.Delete(s, {"abcd", "efgh"});
      auto v_values = table_1.GetTableCols({v});
      REQUIRE_THAT(ArrayUtility::FlattenVector(v_values),
                   Catch::UnorderedEquals(V_VEC));
      table_1.Delete(
          c, s,
          {std::make_pair("wdqwd", "wedwe"), std::make_pair("wadwddd", "awd")});
      v_values = table_1.GetTableCols({v});
      REQUIRE_THAT(ArrayUtility::FlattenVector(v_values),
                   Catch::UnorderedEquals(V_VEC));
    }
  }
}
