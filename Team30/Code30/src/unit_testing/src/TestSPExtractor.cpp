#include <source_processor/design_extractor/IExtractionController.h>
#include <source_processor/node/ProcedureNode.h>
#include <source_processor/node/ProgramNode.h>
#include <source_processor/node/StmtLstNode.h>
#include <source_processor/node/stmt_node/PrintNode.h>

#include <iostream>
#include <source_processor/design_extractor/ExtractionController.cpp>

#include "../../spa/src/program_knowledge_base/PkbApi.h"
#include "ManualASTBuilder.h"
#include "PkbStub.h"
#include "catch.hpp"

class ExtractorBuilder {
 public:
  std::shared_ptr<EntityExtractor> eExtractor;
  std::shared_ptr<FollowsExtractor> fExtractor;
  std::shared_ptr<ParentExtractor> pExtractor;
  std::shared_ptr<ModifiesExtractor> mExtractor;
  std::shared_ptr<UsesExtractor> uExtractor;

  PkbStub pkb;
  ExtractorBuilder(PkbStub& pkb) : pkb(pkb) {
    eExtractor = std::make_shared<EntityExtractor>(pkb);
    fExtractor = std::make_shared<FollowsExtractor>(pkb);
    pExtractor = std::make_shared<ParentExtractor>(pkb);
    mExtractor = std::make_shared<ModifiesExtractor>(pkb);
    uExtractor = std::make_shared<UsesExtractor>(pkb);
  }
};

TEST_CASE("AST 1: Basic SPA, no nesting, while, if") {
  PkbStub pkb = PkbStub();
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
    // NOTE: this expected value includes all the duplicate calls
    // that may occur (handled by pkb)
  }
  SECTION("Modifies extraction functionality") {
    REQUIRE(pkb.insertModifiesCallCount == 11);
  }
}

TEST_CASE("AST 2: Basic SPA, doubly nested while") {
  PkbStub pkb = PkbStub();
  std::shared_ptr<ProgramNode> ast = ManualASTBuilder::getAST_2();
  ExtractorBuilder eb = ExtractorBuilder(pkb);
  ExtractionController ec = ExtractionController(pkb);
  ec.executeProgramExtraction(ast);
  SECTION("Follows extraction functionality") {
    REQUIRE(pkb.insertFollowsCallCount == 1);
  }
  SECTION("Parent extraction functionality") {
    REQUIRE(pkb.insertParentCallCount == 3);
  }
  SECTION("Uses extraction functionality") {
    REQUIRE(pkb.insertUsesCallCount == 9);
    // NOTE: the expected value 15 includes all the duplicate calls
    // that may occur (handled by pkb)
  }
  SECTION("Modifies extraction functionality") {
    REQUIRE(pkb.insertModifiesCallCount == 4);
  }
}

TEST_CASE("AST 3: Basic SPA, 2 procedures") {
  PkbStub pkb = PkbStub();
  std::shared_ptr<ProgramNode> ast = ManualASTBuilder::getAST_3();
  ExtractorBuilder eb = ExtractorBuilder(pkb);
  ExtractionController ec = ExtractionController(pkb);
  ec.executeProgramExtraction(ast);
  SECTION("Follows extraction functionality") {
    REQUIRE(pkb.insertFollowsCallCount == 2);
  }
  SECTION("Parent extraction functionality") {
    REQUIRE(pkb.insertParentCallCount == 0);
  }
  SECTION("Uses extraction functionality") {
    REQUIRE(pkb.insertUsesCallCount == 4);
    // NOTE: the expected value 15 includes all the duplicate calls
    // that may occur (handled by pkb)
  }
  SECTION("Modifies extraction functionality") {
    REQUIRE(pkb.insertModifiesCallCount == 4);
  }
}