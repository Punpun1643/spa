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
  REQUIRE(pkb.isRelationTrue("2", "3", RelationType::FOLLOWS));
  // unadded follows relation
  REQUIRE(pkb.isRelationTrue("5", "8", RelationType::FOLLOWS) == false);
  // follows not transitive
  REQUIRE(pkb.isRelationTrue("1", "4", RelationType::FOLLOWS) == false);

  // added follows and hence follows_star relation
  REQUIRE(pkb.isRelationTrue("2", "3", RelationType::FOLLOWS_STAR));
  // unadded follows_star relation
  REQUIRE(pkb.isRelationTrue("1", "6", RelationType::FOLLOWS_STAR) == false);
  // follows_star should be transitive
  REQUIRE(pkb.isRelationTrue("1", "3", RelationType::FOLLOWS_STAR));

  // added parent relation
  REQUIRE(pkb.isRelationTrue("6", "7", RelationType::PARENT));
  // unadded parent relation
  REQUIRE(pkb.isRelationTrue("3", "5", RelationType::PARENT) == false);
  // parent not transitive
  REQUIRE(pkb.isRelationTrue("4", "7", RelationType::PARENT) == false);

  // added parent and hence parent_star relation
  REQUIRE(pkb.isRelationTrue("4", "5", RelationType::PARENT_STAR));
  // unadded follows_star relation
  REQUIRE(pkb.isRelationTrue("1", "6", RelationType::PARENT_STAR) == false);
  // parent_star should be transitive
  REQUIRE(pkb.isRelationTrue("4", "7", RelationType::PARENT_STAR));

  // Returns if Relation(int, _)
  // no statements follow 7
  REQUIRE(pkb.isRelationTrueGivenFirstValue("7", RelationType::FOLLOWS) ==
          false);
  // statement 6 follows statement 5
  REQUIRE(pkb.isRelationTrueGivenFirstValue("5", RelationType::FOLLOWS));
  // statement 6 is the Parent* of 7
  REQUIRE(pkb.isRelationTrueGivenFirstValue("6", RelationType::PARENT_STAR));
  // statement 1 is not the parent of any statement
  REQUIRE(pkb.isRelationTrueGivenFirstValue("1", RelationType::PARENT) ==
          false);

  // Returns if Relation(_, int)
  // statement 4 follows* statements 1, 2, 3
  REQUIRE(pkb.isRelationTrueGivenSecondValue("4", RelationType::FOLLOWS_STAR));
  // statement 1 does not follow any statement
  REQUIRE(pkb.isRelationTrueGivenSecondValue("1", RelationType::FOLLOWS) ==
          false);
  // statement 7 has parents
  REQUIRE(pkb.isRelationTrueGivenSecondValue("7", RelationType::PARENT_STAR));

  // Returns if Relation(_, _)
  REQUIRE(pkb.isRelationTrueForAny(RelationType::FOLLOWS));
  REQUIRE(pkb.isRelationTrueForAny(RelationType::FOLLOWS_STAR));
  REQUIRE(pkb.isRelationTrueForAny(RelationType::PARENT));
  REQUIRE(pkb.isRelationTrueForAny(RelationType::PARENT_STAR));

  std::vector<std::string> empty_vector;

  // Returns all s such that Relation(s, _)
  // there are no assignment statements that are followed by any other
  // statements
  REQUIRE(*pkb.getRelationValuesGivenFirstType(
              EntityType::ASSIGN, RelationType::FOLLOWS) == empty_vector);
  // statement 4 is the only if statement that is a parent
  std::vector<std::string> tmp = {"4"};
  REQUIRE(*pkb.getRelationValuesGivenFirstType(
              EntityType::IF, RelationType::PARENT_STAR) == tmp);
  // there are no while statements
  REQUIRE(*pkb.getRelationValuesGivenFirstType(
              EntityType::WHILE, RelationType::PARENT) == empty_vector);

  // Returns all s such that Relation(s, int)
  // statement 3 follows 1 and 2
  tmp = {"1", "2"};
  REQUIRE(*pkb.getRelationValues(EntityType::STMT, "3",
                                 RelationType::FOLLOWS_STAR) == tmp);
  // statement 4 is the only if statement that is a parent of 7
  tmp = {"4"};
  REQUIRE(*pkb.getRelationValues(EntityType::IF, "7",
                                 RelationType::PARENT_STAR) == tmp);
  // statement 2 is the only CALL statement that is followed by 3
  tmp = {"2"};
  REQUIRE(*pkb.getRelationValues(EntityType::CALL, "3",
                                 RelationType::FOLLOWS) == tmp);
  // no call statements followed by 4
  REQUIRE(*pkb.getRelationValues(EntityType::CALL, "4",
                                 RelationType::FOLLOWS) == empty_vector);

  // Returns all s such that Relation(int, s)
  // 3 and 4 follows* 2
  tmp = {"3", "4"};
  REQUIRE(*pkb.getRelationValues("2", EntityType::STMT,
                                 RelationType::FOLLOWS_STAR) == tmp);
  // 4 is a parent of 5, 6, 7
  tmp = {"5", "6", "7"};
  REQUIRE(*pkb.getRelationValues("4", EntityType::STMT,
                                 RelationType::PARENT_STAR) == tmp);
  //// 3 is not a parent
  REQUIRE(*pkb.getRelationValues("3", EntityType::STMT, RelationType::PARENT) ==
          empty_vector);

  // Returns all s1, s2 such that Relation(s1, s2)
  std::vector<std::pair<std::string, std::string>> tmp_pair;
  tmp_pair = {{"4", "5"}, {"4", "6"}, {"6", "7"}};
  REQUIRE(
      *pkb.getRelationValues(EntityType::STMT, EntityType::STMT,
                                 RelationType::PARENT) == tmp_pair);
}



TEST_CASE("Follows, Parent, Follows* and Parent* with empty PKB") {
  PKB pkb = PKB();
  REQUIRE(pkb.isRelationTrueForAny(RelationType::FOLLOWS) == false);
  REQUIRE(pkb.isRelationTrueForAny(RelationType::PARENT_STAR) == false);
}



  //UsesS only holds Uses relations for Statements
  //UsesP holds for Procedures
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

    pkb.insertRelation(RelationType::USES, "3", "x");
    pkb.insertRelation(RelationType::USES, "4", "y");
    pkb.insertRelation(RelationType::USES, "sub", "x");

    pkb.insertRelation(RelationType::MODIFIES, "main", "x");
    pkb.insertRelation(RelationType::MODIFIES, "5", "x");
    pkb.insertRelation(RelationType::MODIFIES, "7", "y");

    // added USES relation for statement
    REQUIRE(pkb.isRelationTrue("3", "x", RelationType::USES_S));
    // added USES relation for procedure
    REQUIRE(pkb.isRelationTrue("sub", "x", RelationType::USES_P));

    // USES relation for statement should not be in procedure table
    REQUIRE(pkb.isRelationTrue("4", "y", RelationType::USES_P) == false);
    // USES relation for procedure should not be in statement table
    REQUIRE(pkb.isRelationTrue("sub", "x", RelationType::USES_S) == false);

    // added MODIFIES relation for statement
    REQUIRE(pkb.isRelationTrue("5", "x", RelationType::MODIFIES_S));
    // added MODIFIES relation for procedure
    REQUIRE(pkb.isRelationTrue("main", "x", RelationType::MODIFIES_P));


    // MODIFIES relation for statement should not be in procedure table
    REQUIRE(pkb.isRelationTrue("7", "y", RelationType::MODIFIES_P) == false);
    // MODIFIES relation for procedure should not be in statement table
    REQUIRE(pkb.isRelationTrue("main", "x", RelationType::MODIFIES_S) == false);

    std::vector<std::string> empty_vector;

    // Check APIs

    std::vector<std::string> tmp = {"main"};
    // Select p such that Modifies(p, _)
    REQUIRE(*pkb.getRelationValuesGivenFirstType(
            EntityType::PROCEDURE, RelationType::MODIFIES_P) == tmp);
    tmp = {"5","7"};
    // Select s such that Modifies(s, _)
    REQUIRE(*pkb.getRelationValuesGivenFirstType(
            EntityType::STMT, RelationType::MODIFIES_S) == tmp);
    // Query from wrong table
    REQUIRE(*pkb.getRelationValuesGivenFirstType(
            EntityType::STMT, RelationType::USES_P) == empty_vector);

    // Select Uses(pn, v)
    // Statement 3 is a print statement that uses "x". Returns that pair.
    std::vector<std::pair<std::string, std::string>> emptyPair = {};
    std::vector<std::pair<std::string, std::string>> tmp1 = {
        std::make_pair("3", "x")};

    REQUIRE(*pkb.getRelationValues(EntityType::PRINT, EntityType::VARIABLE,
                                   RelationType::USES_S) == tmp1);

    // Modifies(c, v)
    // No procedure call that modifies a variable
    REQUIRE(*pkb.getRelationValues(EntityType::CALL, EntityType::VARIABLE,
                                   RelationType::MODIFIES_S) == emptyPair);


  }