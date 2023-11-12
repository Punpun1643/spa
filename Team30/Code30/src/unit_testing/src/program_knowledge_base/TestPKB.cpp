#include "catch.hpp"
#include "program_knowledge_base/PKB.h"
#include "shared/types/RelationType.h"

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

std::shared_ptr<TreeNode> buildTree5() {
  // 2 * z
  TreeNode multiply =
      TreeNode("*", std::make_shared<TreeNode>("2", nullptr, nullptr),
               std::make_shared<TreeNode>("z", nullptr, nullptr));
  return std::make_shared<TreeNode>(multiply);
}

std::shared_ptr<TreeNode> buildTree6() {
  // p - y
  TreeNode minus =
      TreeNode("-", std::make_shared<TreeNode>("p", nullptr, nullptr),
               std::make_shared<TreeNode>("y", nullptr, nullptr));
  return std::make_shared<TreeNode>(minus);
}

std::shared_ptr<TreeNode> buildTree7() {
  // x + 24
  TreeNode plus =
      TreeNode("+", std::make_shared<TreeNode>("x", nullptr, nullptr),
               std::make_shared<TreeNode>("24", nullptr, nullptr));
  return std::make_shared<TreeNode>(plus);
}

std::shared_ptr<TreeNode> buildTree8() {
  // x * q - 5
  TreeNode multiply =
      TreeNode("*", std::make_shared<TreeNode>("x", nullptr, nullptr),
               std::make_shared<TreeNode>("q", nullptr, nullptr));
  TreeNode minus = TreeNode("-", std::make_shared<TreeNode>(multiply),
                            std::make_shared<TreeNode>("5", nullptr, nullptr));
  return std::make_shared<TreeNode>(minus);
}

std::shared_ptr<TreeNode> buildTree9() {
  // z - 1
  TreeNode minus =
      TreeNode("-", std::make_shared<TreeNode>("z", nullptr, nullptr),
               std::make_shared<TreeNode>("1", nullptr, nullptr));
  return std::make_shared<TreeNode>(minus);
}

std::shared_ptr<TreeNode> buildTree10() {
  // x + y * z + p * q
  TreeNode multiply =
      TreeNode("*", std::make_shared<TreeNode>("y", nullptr, nullptr),
               std::make_shared<TreeNode>("z", nullptr, nullptr));

  TreeNode multiply_2 =
      TreeNode("*", std::make_shared<TreeNode>("p", nullptr, nullptr),
               std::make_shared<TreeNode>("q", nullptr, nullptr));

  TreeNode plus =
      TreeNode("+", std::make_shared<TreeNode>("x", nullptr, nullptr),
               std::make_shared<TreeNode>(multiply));

  TreeNode plus_2 = TreeNode("+", std::make_shared<TreeNode>(plus),
                             std::make_shared<TreeNode>(multiply_2));

  return std::make_shared<TreeNode>(plus_2);
}

std::shared_ptr<TreeNode> buildTree11() {
  // x + j + z
  TreeNode plus =
      TreeNode("+", std::make_shared<TreeNode>("x", nullptr, nullptr),
               std::make_shared<TreeNode>("j", nullptr, nullptr));
  TreeNode plus_2 = TreeNode("+", std::make_shared<TreeNode>(plus),
                             std::make_shared<TreeNode>("z", nullptr, nullptr));
  return std::make_shared<TreeNode>(plus_2);
}

std::shared_ptr<TreeNode> buildTree12() {
  // x * 10
  TreeNode multiply =
      TreeNode("*", std::make_shared<TreeNode>("x", nullptr, nullptr),
               std::make_shared<TreeNode>("10", nullptr, nullptr));
  return std::make_shared<TreeNode>(multiply);
}

std::shared_ptr<TreeNode> buildTree13() {
  // y + y * 10
  TreeNode multiply =
      TreeNode("*", std::make_shared<TreeNode>("y", nullptr, nullptr),
               std::make_shared<TreeNode>("10", nullptr, nullptr));
  TreeNode plus =
      TreeNode("+", std::make_shared<TreeNode>("y", nullptr, nullptr),
               std::make_shared<TreeNode>(multiply));
  return std::make_shared<TreeNode>(plus);
}

std::shared_ptr<TreeNode> buildTree14() {
  // 5 * p - 5
  TreeNode multiply =
      TreeNode("*", std::make_shared<TreeNode>("5", nullptr, nullptr),
               std::make_shared<TreeNode>("p", nullptr, nullptr));
  TreeNode minus = TreeNode("-", std::make_shared<TreeNode>(multiply),
                            std::make_shared<TreeNode>("5", nullptr, nullptr));
  return std::make_shared<TreeNode>(minus);
}

std::shared_ptr<TreeNode> buildTree15() {
  // p * 5
  TreeNode multiply =
      TreeNode("*", std::make_shared<TreeNode>("p", nullptr, nullptr),
               std::make_shared<TreeNode>("5", nullptr, nullptr));
  return std::make_shared<TreeNode>(multiply);
}

std::shared_ptr<TreeNode> buildTree16() {
  // x * 10
  TreeNode multiply =
      TreeNode("*", std::make_shared<TreeNode>("x", nullptr, nullptr),
               std::make_shared<TreeNode>("10", nullptr, nullptr));
  return std::make_shared<TreeNode>(multiply);
}

std::shared_ptr<TreeNode> buildTree17() {
  // x + y * 10
  TreeNode multiply =
      TreeNode("*", std::make_shared<TreeNode>("y", nullptr, nullptr),
               std::make_shared<TreeNode>("10", nullptr, nullptr));
  TreeNode plus =
      TreeNode("+", std::make_shared<TreeNode>("x", nullptr, nullptr),
               std::make_shared<TreeNode>(multiply));
  return std::make_shared<TreeNode>(plus);
}

std::shared_ptr<TreeNode> buildTree18() {
  // a + 1
  TreeNode plus =
      TreeNode("+", std::make_shared<TreeNode>("a", nullptr, nullptr),
               std::make_shared<TreeNode>("1", nullptr, nullptr));
  return std::make_shared<TreeNode>(plus);
}

std::shared_ptr<TreeNode> buildTree19() {
  // i + x + y * z
  TreeNode multiply =
      TreeNode("*", std::make_shared<TreeNode>("y", nullptr, nullptr),
               std::make_shared<TreeNode>("z", nullptr, nullptr));
  TreeNode plus =
      TreeNode("+", std::make_shared<TreeNode>("i", nullptr, nullptr),
               std::make_shared<TreeNode>("x", nullptr, nullptr));
  TreeNode plus_2 = TreeNode("+", std::make_shared<TreeNode>(plus),
                             std::make_shared<TreeNode>(multiply));

  return std::make_shared<TreeNode>(plus_2);
}

TEST_CASE("Follows, Parent, Follows* and Parent*") {
  PKB pkb = PKB();
  pkb.InsertEntity(EntityType::PROCEDURE, "main");
  pkb.InsertEntity(EntityType::PROCEDURE, "sub");
  pkb.InsertEntity(EntityType::CONSTANT, "3");
  pkb.InsertEntity(EntityType::VARIABLE, "x");
  pkb.InsertEntity(EntityType::VARIABLE, "y");

  pkb.InsertEntity(EntityType::CALL, AttrType::PROC_NAME, "1", "sub");
  pkb.InsertEntity(EntityType::CALL, AttrType::PROC_NAME, "2", "sub");
  pkb.InsertEntity(EntityType::PRINT, AttrType::VAR_NAME, "3", "x");
  pkb.InsertEntity(EntityType::IF, "4");
  pkb.InsertEntity(EntityType::READ, AttrType::VAR_NAME, "5", "y");
  pkb.InsertEntity(EntityType::CALL, AttrType::PROC_NAME, "6", "sub");
  pkb.InsertEntity(EntityType::ASSIGN, "7");

  pkb.InsertRelation(RelationType::FOLLOWS, "1", "2");
  pkb.InsertRelation(RelationType::FOLLOWS, "2", "3");
  pkb.InsertRelation(RelationType::FOLLOWS, "3", "4");
  pkb.InsertRelation(RelationType::FOLLOWS, "5", "6");

  pkb.InsertRelation(RelationType::PARENT, "4", "5");
  pkb.InsertRelation(RelationType::PARENT, "4", "6");
  pkb.InsertRelation(RelationType::PARENT, "6", "7");

  // added follows relation
  REQUIRE(pkb.IsRelationTrueValueValue("2", "3", RelationType::FOLLOWS));
  // unadded follows relation
  REQUIRE(pkb.IsRelationTrueValueValue("5", "8", RelationType::FOLLOWS) ==
          false);
  // follows not transitive
  REQUIRE(pkb.IsRelationTrueValueValue("1", "4", RelationType::FOLLOWS) ==
          false);

  // added follows and hence follows_star relation
  REQUIRE(pkb.IsRelationTrueValueValue("2", "3", RelationType::FOLLOWS_STAR));
  // unadded follows_star relation
  REQUIRE(pkb.IsRelationTrueValueValue("1", "6", RelationType::FOLLOWS_STAR) ==
          false);
  // follows_star should be transitive
  REQUIRE(pkb.IsRelationTrueValueValue("1", "3", RelationType::FOLLOWS_STAR));

  // added parent relation
  REQUIRE(pkb.IsRelationTrueValueValue("6", "7", RelationType::PARENT));
  // unadded parent relation
  REQUIRE(pkb.IsRelationTrueValueValue("3", "5", RelationType::PARENT) ==
          false);
  // parent not transitive
  REQUIRE(pkb.IsRelationTrueValueValue("4", "7", RelationType::PARENT) ==
          false);

  // added parent and hence parent_star relation
  REQUIRE(pkb.IsRelationTrueValueValue("4", "5", RelationType::PARENT_STAR));
  // unadded follows_star relation
  REQUIRE(pkb.IsRelationTrueValueValue("1", "6", RelationType::PARENT_STAR) ==
          false);
  // parent_star should be transitive
  REQUIRE(pkb.IsRelationTrueValueValue("4", "7", RelationType::PARENT_STAR));

  // Returns if Relation(int, _)
  // no statements follow 7
  REQUIRE(pkb.IsRelationTrueValueWild("7", RelationType::FOLLOWS) == false);
  // statement 6 follows statement 5
  REQUIRE(pkb.IsRelationTrueValueWild("5", RelationType::FOLLOWS));
  // statement 6 is the Parent* of 7
  REQUIRE(pkb.IsRelationTrueValueWild("6", RelationType::PARENT_STAR));
  // statement 1 is not the parent of any statement
  REQUIRE(pkb.IsRelationTrueValueWild("1", RelationType::PARENT) == false);

  // Returns if Relation(_, int)
  // statement 4 follows* statements 1, 2, 3
  REQUIRE(pkb.IsRelationTrueWildValue("4", RelationType::FOLLOWS_STAR));
  // statement 1 does not follow any statement
  REQUIRE(pkb.IsRelationTrueWildValue("1", RelationType::FOLLOWS) == false);
  // statement 7 has parents
  REQUIRE(pkb.IsRelationTrueWildValue("7", RelationType::PARENT_STAR));

  // Returns if Relation(_, _)
  REQUIRE(pkb.IsRelationTrueWildWild(RelationType::FOLLOWS));
  REQUIRE(pkb.IsRelationTrueWildWild(RelationType::FOLLOWS_STAR));
  REQUIRE(pkb.IsRelationTrueWildWild(RelationType::PARENT));
  REQUIRE(pkb.IsRelationTrueWildWild(RelationType::PARENT_STAR));

  std::vector<std::string> empty_vector;

  // Returns all s such that Relation(s, _)
  // there are no assignment statements that are followed by any other
  // statements
  REQUIRE(pkb.GetRelationSynonymWild(EntityType::ASSIGN,
                                     RelationType::FOLLOWS) == empty_vector);
  // statement 4 is the only if statement that is a parent
  std::vector<std::string> tmp = {"4"};
  REQUIRE(pkb.GetRelationSynonymWild(EntityType::IF,
                                     RelationType::PARENT_STAR) == tmp);
  // there are no while statements
  REQUIRE(pkb.GetRelationSynonymWild(EntityType::WHILE, RelationType::PARENT) ==
          empty_vector);

  // Returns all s such that Relation(s, int)
  // statement 3 follows 1 and 2
  tmp = {"1", "2"};
  std::vector<std::string> actual = pkb.GetRelationSynonymValue(
      EntityType::STMT, "3", RelationType::FOLLOWS_STAR);
  std::sort(actual.begin(), actual.end());

  REQUIRE(actual == tmp);
  // statement 4 is the only if statement that is a parent of 7
  tmp = {"4"};
  REQUIRE(pkb.GetRelationSynonymValue(EntityType::IF, "7",
                                      RelationType::PARENT_STAR) == tmp);
  // statement 2 is the only CALL statement that is followed by 3
  tmp = {"2"};
  REQUIRE(pkb.GetRelationSynonymValue(EntityType::CALL, "3",
                                      RelationType::FOLLOWS) == tmp);
  // no call statements followed by 4
  REQUIRE(pkb.GetRelationSynonymValue(EntityType::CALL, "4",
                                      RelationType::FOLLOWS) == empty_vector);

  // Returns all s such that Relation(int, s)
  // 3 and 4 follows* 2
  tmp = {"3", "4"};
  actual = pkb.GetRelationValueSynonym("2", EntityType::STMT,
                                       RelationType::FOLLOWS_STAR);
  std::sort(actual.begin(), actual.end());
  REQUIRE(actual == tmp);

  // 4 is a parent of 5, 6, 7
  tmp = {"5", "6", "7"};
  actual = pkb.GetRelationValueSynonym("4", EntityType::STMT,
                                       RelationType::PARENT_STAR);
  std::sort(actual.begin(), actual.end());
  REQUIRE(actual == tmp);

  //// 3 is not a parent
  REQUIRE(pkb.GetRelationValueSynonym("3", EntityType::STMT,
                                      RelationType::PARENT) == empty_vector);

  // Returns all s1, s2 such that Relation(s1, s2)
  std::vector<std::pair<std::string, std::string>> tmp_pair;
  tmp_pair = {{"4", "5"}, {"4", "6"}, {"6", "7"}};
  std::vector<std::pair<std::string, std::string>> actual_pairs =
      pkb.GetRelationSynonymSynonym(EntityType::STMT, EntityType::STMT,
                                    RelationType::PARENT);
  std::sort(actual_pairs.begin(), actual_pairs.end());
  REQUIRE(actual_pairs == tmp_pair);

  tmp_pair = {{"4", "5"}, {"4", "6"}};
  std::unordered_set<std::string> possible_values_first = {"4"};
  std::unordered_set<std::string> possible_values_second = {"5", "6", "7"};
  actual_pairs = pkb.GetRelationSynonymSynonym(
      EntityType::STMT, EntityType::STMT, RelationType::PARENT,
      possible_values_first, possible_values_second);
  std::sort(actual_pairs.begin(), actual_pairs.end());
  REQUIRE(actual_pairs == tmp_pair);

  tmp_pair = {{"6", "7"}};
  possible_values_first = {"4", "6"};
  possible_values_second = {"7"};
  actual_pairs = pkb.GetRelationSynonymSynonym(
      EntityType::STMT, EntityType::STMT, RelationType::PARENT,
      possible_values_first, possible_values_second);
  std::sort(actual_pairs.begin(), actual_pairs.end());
  REQUIRE(actual_pairs == tmp_pair);

  tmp_pair = {{"4", "6"}};
  possible_values_first = {"4", "77", "8"};
  possible_values_second = {"7", "3", "4", "6"};
  actual_pairs = pkb.GetRelationSynonymSynonym(
      EntityType::STMT, EntityType::STMT, RelationType::PARENT,
      possible_values_first, possible_values_second);
  std::sort(actual_pairs.begin(), actual_pairs.end());
  REQUIRE(actual_pairs == tmp_pair);

  tmp_pair = {{"4", "5"}, {"6", "7"}};
  possible_values_first = {};
  possible_values_second = {"5", "3", "4", "7"};
  actual_pairs = pkb.GetRelationSynonymSynonym(
      EntityType::STMT, EntityType::STMT, RelationType::PARENT,
      possible_values_first, possible_values_second);
  std::sort(actual_pairs.begin(), actual_pairs.end());
  REQUIRE(actual_pairs == tmp_pair);

  tmp_pair = {{"4", "5"}, {"4", "6"}};
  possible_values_first = {};
  possible_values_second = {"5", "6"};
  actual_pairs = pkb.GetRelationSynonymSynonym(
      EntityType::STMT, EntityType::STMT, RelationType::PARENT,
      possible_values_first, possible_values_second);
  std::sort(actual_pairs.begin(), actual_pairs.end());
  REQUIRE(actual_pairs == tmp_pair);
}

TEST_CASE("Follows, Parent, Follows* and Parent* with empty PKB") {
  PKB pkb = PKB();
  REQUIRE(pkb.IsRelationTrueWildWild(RelationType::FOLLOWS) == false);
  REQUIRE(pkb.IsRelationTrueWildWild(RelationType::PARENT_STAR) == false);
}

// UsesS only holds Uses relations for Statements
// UsesP holds for Procedures
TEST_CASE("Uses and Modifies") {
  PKB pkb = PKB();
  pkb.InsertEntity(EntityType::PROCEDURE, "main");
  pkb.InsertEntity(EntityType::PROCEDURE, "sub");
  pkb.InsertEntity(EntityType::VARIABLE, "x");
  pkb.InsertEntity(EntityType::VARIABLE, "y");

  pkb.InsertEntity(EntityType::CALL, AttrType::PROC_NAME, "1", "sub");
  pkb.InsertEntity(EntityType::CALL, AttrType::PROC_NAME, "2", "sub");
  pkb.InsertEntity(EntityType::PRINT, AttrType::VAR_NAME, "3", "x");
  pkb.InsertEntity(EntityType::IF, "4");
  pkb.InsertEntity(EntityType::READ, AttrType::VAR_NAME, "5", "y");
  pkb.InsertEntity(EntityType::CALL, AttrType::PROC_NAME, "6", "sub");
  pkb.InsertEntity(EntityType::ASSIGN, "7");

  pkb.InsertRelation(RelationType::USES_S, "3", "x");
  pkb.InsertRelation(RelationType::USES_S, "4", "y");
  pkb.InsertRelation(RelationType::USES_P, "sub", "x");

  pkb.InsertRelation(RelationType::MODIFIES_P, "main", "x");
  pkb.InsertRelation(RelationType::MODIFIES_S, "5", "x");
  pkb.InsertRelation(RelationType::MODIFIES_S, "7", "y");

  // added USES relation for statement
  REQUIRE(pkb.IsRelationTrueValueValue("3", "x", RelationType::USES_S));
  // added USES relation for procedure
  REQUIRE(pkb.IsRelationTrueValueValue("sub", "x", RelationType::USES_P));

  // USES relation for statement should not be in procedure table
  REQUIRE(pkb.IsRelationTrueValueValue("4", "y", RelationType::USES_P) ==
          false);
  // USES relation for procedure should not be in statement table
  REQUIRE(pkb.IsRelationTrueValueValue("sub", "x", RelationType::USES_S) ==
          false);

  // added MODIFIES relation for statement
  REQUIRE(pkb.IsRelationTrueValueValue("5", "x", RelationType::MODIFIES_S));
  // added MODIFIES relation for procedure
  REQUIRE(pkb.IsRelationTrueValueValue("main", "x", RelationType::MODIFIES_P));

  // MODIFIES relation for statement should not be in procedure table
  REQUIRE(pkb.IsRelationTrueValueValue("7", "y", RelationType::MODIFIES_P) ==
          false);
  // MODIFIES relation for procedure should not be in statement table
  REQUIRE(pkb.IsRelationTrueValueValue("main", "x", RelationType::MODIFIES_S) ==
          false);

  std::vector<std::string> empty_vector;

  // Check APIs

  std::vector<std::string> tmp = {"main"};
  // Select p such that Modifies(p, _)
  REQUIRE(pkb.GetRelationSynonymWild(EntityType::PROCEDURE,
                                     RelationType::MODIFIES_P) == tmp);
  tmp = {"5", "7"};
  // Select s such that Modifies(s, _)
  REQUIRE(pkb.GetRelationSynonymWild(EntityType::STMT,
                                     RelationType::MODIFIES_S) == tmp);
  // Query from wrong table
  REQUIRE(pkb.GetRelationSynonymWild(EntityType::STMT, RelationType::USES_P) ==
          empty_vector);

  // Select Uses(pn, v)
  // Statement 3 is a print statement that uses "x". Returns that pair.
  std::vector<std::pair<std::string, std::string>> emptyPair = {};
  std::vector<std::pair<std::string, std::string>> tmp1 = {
      std::make_pair("3", "x")};

  REQUIRE(pkb.GetRelationSynonymSynonym(EntityType::PRINT, EntityType::VARIABLE,
                                        RelationType::USES_S) == tmp1);

  // Modifies(c, v)
  // No procedure call that modifies a variable
  REQUIRE(pkb.GetRelationSynonymSynonym(EntityType::CALL, EntityType::VARIABLE,
                                        RelationType::MODIFIES_S) == emptyPair);
}

TEST_CASE("PKB test1-source Parent*") {
  PKB pkb = PKB();
  pkb.InsertRelation(RelationType::PARENT, "4", "5");
  pkb.InsertRelation(RelationType::PARENT, "4", "6");
  pkb.InsertRelation(RelationType::PARENT, "4", "7");
  pkb.InsertRelation(RelationType::PARENT, "7", "8");
  pkb.InsertRelation(RelationType::PARENT, "4", "10");
  pkb.InsertRelation(RelationType::PARENT, "14", "15");
  pkb.InsertRelation(RelationType::PARENT, "14", "16");
  pkb.InsertRelation(RelationType::PARENT, "20", "21");
  pkb.InsertRelation(RelationType::PARENT, "20", "22");
  pkb.InsertRelation(RelationType::PARENT, "20", "23");
  pkb.InsertRelation(RelationType::PARENT, "20", "24");
  pkb.InsertRelation(RelationType::PARENT, "24", "25");
  pkb.InsertRelation(RelationType::PARENT, "24", "26");

  for (int i = 1; i < 27; i++) {
    REQUIRE(!pkb.IsRelationTrueValueValue("22", std::to_string(i),
                                          RelationType::PARENT_STAR));
    REQUIRE(!pkb.IsRelationTrueValueValue("22", std::to_string(i),
                                          RelationType::PARENT));
  }

  REQUIRE(!pkb.IsRelationTrueValueValue("2", "100", RelationType::PARENT_STAR));
  REQUIRE(!pkb.IsRelationTrueValueValue("100", "2", RelationType::PARENT_STAR));
  REQUIRE(!pkb.IsRelationTrueValueValue("0", "1", RelationType::FOLLOWS_STAR));
  REQUIRE(!pkb.IsRelationTrueValueValue("2", "0", RelationType::PARENT_STAR));
  REQUIRE(!pkb.IsRelationTrueValueValue("2", "-1", RelationType::FOLLOWS_STAR));
  REQUIRE(!pkb.IsRelationTrueValueValue("0", "300", RelationType::PARENT_STAR));
}

TEST_CASE("Test1-Source PKB") {
  PKB pkb = PKB();
  pkb.InsertEntity(EntityType::PROCEDURE, "Advanced");
  pkb.InsertEntity(EntityType::VARIABLE, "y");
  pkb.InsertEntity(EntityType::VARIABLE, "z");
  pkb.InsertEntity(EntityType::VARIABLE, "p");
  pkb.InsertEntity(EntityType::VARIABLE, "q");
  pkb.InsertEntity(EntityType::VARIABLE, "i");
  pkb.InsertEntity(EntityType::VARIABLE, "j");
  pkb.InsertEntity(EntityType::VARIABLE, "x");
  pkb.InsertEntity(EntityType::VARIABLE, "t");
  pkb.InsertEntity(EntityType::VARIABLE, "a");
  pkb.InsertEntity(EntityType::VARIABLE, "b");
  pkb.InsertEntity(EntityType::CONSTANT, "2");
  pkb.InsertEntity(EntityType::CONSTANT, "1");
  pkb.InsertEntity(EntityType::CONSTANT, "24");
  pkb.InsertEntity(EntityType::CONSTANT, "0");
  pkb.InsertEntity(EntityType::CONSTANT, "5");
  pkb.InsertEntity(EntityType::VARIABLE, "10");
  pkb.InsertEntity(EntityType::VARIABLE, "100");
  pkb.InsertEntity(EntityType::ASSIGN, "2");
  pkb.InsertEntity(EntityType::ASSIGN, "3");
  pkb.InsertEntity(EntityType::ASSIGN, "5");
  pkb.InsertEntity(EntityType::ASSIGN, "8");
  pkb.InsertEntity(EntityType::ASSIGN, "9");
  pkb.InsertEntity(EntityType::ASSIGN, "10");
  pkb.InsertEntity(EntityType::ASSIGN, "11");
  pkb.InsertEntity(EntityType::ASSIGN, "12");
  pkb.InsertEntity(EntityType::ASSIGN, "13");
  pkb.InsertEntity(EntityType::ASSIGN, "15");
  pkb.InsertEntity(EntityType::ASSIGN, "17");
  pkb.InsertEntity(EntityType::ASSIGN, "18");
  pkb.InsertEntity(EntityType::ASSIGN, "19");
  pkb.InsertEntity(EntityType::ASSIGN, "22");
  pkb.InsertEntity(EntityType::ASSIGN, "23");
  pkb.InsertEntity(EntityType::ASSIGN, "25");
  pkb.InsertEntity(EntityType::ASSIGN, "26");
  pkb.InsertEntity(EntityType::ASSIGN, "27");

  pkb.InsertAssignPattern("2", "x", buildTree5());
  pkb.InsertAssignPattern("3", "z", buildTree6());
  pkb.InsertAssignPattern("5", "z", buildTree7());
  pkb.InsertAssignPattern("8", "y", buildTree8());
  pkb.InsertAssignPattern("9", "z", buildTree9());
  pkb.InsertAssignPattern("10", "y", buildTree10());
  pkb.InsertAssignPattern("11", "i", buildTree11());
  pkb.InsertAssignPattern("12", "p", buildTree12());
  pkb.InsertAssignPattern("13", "q", buildTree13());
  pkb.InsertAssignPattern("15", "a", buildTree14());
  pkb.InsertAssignPattern("17", "q", buildTree15());
  pkb.InsertAssignPattern("18", "i", buildTree16());
  pkb.InsertAssignPattern("19", "j", buildTree17());
  pkb.InsertAssignPattern("22", "x",
                          std::make_shared<TreeNode>("1", nullptr, nullptr));
  pkb.InsertAssignPattern("23", "y", buildTree19());
  pkb.InsertAssignPattern("25", "x", buildTree18());
  pkb.InsertAssignPattern("26", "a",
                          std::make_shared<TreeNode>("b", nullptr, nullptr));

  std::vector<std::string> expected_res = {"8", "15", "17"};
  std::vector<std::string> actual;
  actual = pkb.GetMatchingAssignStmts(
      std::make_shared<TreeNode>("5", nullptr, nullptr),
      MatchType::PARTIAL_MATCH);

  std::sort(actual.begin(), actual.end());
  std::sort(expected_res.begin(), expected_res.end());
  REQUIRE(actual == expected_res);

  std::vector<std::pair<std::string, std::string>> expected_pairs = {
      {"9", "z"}, {"22", "x"}, {"25", "x"}};
  std::vector<std::pair<std::string, std::string>> actual_pairs;
  actual_pairs = pkb.GetMatchingAssignStmtLhsVarPairs(
      std::make_shared<TreeNode>("1", nullptr, nullptr),
      MatchType::PARTIAL_MATCH);
  std::sort(actual_pairs.begin(), actual_pairs.end());
  std::sort(expected_pairs.begin(), expected_pairs.end());
  REQUIRE(actual_pairs == expected_pairs);

  std::unordered_set<std::string> possible_values_1;
  std::unordered_set<std::string> possible_values_2;

  possible_values_1 = {"9", "22", "25"};
  possible_values_2 = {"x"};
  expected_pairs = {{"22", "x"}, {"25", "x"}};
  actual_pairs = pkb.GetMatchingAssignStmtLhsVarPairs(
      std::make_shared<TreeNode>("1", nullptr, nullptr),
      MatchType::PARTIAL_MATCH, possible_values_1, possible_values_2);
  std::sort(actual_pairs.begin(), actual_pairs.end());
  std::sort(expected_pairs.begin(), expected_pairs.end());
  REQUIRE(actual_pairs == expected_pairs);

  possible_values_1 = {"9", "22", "25"};
  possible_values_2 = {};
  expected_pairs = {{"9", "z"}, {"22", "x"}, {"25", "x"}};
  actual_pairs = pkb.GetMatchingAssignStmtLhsVarPairs(
      std::make_shared<TreeNode>("1", nullptr, nullptr),
      MatchType::PARTIAL_MATCH, possible_values_1, possible_values_2);
  std::sort(actual_pairs.begin(), actual_pairs.end());
  std::sort(expected_pairs.begin(), expected_pairs.end());
  REQUIRE(actual_pairs == expected_pairs);

  possible_values_1 = {};
  possible_values_2 = {"z"};
  expected_pairs = {{"9", "z"}};
  actual_pairs = pkb.GetMatchingAssignStmtLhsVarPairs(
      std::make_shared<TreeNode>("1", nullptr, nullptr),
      MatchType::PARTIAL_MATCH, possible_values_1, possible_values_2);
  std::sort(actual_pairs.begin(), actual_pairs.end());
  std::sort(expected_pairs.begin(), expected_pairs.end());
  REQUIRE(actual_pairs == expected_pairs);

  expected_res = {"8", "10"};
  actual = pkb.GetMatchingAssignStmts(
      std::make_shared<TreeNode>("q", nullptr, nullptr),
      MatchType::PARTIAL_MATCH);
  std::sort(actual.begin(), actual.end());
  std::sort(expected_res.begin(), expected_res.end());
  REQUIRE(actual == expected_res);

  expected_pairs = {{"26", "a"}};
  actual_pairs = pkb.GetMatchingAssignStmtLhsVarPairs(
      std::make_shared<TreeNode>("b", nullptr, nullptr),
      MatchType::PARTIAL_MATCH);
  std::sort(actual_pairs.begin(), actual_pairs.end());
  std::sort(expected_pairs.begin(), expected_pairs.end());
  REQUIRE(actual_pairs == expected_pairs);
}

TEST_CASE("Pattern Database Assignment insertion and retrieval") {
  PKB pkb = PKB();

  /*
     Line 3: x = a + b * c % d;
     Line 4: y = b * c + d;
     Line 5: x = a + b;
  */

  pkb.InsertEntity(EntityType::ASSIGN, "3");
  pkb.InsertEntity(EntityType::ASSIGN, "4");
  pkb.InsertEntity(EntityType::ASSIGN, "5");
  pkb.InsertEntity(EntityType::VARIABLE, "x");
  pkb.InsertEntity(EntityType::VARIABLE, "y");
  pkb.InsertEntity(EntityType::VARIABLE, "a");
  pkb.InsertEntity(EntityType::VARIABLE, "b");
  pkb.InsertEntity(EntityType::VARIABLE, "c");
  pkb.InsertEntity(EntityType::VARIABLE, "d");

  pkb.InsertAssignPattern("3", "x", buildTree1());
  pkb.InsertAssignPattern("4", "y", buildTree2());
  pkb.InsertAssignPattern("5", "x", buildTree3());

  std::vector<std::pair<std::string, std::string>> expected_pairs;

  // syn = _ (all assignment statements)
  expected_pairs.push_back({"3", "x"});
  expected_pairs.push_back({"4", "y"});
  expected_pairs.push_back({"5", "x"});
  auto actual_pairs =
      pkb.GetMatchingAssignStmtLhsVarPairs(buildTree4(), MatchType::WILD_MATCH);
  std::sort(actual_pairs.begin(), actual_pairs.end());
  REQUIRE(actual_pairs == expected_pairs);

  // syn = _b * c_
  expected_pairs.clear();
  expected_pairs.push_back({"3", "x"});
  expected_pairs.push_back({"4", "y"});
  actual_pairs = pkb.GetMatchingAssignStmtLhsVarPairs(buildTree4(),
                                                      MatchType::PARTIAL_MATCH);
  std::sort(actual_pairs.begin(), actual_pairs.end());
  REQUIRE(actual_pairs == expected_pairs);

  // syn = b * c
  expected_pairs.clear();
  actual_pairs = pkb.GetMatchingAssignStmtLhsVarPairs(buildTree4(),
                                                      MatchType::EXACT_MATCH);
  std::sort(actual_pairs.begin(), actual_pairs.end());
  REQUIRE(actual_pairs == expected_pairs);

  // syn = a + b
  expected_pairs.push_back({"5", "x"});
  actual_pairs = pkb.GetMatchingAssignStmtLhsVarPairs(buildTree3(),
                                                      MatchType::EXACT_MATCH);
  std::sort(actual_pairs.begin(), actual_pairs.end());
  REQUIRE(actual_pairs == expected_pairs);

  // x = _
  std::vector<std::string> expected;
  expected.push_back("3");
  expected.push_back("5");
  auto actual =
      pkb.GetMatchingAssignStmts("x", buildTree4(), MatchType::WILD_MATCH);
  std::sort(actual.begin(), actual.end());
  REQUIRE(actual == expected);

  // x = _b * c_
  expected.clear();
  expected.push_back("3");
  actual =
      pkb.GetMatchingAssignStmts("x", buildTree4(), MatchType::PARTIAL_MATCH);
  std::sort(actual.begin(), actual.end());
  REQUIRE(actual == expected);

  // x = _a + b_
  expected.clear();
  expected.push_back("5");
  actual =
      pkb.GetMatchingAssignStmts("x", buildTree3(), MatchType::PARTIAL_MATCH);
  std::sort(actual.begin(), actual.end());
  REQUIRE(actual == expected);

  // x = a + b * c % d
  expected.clear();
  expected.push_back("3");
  actual =
      pkb.GetMatchingAssignStmts("x", buildTree1(), MatchType::EXACT_MATCH);
  std::sort(actual.begin(), actual.end());
  REQUIRE(actual == expected);

  // y = a + b * c % d;
  expected.clear();
  actual =
      pkb.GetMatchingAssignStmts("y", buildTree1(), MatchType::EXACT_MATCH);
  std::sort(actual.begin(), actual.end());
  REQUIRE(actual == expected);

  // _ = _
  expected.clear();
  expected.push_back("3");
  expected.push_back("4");
  expected.push_back("5");
  actual = pkb.GetMatchingAssignStmts(buildTree1(), MatchType::WILD_MATCH);
  std::sort(actual.begin(), actual.end());
  REQUIRE(actual == expected);

  // _ = _a_
  expected.clear();
  expected.push_back("3");
  expected.push_back("5");
  actual = pkb.GetMatchingAssignStmts(
      std::make_shared<TreeNode>("a", nullptr, nullptr),
      MatchType::PARTIAL_MATCH);
  std::sort(actual.begin(), actual.end());
  REQUIRE(actual == expected);

  // _ = _a_
  expected.clear();
  expected.push_back("4");
  actual = pkb.GetMatchingAssignStmts(buildTree2(), MatchType::EXACT_MATCH);
  std::sort(actual.begin(), actual.end());
  REQUIRE(actual == expected);
}

TEST_CASE("Get statement uses/modifies and Get procedure uses/modifies") {
  /*
  procedure main {
1.    a = b + c;
2.    print a;
3.    call sub
  }

  procedure sub {
4.    read c;
  }
  */
  PKB pkb = PKB();
  pkb.InsertEntity(EntityType::VARIABLE, "a");
  pkb.InsertEntity(EntityType::VARIABLE, "b");
  pkb.InsertEntity(EntityType::VARIABLE, "c");
  pkb.InsertEntity(EntityType::PROCEDURE, "main");
  pkb.InsertEntity(EntityType::PROCEDURE, "sub");
  pkb.InsertEntity(EntityType::ASSIGN, "1");
  pkb.InsertEntity(EntityType::PRINT, AttrType::VAR_NAME, "2", "a");
  pkb.InsertEntity(EntityType::CALL, AttrType::PROC_NAME, "3", "sub");
  pkb.InsertEntity(EntityType::READ, AttrType::VAR_NAME, "4", "c");

  pkb.InsertRelation(RelationType::USES_S, "1", "b");
  pkb.InsertRelation(RelationType::USES_S, "1", "c");
  pkb.InsertRelation(RelationType::MODIFIES_S, "1", "a");
  pkb.InsertRelation(RelationType::USES_S, "2", "a");
  pkb.InsertRelation(RelationType::USES_P, "main", "b");
  pkb.InsertRelation(RelationType::USES_P, "main", "c");
  pkb.InsertRelation(RelationType::USES_P, "main", "a");
  pkb.InsertRelation(RelationType::MODIFIES_P, "main", "a");
  pkb.InsertRelation(RelationType::MODIFIES_P, "main", "c");
  pkb.InsertRelation(RelationType::MODIFIES_S, "3", "c");
  pkb.InsertRelation(RelationType::MODIFIES_S, "4", "c");
  pkb.InsertRelation(RelationType::MODIFIES_P, "sub", "c");

  std::unordered_set<std::string> expected = {"a"};
  std::unordered_set<std::string> actual = pkb.GetStatementModifies("1");
  REQUIRE(expected == actual);

  expected = {"b", "c"};
  actual = pkb.GetStatementUses("1");
  REQUIRE(expected == actual);

  expected = {"a"};
  actual = pkb.GetStatementUses("2");
  REQUIRE(expected == actual);

  expected = {};
  actual = pkb.GetStatementModifies("2");
  REQUIRE(expected == actual);

  expected = {};
  actual = pkb.GetStatementUses("3");
  REQUIRE(expected == actual);

  expected = {"c"};
  actual = pkb.GetStatementModifies("3");
  REQUIRE(expected == actual);

  expected = {};
  actual = pkb.GetStatementUses("4");
  REQUIRE(expected == actual);

  expected = {"c"};
  actual = pkb.GetStatementModifies("4");
  REQUIRE(expected == actual);

  expected = {"a", "b", "c"};
  actual = pkb.GetProcedureUses("main");
  REQUIRE(expected == actual);

  expected = {"a", "c"};
  actual = pkb.GetProcedureModifies("main");
  REQUIRE(expected == actual);

  expected = {"c"};
  actual = pkb.GetProcedureModifies("sub");
  REQUIRE(expected == actual);
}
