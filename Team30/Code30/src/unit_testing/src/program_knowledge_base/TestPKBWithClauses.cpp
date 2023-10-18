#include "../../../spa/src/program_knowledge_base/PKB.h"
#include "catch.hpp"

TEST_CASE("PKB With Clauses") {
  /*
          procedure With {
          1.  x = a + b;
          2.  y = c + b;
          3.  call sub;
          4.  print sub;
          5.  if (x == 3) {
          6.      read c;
                  }
          }

          procedure sub {
          7.  while (y < 3) {
          8.      print y;
          9.      print sub;
              }
          }
  */
  PKB pkb = PKB();
  pkb.InsertEntity(EntityType::VARIABLE, "x");
  pkb.InsertEntity(EntityType::VARIABLE, "y");
  pkb.InsertEntity(EntityType::VARIABLE, "a");
  pkb.InsertEntity(EntityType::VARIABLE, "b");
  pkb.InsertEntity(EntityType::VARIABLE, "c");
  pkb.InsertEntity(EntityType::VARIABLE, "sub");
  pkb.InsertEntity(EntityType::CONSTANT, "3");
  pkb.InsertEntity(EntityType::PROCEDURE, "With");
  pkb.InsertEntity(EntityType::PROCEDURE, "sub");
  pkb.InsertEntity(EntityType::VARIABLE, "x");
  pkb.InsertEntity(EntityType::ASSIGN, "1");
  pkb.InsertEntity(EntityType::ASSIGN, "2");
  pkb.InsertEntity(EntityType::CALL, AttrType::PROC_NAME, "3", "sub");
  pkb.InsertEntity(EntityType::PRINT, AttrType::VAR_NAME, "4", "sub");
  pkb.InsertEntity(EntityType::IF, "5");
  pkb.InsertEntity(EntityType::READ, AttrType::VAR_NAME, "6", "c");
  pkb.InsertEntity(EntityType::WHILE, "7");
  pkb.InsertEntity(EntityType::PRINT, AttrType::VAR_NAME, "8", "y");
  pkb.InsertEntity(EntityType::PRINT, AttrType::VAR_NAME, "9", "sub");

  // read.varName = c
  std::vector<std::string> expected = {"6"};
  REQUIRE(pkb.GetEntitiesMatchingAttrValue(EntityType::READ, AttrType::VAR_NAME,
                                           "c") == expected);

  // proc.procName = With
  expected = {"With"};
  REQUIRE(pkb.GetEntitiesMatchingAttrValue(
              EntityType::PROCEDURE, AttrType::PROC_NAME, "With") == expected);

  // print.varName = sub
  expected = {"4", "9"};
  std::vector<std::string> actual = pkb.GetEntitiesMatchingAttrValue(
      EntityType::PRINT, AttrType::VAR_NAME, "sub");
  std::sort(actual.begin(), actual.end());
  REQUIRE(actual == expected);

  // call.procName = sub
  expected = {"3"};
  actual = pkb.GetEntitiesMatchingAttrValue(EntityType::CALL,
                                            AttrType::PROC_NAME, "sub");
  REQUIRE(actual == expected);

  // call.procName = x
  expected = {};
  actual = pkb.GetEntitiesMatchingAttrValue(EntityType::CALL,
                                            AttrType::PROC_NAME, "x");
  REQUIRE(actual == expected);

  // stmt.stmt# = 1
  expected = {"1"};
  REQUIRE(pkb.GetEntitiesMatchingAttrValue(
              EntityType::ASSIGN, AttrType::STMT_NUM, "1") == expected);

  // var.varName = a
  expected = {"a"};
  REQUIRE(pkb.GetEntitiesMatchingAttrValue(
              EntityType::VARIABLE, AttrType::VAR_NAME, "a") == expected);

  // const.value = 3
  expected = {"3"};
  REQUIRE(pkb.GetEntitiesMatchingAttrValue(EntityType::CONSTANT,
                                           AttrType::VALUE, "3") == expected);

  // const.value = 55555
  expected = {};
  REQUIRE(pkb.GetEntitiesMatchingAttrValue(
              EntityType::CONSTANT, AttrType::VALUE, "55555") == expected);

  // print.varName = call.procName
  std::vector<std::pair<std::string, std::string>> expected_pairs = {
      {"4", "3"}, {"9", "3"}};
  std::vector<std::pair<std::string, std::string>> actual_pairs =
      pkb.GetEntitiesWhereAttributesMatch(EntityType::PRINT, AttrType::VAR_NAME,
                                          EntityType::CALL,
                                          AttrType::PROC_NAME);
  std::sort(actual_pairs.begin(), actual_pairs.end());
  REQUIRE(actual_pairs == expected_pairs);

  // proc.procName = print.varName
  expected_pairs = {{"sub", "4"}, {"sub", "9"}};
  actual_pairs = pkb.GetEntitiesWhereAttributesMatch(
      EntityType::PROCEDURE, AttrType::PROC_NAME, EntityType::PRINT,
      AttrType::VAR_NAME);
  std::sort(actual_pairs.begin(), actual_pairs.end());
  REQUIRE(actual_pairs == expected_pairs);

  // call.stmt# = constant.value
  expected_pairs = {{"3", "3"}};
  actual_pairs = pkb.GetEntitiesWhereAttributesMatch(
      EntityType::CALL, AttrType::STMT_NUM, EntityType::CONSTANT,
      AttrType::VALUE);
  std::sort(actual_pairs.begin(), actual_pairs.end());
  REQUIRE(actual_pairs == expected_pairs);

  // stmt.stmt# = constant.value
  expected_pairs = {{"3", "3"}};
  actual_pairs = pkb.GetEntitiesWhereAttributesMatch(
      EntityType::STMT, AttrType::STMT_NUM, EntityType::CONSTANT,
      AttrType::VALUE);
  std::sort(actual_pairs.begin(), actual_pairs.end());
  REQUIRE(actual_pairs == expected_pairs);

  // convert call.stmt# to procName
  std::string expected_str = "sub";
  REQUIRE(pkb.ConvertEntityValueToAlias("3", EntityType::CALL,
                                        AttrType::STMT_NUM,
                                        AttrType::PROC_NAME) == expected_str);

  // convert call.stmt# to varName (invalid, returns empty string)
  expected_str = "";
  REQUIRE(pkb.ConvertEntityValueToAlias("3", EntityType::CALL,
                                        AttrType::STMT_NUM,
                                        AttrType::VAR_NAME) == expected_str);

  // convert print.stmt# to varName
  expected_str = "sub";
  REQUIRE(pkb.ConvertEntityValueToAlias("4", EntityType::PRINT,
                                        AttrType::STMT_NUM,
                                        AttrType::VAR_NAME) == expected_str);

  // convert read.stmt# to varName
  expected_str = "c";
  REQUIRE(pkb.ConvertEntityValueToAlias("6", EntityType::READ,
                                        AttrType::STMT_NUM,
                                        AttrType::VAR_NAME) == expected_str);

  // convert assign.stmt# to varName (invalid, returns empty string)
  expected_str = "";
  REQUIRE(pkb.ConvertEntityValueToAlias("1", EntityType::ASSIGN,
                                        AttrType::STMT_NUM,
                                        AttrType::VAR_NAME) == expected_str);

  // get entities matching default attribute values
  expected = {"1"};
  REQUIRE(pkb.GetEntitiesMatchingAttrValue(
              EntityType::ASSIGN, AttrType::STMT_NUM, "1") == expected);

  expected = {"4"};
  REQUIRE(pkb.GetEntitiesMatchingAttrValue(
              EntityType::PRINT, AttrType::STMT_NUM, "4") == expected);

  // no matching default attribute values
  expected = {};
  REQUIRE(pkb.GetEntitiesMatchingAttrValue(
              EntityType::PRINT, AttrType::STMT_NUM, "5") == expected);
}