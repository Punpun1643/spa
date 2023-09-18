#include <source_processor/design_extractor/IExtractionController.h>
#include <source_processor/node/ProcedureNode.h>
#include <source_processor/node/ProgramNode.h>
#include <source_processor/node/StmtLstNode.h>
#include <source_processor/node/stmt_node/PrintNode.h>

#include <iostream>
#include <source_processor/design_extractor/ExtractionController.cpp>

#include "../../spa/src/program_knowledge_base/PkbApi.h"
#include "ManualASTBuilder.h"
#include "catch.hpp"

class PkbApiStub : public PkbApi {
 public:
  int insertEntityCallCount;
  int insertRelationCallCount;
  int insertFollowsCallCount;
  int insertParentCallCount;
  int insertUsesCallCount;
  int insertModifiesCallCount;

  PkbApiStub()
      : insertEntityCallCount(0),
        insertRelationCallCount(0),
        insertFollowsCallCount(0),
        insertParentCallCount(0),
        insertUsesCallCount(0),
        insertModifiesCallCount(0) {}

  void insertEntity(EntityType type, std::string entity) override {
    ++insertEntityCallCount;
  }

  void insertRelationCommon(RelationType type) {
    ++insertRelationCallCount;
    switch (type) {
      case (RelationType::FOLLOWS):
        ++insertFollowsCallCount;
        break;
      case (RelationType::PARENT):
        ++insertParentCallCount;
        break;
      case (RelationType::USES):
        ++insertUsesCallCount;
        break;
      case (RelationType::MODIFIES):
        ++insertModifiesCallCount;
        break;
      default:
        break;
    }
  }

  // Relation (integer, integer)
  void insertRelation(RelationType rel_type, std::string s1_line_num,
                      std::string s2_line_num) override {
    insertRelationCommon(rel_type);
  }

  // Relation (integer, EntityType)
  void insertRelation(RelationType rel_type, std::string s_line_num,
                      EntityType ent_type, std::string enity) override {
    insertRelationCommon(rel_type);
  }

  // Relation (EntityType, integer)
  void insertRelation(RelationType rel_type, EntityType ent_type,
                      std::string entity, std::string s_line_num) override {
    insertRelationCommon(rel_type);
  }

  // Relation (EntityType, EntityType)
  void insertRelation(RelationType rel_type, EntityType ent_type1,
                      std::string entity1, EntityType ent_type2,
                      std::string entity2) override {
    insertRelationCommon(rel_type);
  }

  std::unique_ptr<std::vector<std::string>> getEntitiesWithType(
      EntityType type) override {
    std::unique_ptr<std::vector<std::string>> output =
        std::make_unique<std::vector<std::string>>();
    return output;
  }

  bool isRelationTrue(std::string value_1, std::string value_2,
                      RelationType rel_type) {
    return true;
  }
  bool isRelationTrueGivenFirstValue(std::string value, RelationType rel_type) {
    return false;
  }
  bool isRelationTrueGivenSecondValue(std::string value,
                                      RelationType rel_type) {
    return true;
  }
  bool isRelationTrueForAny(RelationType relation_type) { return false; }

  std::unique_ptr<std::vector<std::string>> getRelationValuesGivenFirstType(
      EntityType entity_type, RelationType rel_type) {
    return std::make_unique<std::vector<std::string>>();  // empty
  }
  std::unique_ptr<std::vector<std::string>> getRelationValuesGivenSecondType(
      EntityType entity_type, RelationType rel_type) {
    std::vector<std::string> vec = {"1", "3", "5", "7", "9"};
    return std::make_unique<std::vector<std::string>>(vec);
  }

  std::unique_ptr<std::vector<std::string>> getRelationValues(
      EntityType entity_type, std::string value, RelationType rel_type) {
    std::vector<std::string> vec = {"2", "4", "6", "8", "10"};
    return std::make_unique<std::vector<std::string>>(vec);
  }
  std::unique_ptr<std::vector<std::string>> getRelationValues(
      std::string value, EntityType entity_type, RelationType rel_type) {
    return std::make_unique<std::vector<std::string>>();  // empty
  }

  std::unique_ptr<std::vector<std::pair<std::string, std::string>>>
  getRelationValues(EntityType entity_type_1, EntityType entity_type_2,
                    RelationType rel_type) {
    if (entity_type_1 == STMT && entity_type_2 == STMT) {
      auto result =
          std::make_unique<std::vector<std::pair<std::string, std::string>>>();
      result->push_back(std::make_pair("5", "10"));
      result->push_back(std::make_pair("9", "1"));
      result->push_back(std::make_pair("2", "2"));
      result->push_back(std::make_pair("2", "1"));
      return result;
    } else {
      return std::make_unique<
          std::vector<std::pair<std::string, std::string>>>();
    }
  }
};

class ExtractorBuilder {
 public:
  std::shared_ptr<EntityExtractor> eExtractor;
  std::shared_ptr<FollowsExtractor> fExtractor;
  std::shared_ptr<ParentExtractor> pExtractor;
  std::shared_ptr<ModifiesExtractor> mExtractor;
  std::shared_ptr<UsesExtractor> uExtractor;

  PkbApiStub pkb;
  ExtractorBuilder(PkbApiStub& pkb) : pkb(pkb) {
    eExtractor = std::make_shared<EntityExtractor>(pkb);
    fExtractor = std::make_shared<FollowsExtractor>(pkb);
    pExtractor = std::make_shared<ParentExtractor>(pkb);
    mExtractor = std::make_shared<ModifiesExtractor>(pkb);
    uExtractor = std::make_shared<UsesExtractor>(pkb);
  }
};

TEST_CASE("AST 1: Basic SPA, no nesting, while, if") {
  PkbApiStub pkb = PkbApiStub();
  std::shared_ptr<ProgramNode> ast = ManualASTBuilder::getAST_1();
  ExtractorBuilder eb = ExtractorBuilder(pkb);
  ExtractionController ec = ExtractionController(pkb);
  ec.executeProgramExtraction(ast);
  SECTION("Follows extraction functionality") {
    REQUIRE(pkb.insertFollowsCallCount == 7);
  }
  SECTION("Parent extraction functionality") {
    REQUIRE(pkb.insertParentCallCount == 6);
  }
  SECTION("Uses extraction functionality") {
    REQUIRE(pkb.insertUsesCallCount == 15);
    // NOTE: the expected value 15 includes all the duplicate calls
    // that may occur (handled by pkb)
  }
}

TEST_CASE("AST 2: Basic SPA, doubly nested while") {
  PkbApiStub pkb = PkbApiStub();
  std::shared_ptr<ProgramNode> ast = ManualASTBuilder::getAST_2();
  ExtractorBuilder eb = ExtractorBuilder(pkb);
  ExtractionController ec = ExtractionController(pkb);
  ec.executeProgramExtraction(ast);
  SECTION("Follows extraction functionality") {
    REQUIRE(pkb.insertFollowsCallCount == 0);
  }
  SECTION("Parent extraction functionality") {
    REQUIRE(pkb.insertParentCallCount == 2);
  }
  SECTION("Uses extraction functionality") {
    REQUIRE(pkb.insertUsesCallCount == 9);
    // NOTE: the expected value 15 includes all the duplicate calls
    // that may occur (handled by pkb)
  }
}

//TEST_CASE("Follows extraction") {
//  PkbApiStub pkb = PkbApiStub();
//  ExtractorBuilder eb = ExtractorBuilder(pkb);
//  ExtractionController ec = ExtractionController(pkb);
//  SECTION("Full extraction") {
//    ec.executeProgramExtraction(ManualASTBuilder::getAST_1());
//    REQUIRE(pkb.insertFollowsCallCount == 7);
//  }
//}
//
//TEST_CASE("Parent extraction") {
//  PkbApiStub pkb = PkbApiStub();
//  ExtractorBuilder eb = ExtractorBuilder(pkb);
//  ExtractionController ec = ExtractionController(pkb);
//  SECTION("Full extraction") {
//    ec.executeProgramExtraction(ManualASTBuilder::getAST_1());
//    REQUIRE(pkb.insertParentCallCount == 6);
//  }
//}
//
//TEST_CASE("Uses extraction") {
//  PkbApiStub pkb = PkbApiStub();
//  ExtractorBuilder eb = ExtractorBuilder(pkb);
//  ExtractionController ec = ExtractionController(pkb);
//  SECTION("Full extraction") {
//    ec.executeProgramExtraction(ManualASTBuilder::getAST_1());
//    REQUIRE(pkb.insertUsesCallCount == 15);
//    // NOTE: the expected value 15 includes all the duplicate calls
//    // that may occur (handled by pkb)
//  }
//}