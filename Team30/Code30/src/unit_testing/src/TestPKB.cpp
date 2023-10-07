#include "catch.hpp"
#include "program_knowledge_base/PKB.h"
#include "program_knowledge_base/RelationType.h"

TEST_CASE("Follows, Parent, Follows* and Parent*") {
  PKB pkb = PKB();
  pkb.insertEntity(EntityType::PROCEDURE, "main");
  pkb.insertEntity(EntityType::PROCEDURE, "sub");
  pkb.insertEntity(EntityType::CONSTANT, "3");
  pkb.insertEntity(EntityType::VARIABLE, "x");
  pkb.insertEntity(EntityType::VARIABLE, "y");

  pkb.insertEntity(EntityType::CALL, "1");
  pkb.insertEntity(EntityType::CALL, "2");
  pkb.insertEntity(EntityType::PRINT, "3");
  pkb.insertEntity(EntityType::IF, "4");
  pkb.insertEntity(EntityType::READ, "5");
  pkb.insertEntity(EntityType::CALL, "6");
  pkb.insertEntity(EntityType::ASSIGN, "7");

  pkb.insertRelation(RelationType::FOLLOWS, "1", "2");
  pkb.insertRelation(RelationType::FOLLOWS, "2", "3");
  pkb.insertRelation(RelationType::FOLLOWS, "3", "4");
  pkb.insertRelation(RelationType::FOLLOWS, "5", "6");

  pkb.insertRelation(RelationType::PARENT, "4", "5");
  pkb.insertRelation(RelationType::PARENT, "4", "6");
  pkb.insertRelation(RelationType::PARENT, "6", "7");

  // added follows relation
  REQUIRE(pkb.isRelationTrueValueValue("2", "3", RelationType::FOLLOWS));
  // unadded follows relation
  REQUIRE(pkb.isRelationTrueValueValue("5", "8", RelationType::FOLLOWS) ==
          false);
  // follows not transitive
  REQUIRE(pkb.isRelationTrueValueValue("1", "4", RelationType::FOLLOWS) ==
          false);

  // added follows and hence follows_star relation
  REQUIRE(pkb.isRelationTrueValueValue("2", "3", RelationType::FOLLOWS_STAR));
  // unadded follows_star relation
  REQUIRE(pkb.isRelationTrueValueValue("1", "6", RelationType::FOLLOWS_STAR) ==
          false);
  // follows_star should be transitive
  REQUIRE(pkb.isRelationTrueValueValue("1", "3", RelationType::FOLLOWS_STAR));

  // added parent relation
  REQUIRE(pkb.isRelationTrueValueValue("6", "7", RelationType::PARENT));
  // unadded parent relation
  REQUIRE(pkb.isRelationTrueValueValue("3", "5", RelationType::PARENT) ==
          false);
  // parent not transitive
  REQUIRE(pkb.isRelationTrueValueValue("4", "7", RelationType::PARENT) ==
          false);

  // added parent and hence parent_star relation
  REQUIRE(pkb.isRelationTrueValueValue("4", "5", RelationType::PARENT_STAR));
  // unadded follows_star relation
  REQUIRE(pkb.isRelationTrueValueValue("1", "6", RelationType::PARENT_STAR) ==
          false);
  // parent_star should be transitive
  REQUIRE(pkb.isRelationTrueValueValue("4", "7", RelationType::PARENT_STAR));

  // Returns if Relation(int, _)
  // no statements follow 7
  REQUIRE(pkb.isRelationTrueValueWild("7", RelationType::FOLLOWS) == false);
  // statement 6 follows statement 5
  REQUIRE(pkb.isRelationTrueValueWild("5", RelationType::FOLLOWS));
  // statement 6 is the Parent* of 7
  REQUIRE(pkb.isRelationTrueValueWild("6", RelationType::PARENT_STAR));
  // statement 1 is not the parent of any statement
  REQUIRE(pkb.isRelationTrueValueWild("1", RelationType::PARENT) == false);

  // Returns if Relation(_, int)
  // statement 4 follows* statements 1, 2, 3
  REQUIRE(pkb.isRelationTrueWildValue("4", RelationType::FOLLOWS_STAR));
  // statement 1 does not follow any statement
  REQUIRE(pkb.isRelationTrueWildValue("1", RelationType::FOLLOWS) == false);
  // statement 7 has parents
  REQUIRE(pkb.isRelationTrueWildValue("7", RelationType::PARENT_STAR));

  // Returns if Relation(_, _)
  REQUIRE(pkb.isRelationTrueWildWild(RelationType::FOLLOWS));
  REQUIRE(pkb.isRelationTrueWildWild(RelationType::FOLLOWS_STAR));
  REQUIRE(pkb.isRelationTrueWildWild(RelationType::PARENT));
  REQUIRE(pkb.isRelationTrueWildWild(RelationType::PARENT_STAR));

  std::vector<std::string> empty_vector;

  // Returns all s such that Relation(s, _)
  // there are no assignment statements that are followed by any other
  // statements
  REQUIRE(*pkb.getRelationSynonymWild(EntityType::ASSIGN,
                                      RelationType::FOLLOWS) == empty_vector);
  // statement 4 is the only if statement that is a parent
  std::vector<std::string> tmp = {"4"};
  REQUIRE(*pkb.getRelationSynonymWild(EntityType::IF,
                                      RelationType::PARENT_STAR) == tmp);
  // there are no while statements
  REQUIRE(*pkb.getRelationSynonymWild(EntityType::WHILE,
                                      RelationType::PARENT) == empty_vector);

  // Returns all s such that Relation(s, int)
  // statement 3 follows 1 and 2
  tmp = {"1", "2"};
  REQUIRE(*pkb.getRelationSynonymValue(EntityType::STMT, "3",
                                       RelationType::FOLLOWS_STAR) == tmp);
  // statement 4 is the only if statement that is a parent of 7
  tmp = {"4"};
  REQUIRE(*pkb.getRelationSynonymValue(EntityType::IF, "7",
                                       RelationType::PARENT_STAR) == tmp);
  // statement 2 is the only CALL statement that is followed by 3
  tmp = {"2"};
  REQUIRE(*pkb.getRelationSynonymValue(EntityType::CALL, "3",
                                       RelationType::FOLLOWS) == tmp);
  // no call statements followed by 4
  REQUIRE(*pkb.getRelationSynonymValue(EntityType::CALL, "4",
                                       RelationType::FOLLOWS) == empty_vector);

  // Returns all s such that Relation(int, s)
  // 3 and 4 follows* 2
  tmp = {"3", "4"};
  REQUIRE(*pkb.getRelationValueSynonym("2", EntityType::STMT,
                                       RelationType::FOLLOWS_STAR) == tmp);
  // 4 is a parent of 5, 6, 7
  tmp = {"5", "6", "7"};
  //  REQUIRE(*pkb.getRelationValues("4", EntityType::STMT,
  //                                 RelationType::PARENT_STAR) == tmp);
  //// 3 is not a parent
  REQUIRE(*pkb.getRelationValueSynonym("3", EntityType::STMT,
                                       RelationType::PARENT) == empty_vector);

  // Returns all s1, s2 such that Relation(s1, s2)
  std::vector<std::pair<std::string, std::string>> tmp_pair;
  tmp_pair = {{"4", "5"}, {"4", "6"}, {"6", "7"}};
  //  REQUIRE(
  //      *pkb.getRelationValues(EntityType::STMT, EntityType::STMT,
  //                                 RelationType::PARENT) == tmp_pair);
}

TEST_CASE("Follows, Parent, Follows* and Parent* with empty PKB") {
  PKB pkb = PKB();
  REQUIRE(pkb.isRelationTrueWildWild(RelationType::FOLLOWS) == false);
  REQUIRE(pkb.isRelationTrueWildWild(RelationType::PARENT_STAR) == false);
}

// UsesS only holds Uses relations for Statements
// UsesP holds for Procedures
TEST_CASE("Uses and Modifies") {
  PKB pkb = PKB();
  pkb.insertEntity(EntityType::PROCEDURE, "main");
  pkb.insertEntity(EntityType::PROCEDURE, "sub");
  pkb.insertEntity(EntityType::VARIABLE, "x");
  pkb.insertEntity(EntityType::VARIABLE, "y");

  pkb.insertEntity(EntityType::CALL, "1");
  pkb.insertEntity(EntityType::CALL, "2");
  pkb.insertEntity(EntityType::PRINT, "3");
  pkb.insertEntity(EntityType::IF, "4");
  pkb.insertEntity(EntityType::READ, "5");
  pkb.insertEntity(EntityType::CALL, "6");
  pkb.insertEntity(EntityType::ASSIGN, "7");

  pkb.insertRelation(RelationType::USES_S, "3", "x");
  pkb.insertRelation(RelationType::USES_S, "4", "y");
  pkb.insertRelation(RelationType::USES_P, "sub", "x");

  pkb.insertRelation(RelationType::MODIFIES_P, "main", "x");
  pkb.insertRelation(RelationType::MODIFIES_S, "5", "x");
  pkb.insertRelation(RelationType::MODIFIES_S, "7", "y");

  // added USES relation for statement
  REQUIRE(pkb.isRelationTrueValueValue("3", "x", RelationType::USES_S));
  // added USES relation for procedure
  REQUIRE(pkb.isRelationTrueValueValue("sub", "x", RelationType::USES_P));

  // USES relation for statement should not be in procedure table
  REQUIRE(pkb.isRelationTrueValueValue("4", "y", RelationType::USES_P) ==
          false);
  // USES relation for procedure should not be in statement table
  REQUIRE(pkb.isRelationTrueValueValue("sub", "x", RelationType::USES_S) ==
          false);

  // added MODIFIES relation for statement
  REQUIRE(pkb.isRelationTrueValueValue("5", "x", RelationType::MODIFIES_S));
  // added MODIFIES relation for procedure
  REQUIRE(pkb.isRelationTrueValueValue("main", "x", RelationType::MODIFIES_P));

  // MODIFIES relation for statement should not be in procedure table
  REQUIRE(pkb.isRelationTrueValueValue("7", "y", RelationType::MODIFIES_P) ==
          false);
  // MODIFIES relation for procedure should not be in statement table
  REQUIRE(pkb.isRelationTrueValueValue("main", "x", RelationType::MODIFIES_S) ==
          false);

  std::vector<std::string> empty_vector;

  // Check APIs

  std::vector<std::string> tmp = {"main"};
  // Select p such that Modifies(p, _)
  REQUIRE(*pkb.getRelationSynonymWild(EntityType::PROCEDURE,
                                      RelationType::MODIFIES_P) == tmp);
  tmp = {"5", "7"};
  // Select s such that Modifies(s, _)
  REQUIRE(*pkb.getRelationSynonymWild(EntityType::STMT,
                                      RelationType::MODIFIES_S) == tmp);
  // Query from wrong table
  REQUIRE(*pkb.getRelationSynonymWild(EntityType::STMT, RelationType::USES_P) ==
          empty_vector);

  // Select Uses(pn, v)
  // Statement 3 is a print statement that uses "x". Returns that pair.
  std::vector<std::pair<std::string, std::string>> emptyPair = {};
  std::vector<std::pair<std::string, std::string>> tmp1 = {
      std::make_pair("3", "x")};

  REQUIRE(*pkb.getRelationSynonymSynonym(EntityType::PRINT,
                                         EntityType::VARIABLE,
                                         RelationType::USES_S) == tmp1);

  // Modifies(c, v)
  // No procedure call that modifies a variable
  REQUIRE(*pkb.getRelationSynonymSynonym(EntityType::CALL, EntityType::VARIABLE,
                                         RelationType::MODIFIES_S) ==
          emptyPair);
}

TEST_CASE("Assignment Pattern PKB") {
  PKB pkb = PKB();
  /*
 Line 3: x = a + b + 1;
 Line 4: y = b + c;
 Line 5: x = y + x;
*/
  pkb.insertEntity(EntityType::VARIABLE, "a");
  pkb.insertEntity(EntityType::VARIABLE, "b");
  pkb.insertEntity(EntityType::VARIABLE, "c");
  pkb.insertEntity(EntityType::VARIABLE, "x");
  pkb.insertEntity(EntityType::VARIABLE, "y");
  pkb.insertEntity(EntityType::CONSTANT, "1");
  pkb.insertEntity(EntityType::ASSIGN, "3");
  pkb.insertEntity(EntityType::ASSIGN, "4");
  pkb.insertEntity(EntityType::ASSIGN, "5");

  pkb.insertPattern("3", "x", std::unordered_set<std::string>({"a", "b", "1"}));
  pkb.insertPattern("4", "y", std::unordered_set<std::string>({"b", "c"}));
  pkb.insertPattern("5", "x", std::unordered_set<std::string>({"y", "x"}));

  std::vector<std::string> empty_vector = {};

  // pattern a("x", "_a_")
  std::vector<std::string> expected_result = {"3"};
  REQUIRE(*pkb.getPatternMatchesValueLhs("x", "a", MatchType::PARTIAL_MATCH) ==
          expected_result);

  // pattern a("x", _)
  expected_result = {"3", "5"};
  REQUIRE(*pkb.getPatternMatchesValueLhs("x", "", MatchType::WILD_MATCH) ==
          expected_result);

  // pattern a("y", "_a_")
  REQUIRE(*pkb.getPatternMatchesValueLhs("y", "a", MatchType::PARTIAL_MATCH) ==
          empty_vector);

  // pattern a(_, "_b_")
  expected_result = {"3", "4"};
  REQUIRE(*pkb.getPatternMatchesWildLhs("b", MatchType::PARTIAL_MATCH) ==
          expected_result);

  // pattern a(_, "_x_")
  expected_result = {"5"};
  REQUIRE(*pkb.getPatternMatchesWildLhs("x", MatchType::PARTIAL_MATCH) ==
          expected_result);

  // pattern a(_, _)
  expected_result = {"3", "4", "5"};
  REQUIRE(*pkb.getPatternMatchesWildLhs("", MatchType::WILD_MATCH) ==
          expected_result);

  // pattern a(var, "_b_")
  std::vector<std::pair<std::string, std::string>> expected_pairs = {
      {"3", "x"}, {"4", "y"}};
  REQUIRE(*pkb.getPatternMatchesSynonymLhs("b", MatchType::PARTIAL_MATCH) ==
          expected_pairs);

  // pattern a(var, "_")
  expected_pairs = {{"3", "x"}, {"4", "y"}, {"5", "x"}};
  REQUIRE_THAT(*pkb.getPatternMatchesSynonymLhs("", MatchType::WILD_MATCH),
               Catch::UnorderedEquals(expected_pairs));
}

TEST_CASE("PKB test1-source Parent*") {
  PKB pkb = PKB();
  pkb.insertRelation(RelationType::PARENT, "4", "5");
  pkb.insertRelation(RelationType::PARENT, "4", "6");
  pkb.insertRelation(RelationType::PARENT, "4", "7");
  pkb.insertRelation(RelationType::PARENT, "7", "8");
  pkb.insertRelation(RelationType::PARENT, "4", "10");
  pkb.insertRelation(RelationType::PARENT, "14", "15");
  pkb.insertRelation(RelationType::PARENT, "14", "16");
  pkb.insertRelation(RelationType::PARENT, "20", "21");
  pkb.insertRelation(RelationType::PARENT, "20", "22");
  pkb.insertRelation(RelationType::PARENT, "20", "23");
  pkb.insertRelation(RelationType::PARENT, "20", "24");
  pkb.insertRelation(RelationType::PARENT, "24", "25");
  pkb.insertRelation(RelationType::PARENT, "24", "26");

  for (int i = 1; i < 27; i++) {
    REQUIRE(!pkb.isRelationTrueValueValue("22", std::to_string(i),
                                          RelationType::PARENT_STAR));
    REQUIRE(!pkb.isRelationTrueValueValue("22", std::to_string(i),
                                          RelationType::PARENT));
  }
}

TEST_CASE("Test1-Source PKB") {
  PKB pkb = PKB();
  pkb.insertEntity(EntityType::PROCEDURE, "Advanced");
  pkb.insertEntity(EntityType::VARIABLE, "y");
  pkb.insertEntity(EntityType::VARIABLE, "z");
  pkb.insertEntity(EntityType::VARIABLE, "p");
  pkb.insertEntity(EntityType::VARIABLE, "q");
  pkb.insertEntity(EntityType::VARIABLE, "i");
  pkb.insertEntity(EntityType::VARIABLE, "j");
  pkb.insertEntity(EntityType::VARIABLE, "x");
  pkb.insertEntity(EntityType::VARIABLE, "t");
  pkb.insertEntity(EntityType::VARIABLE, "a");
  pkb.insertEntity(EntityType::VARIABLE, "b");
  pkb.insertEntity(EntityType::CONSTANT, "2");
  pkb.insertEntity(EntityType::CONSTANT, "1");
  pkb.insertEntity(EntityType::CONSTANT, "24");
  pkb.insertEntity(EntityType::CONSTANT, "0");
  pkb.insertEntity(EntityType::CONSTANT, "5");
  pkb.insertEntity(EntityType::VARIABLE, "10");
  pkb.insertEntity(EntityType::VARIABLE, "100");

  pkb.insertPattern("2", "y", std::unordered_set<std::string>({"2", "z"}));
  pkb.insertPattern("3", "z", std::unordered_set<std::string>({"p", "y"}));
  pkb.insertPattern("5", "z", std::unordered_set<std::string>({"x", "24"}));
  pkb.insertPattern("8", "y", std::unordered_set<std::string>({"x", "q", "5"}));
  pkb.insertPattern("9", "z", std::unordered_set<std::string>({"z", "1"}));
  pkb.insertPattern("10", "y",
                    std::unordered_set<std::string>({"x", "z", "p", "q"}));
  pkb.insertPattern("11", "i",
                    std::unordered_set<std::string>({"x", "j", "z"}));
  pkb.insertPattern("12", "p", std::unordered_set<std::string>({"x", "10"}));
  pkb.insertPattern("13", "q", std::unordered_set<std::string>({"y", "10"}));
  pkb.insertPattern("15", "a", std::unordered_set<std::string>({"5", "p"}));
  pkb.insertPattern("17", "q", std::unordered_set<std::string>({"p", "5"}));
  pkb.insertPattern("18", "i", std::unordered_set<std::string>({"x", "10"}));
  pkb.insertPattern("19", "j",
                    std::unordered_set<std::string>({"x", "y", "10"}));
  pkb.insertPattern("22", "x", std::unordered_set<std::string>({"1"}));
  pkb.insertPattern("23", "y",
                    std::unordered_set<std::string>({"i", "x", "y", "z"}));
  pkb.insertPattern("25", "x", std::unordered_set<std::string>({"a", "1"}));
  pkb.insertPattern("26", "a", std::unordered_set<std::string>({"b"}));
  pkb.insertPattern("2", "y", std::unordered_set<std::string>({"2", "z"}));

  std::vector<std::string> expected_res = {"22", "25"};
  REQUIRE_THAT(
      *pkb.getPatternMatchesValueLhs("x", "1", MatchType::PARTIAL_MATCH),
      Catch::UnorderedEquals(expected_res));
}

std::shared_ptr<TreeNode> buildTree1() {
  // a + b * c % d;
  TreeNode multiply =
      TreeNode("*", std::make_shared<TreeNode>("b", nullptr, nullptr),
               std::make_shared<TreeNode>("c", nullptr, nullptr));
  TreeNode modulo = TreeNode("%", std::make_shared<TreeNode>(multiply),
                             std::make_shared<TreeNode>("d", nullptr, nullptr));
  TreeNode add =
      TreeNode("%", std::make_shared<TreeNode>("a", nullptr, nullptr),
               std::make_shared<TreeNode>(modulo));
  return std::make_shared<TreeNode>(add);
}

std::shared_ptr<TreeNode> buildTree2() {
  // b * c + d;
  TreeNode multiply =
      TreeNode("*", std::make_shared<TreeNode>("b", nullptr, nullptr),
               std::make_shared<TreeNode>("c", nullptr, nullptr));
  TreeNode add = TreeNode("%", std::make_shared<TreeNode>(multiply),
                          std::make_shared<TreeNode>("d", nullptr, nullptr));
  return std::make_shared<TreeNode>(add);
}

std::shared_ptr<TreeNode> buildTree3() {
  // a + b;
  TreeNode add =
      TreeNode("+", std::make_shared<TreeNode>("a", nullptr, nullptr),
               std::make_shared<TreeNode>("b", nullptr, nullptr));
  return std::make_shared<TreeNode>(add);
}

std::shared_ptr<TreeNode> buildTree4() {
  // b * c;
  TreeNode multiply =
      TreeNode("*", std::make_shared<TreeNode>("b", nullptr, nullptr),
               std::make_shared<TreeNode>("c", nullptr, nullptr));
  return std::make_shared<TreeNode>(multiply);
}

TEST_CASE("Pattern Database Assignment insertion and retrieval") {
  PKB pkb = PKB();

  /*
     Line 3: x = a + b * c % d;
     Line 4: y = b * c + d;
     Line 5: x = a + b;
  */

  pkb.insertEntity(EntityType::ASSIGN, "3");
  pkb.insertEntity(EntityType::ASSIGN, "4");
  pkb.insertEntity(EntityType::ASSIGN, "5");
  pkb.insertEntity(EntityType::VARIABLE, "x");
  pkb.insertEntity(EntityType::VARIABLE, "y");
  pkb.insertEntity(EntityType::VARIABLE, "a");
  pkb.insertEntity(EntityType::VARIABLE, "b");
  pkb.insertEntity(EntityType::VARIABLE, "c");
  pkb.insertEntity(EntityType::VARIABLE, "d");

  pkb.insertPattern(PatternType::ASSIGN, "3", "x", buildTree1());
  pkb.insertPattern(PatternType::ASSIGN, "4", "y", buildTree2());
  pkb.insertPattern(PatternType::ASSIGN, "5", "x", buildTree3());

  std::vector<std::pair<std::string, std::string>> expected_pairs;

  // syn = _ (all assignment statements)
  expected_pairs.push_back({"3", "x"});
  expected_pairs.push_back({"4", "y"});
  expected_pairs.push_back({"5", "x"});
  auto actual_pairs =
      *pkb.getPatternMatchesSynonymLhs(buildTree4(), MatchType::WILD_MATCH);
  std::sort(actual_pairs.begin(), actual_pairs.end());
  REQUIRE(actual_pairs == expected_pairs);

  // syn = _b * c_
  expected_pairs.clear();
  expected_pairs.push_back({"3", "x"});
  expected_pairs.push_back({"4", "y"});
  actual_pairs =
      *pkb.getPatternMatchesSynonymLhs(buildTree4(), MatchType::PARTIAL_MATCH);
  std::sort(actual_pairs.begin(), actual_pairs.end());
  REQUIRE(actual_pairs == expected_pairs);

  // syn = b * c
  expected_pairs.clear();
  actual_pairs =
      *pkb.getPatternMatchesSynonymLhs(buildTree4(), MatchType::EXACT_MATCH);
  std::sort(actual_pairs.begin(), actual_pairs.end());
  REQUIRE(actual_pairs == expected_pairs);

  // syn = a + b
  expected_pairs.push_back({"5", "x"});
  actual_pairs =
      *pkb.getPatternMatchesSynonymLhs(buildTree3(), MatchType::EXACT_MATCH);
  std::sort(actual_pairs.begin(), actual_pairs.end());
  REQUIRE(actual_pairs == expected_pairs);

  // x = _
  std::vector<std::string> expected;
  expected.push_back("3");
  expected.push_back("5");
  auto actual =
      *pkb.getPatternMatchesValueLhs("x", buildTree4(), MatchType::WILD_MATCH);
  std::sort(actual.begin(), actual.end());
  REQUIRE(actual == expected);

  // x = _b * c_
  expected.clear();
  expected.push_back("3");
  actual = *pkb.getPatternMatchesValueLhs("x", buildTree4(),
                                          MatchType::PARTIAL_MATCH);
  std::sort(actual.begin(), actual.end());
  REQUIRE(actual == expected);

  // x = _a + b_
  expected.clear();
  expected.push_back("5");
  actual = *pkb.getPatternMatchesValueLhs("x", buildTree3(),
                                          MatchType::PARTIAL_MATCH);
  std::sort(actual.begin(), actual.end());
  REQUIRE(actual == expected);

  // x = a + b * c % d
  expected.clear();
  expected.push_back("3");
  actual =
      *pkb.getPatternMatchesValueLhs("x", buildTree1(), MatchType::EXACT_MATCH);
  std::sort(actual.begin(), actual.end());
  REQUIRE(actual == expected);

  // y = a + b * c % d;
  expected.clear();
  actual =
      *pkb.getPatternMatchesValueLhs("y", buildTree1(), MatchType::EXACT_MATCH);
  std::sort(actual.begin(), actual.end());
  REQUIRE(actual == expected);

  // _ = _
  expected.clear();
  expected.push_back("3");
  expected.push_back("4");
  expected.push_back("5");
  actual = *pkb.getPatternMatchesWildLhs(buildTree1(), MatchType::WILD_MATCH);
  std::sort(actual.begin(), actual.end());
  REQUIRE(actual == expected);

  // _ = _a_
  expected.clear();
  expected.push_back("3");
  expected.push_back("5");
  actual = *pkb.getPatternMatchesWildLhs(
      std::make_shared<TreeNode>("a", nullptr, nullptr),
      MatchType::PARTIAL_MATCH);
  std::sort(actual.begin(), actual.end());
  REQUIRE(actual == expected);

  // _ = _a_
  expected.clear();
  expected.push_back("4");
  actual = *pkb.getPatternMatchesWildLhs(buildTree2(), MatchType::EXACT_MATCH);
  std::sort(actual.begin(), actual.end());
  REQUIRE(actual == expected);
}