#include <string>

#include "../../spa/src/source_processor/node/stmt_node/CallNode.h"
#include "../../spa/src/source_processor/node/stmt_node/PrintNode.h"
#include "../../spa/src/source_processor/node/stmt_node/ReadNode.h"
#include "../../spa/src/source_processor/node/stmt_node/StmtType.h"
#include "../../spa/src/source_processor/parser/SpParserManager.h"
#include "catch.hpp"

TEST_CASE(
    "Test SpParserManager parses short length source program successfully",
    "[parseInputFile, Sample_source_w4_short.txt]") {
  SECTION("Test file with valid path and valid source code") {
    SpParserManager spParserManager = SpParserManager();
    std::string filePath =
        "/Users/papattaradaapithanangsiri/23s1-cp-spa-team-30/Team30/Tests30/"
        "Sample_source_w4_short.txt";
    std::shared_ptr<ProgramNode> programNode =
        spParserManager.parseInputFile(filePath);
    std::shared_ptr<ProcedureNode> procedureNode =
        programNode->getChildren().at(0);
    std::shared_ptr<StmtLstNode> stmtLstNode =
        procedureNode->getChildren().at(0);
    std::vector<std::shared_ptr<StmtNode>> stmtNodes =
        stmtLstNode->getChildren();
    auto callNode = std::dynamic_pointer_cast<CallNode>(stmtNodes.at(0));
    auto readNode = std::dynamic_pointer_cast<ReadNode>(stmtNodes.at(1));
    auto printNode = std::dynamic_pointer_cast<PrintNode>(stmtNodes.at(2));

    REQUIRE(programNode->getChildren().size() == 1);
    REQUIRE(procedureNode->getProcedureName() == "Example");
    REQUIRE(stmtLstNode->getChildren().size() == 3);
    REQUIRE(stmtNodes.at(0)->getStmtType() == StmtType::CALL_STMT);
    REQUIRE(stmtNodes.at(1)->getStmtType() == StmtType::READ_STMT);
    REQUIRE(stmtNodes.at(2)->getStmtType() == StmtType::PRINT_STMT);
    REQUIRE(callNode->getProcName() == "c");
    REQUIRE(readNode->getVarName() == "r");
    REQUIRE(printNode->getVarName() == "p");
  }
}

TEST_CASE(
    "Test SpParserManager parses medium length source program successfully",
    "[parseInputFile, Sample_source_w4_medium.txt]") {
  SECTION(
      "Test file with valid path and valid source code for Example 1 and 2") {
    SpParserManager spParserManager = SpParserManager();
    std::string filePath =
        "/Users/papattaradaapithanangsiri/23s1-cp-spa-team-30/Team30/Tests30/"
        "Sample_source_w4_medium.txt";
    auto programNode = spParserManager.parseInputFile(filePath);

    REQUIRE(programNode->getChildren().size() == 3);

    // Test details for procedure Example 1
    {
      auto procedureNode = programNode->getChildren().at(0);
      auto stmtLstNode = procedureNode->getChildren().at(0);
      auto stmtNodes = stmtLstNode->getChildren();

      auto callNode = std::dynamic_pointer_cast<CallNode>(stmtNodes.at(0));
      auto readNode = std::dynamic_pointer_cast<ReadNode>(stmtNodes.at(1));
      auto printNode = std::dynamic_pointer_cast<PrintNode>(stmtNodes.at(2));

      REQUIRE(procedureNode->getProcedureName() == "Example1");
      REQUIRE(stmtLstNode->getChildren().size() == 3);
      REQUIRE(callNode->getProcName() == "Function1");
      REQUIRE(readNode->getVarName() == "userInput1");
      REQUIRE(printNode->getVarName() == "output1");
    }

    // Test details for procedure Example 2
    {
      auto procedureNode = programNode->getChildren().at(1);
      auto stmtLstNode = procedureNode->getChildren().at(0);
      auto stmtNodes = stmtLstNode->getChildren();

      auto callNode1 = std::dynamic_pointer_cast<CallNode>(stmtNodes.at(0));
      auto readNode = std::dynamic_pointer_cast<ReadNode>(stmtNodes.at(1));
      auto printNode = std::dynamic_pointer_cast<PrintNode>(stmtNodes.at(2));
      auto callNode2 = std::dynamic_pointer_cast<CallNode>(stmtNodes.at(3));

      REQUIRE(procedureNode->getProcedureName() == "Example2");
      REQUIRE(stmtLstNode->getChildren().size() == 4);
      REQUIRE(callNode1->getProcName() == "ExternalProc");
      REQUIRE(readNode->getVarName() == "configValue");
      REQUIRE(printNode->getVarName() == "statusMessage");
      REQUIRE(callNode2->getProcName() == "Cleanup");
    }

    // Test details for procedure Example 3
    {
      auto procedureNode = programNode->getChildren().at(2);
      auto stmtLstNode = procedureNode->getChildren().at(0);
      auto stmtNodes = stmtLstNode->getChildren();

      auto callNode1 = std::dynamic_pointer_cast<CallNode>(stmtNodes.at(0));
      auto callNode2 = std::dynamic_pointer_cast<CallNode>(stmtNodes.at(1));
      auto readNode1 = std::dynamic_pointer_cast<ReadNode>(stmtNodes.at(2));
      auto readNode2 = std::dynamic_pointer_cast<ReadNode>(stmtNodes.at(3));
      auto printNode1 = std::dynamic_pointer_cast<PrintNode>(stmtNodes.at(4));
      auto printNode2 = std::dynamic_pointer_cast<PrintNode>(stmtNodes.at(5));

      REQUIRE(procedureNode->getProcedureName() == "Example3");
      REQUIRE(stmtLstNode->getChildren().size() == 6);
      REQUIRE(callNode1->getProcName() == "Initialize");
      REQUIRE(callNode2->getProcName() == "Configure");
      REQUIRE(readNode1->getVarName() == "configA");
      REQUIRE(readNode2->getVarName() == "configB");
      REQUIRE(printNode1->getVarName() == "reportA");
      REQUIRE(printNode2->getVarName() == "reportB");
    }
  }
}
