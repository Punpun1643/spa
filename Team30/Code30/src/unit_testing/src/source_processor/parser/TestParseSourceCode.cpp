#include "../../../../spa/src/shared/tokenizer/token/EofToken.h"
#include "../../../../spa/src/shared/tokenizer/token/IntegerToken.h"
#include "../../../../spa/src/shared/tokenizer/token/SpecialCharToken.h"
#include "../../../../spa/src/shared/tokenizer/token/Token.h"
#include "../../../../spa/src/shared/tokenizer/token/WordToken.h"
#include "../../../../spa/src/source_processor/parser/SpParser.h"
#include "catch.hpp"

TEST_CASE("Test parse() method") {
  std::vector<std::shared_ptr<Token>> tokens;

  SECTION("Test parse() method") {
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<WordToken>("procedure")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<WordToken>("computeAverage")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("{")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("read")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("num1")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(";")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("read")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("num2")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(";")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("read")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("num3")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(";")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("print")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("ave")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(";")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("call")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("dummy")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(";")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("}")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<EofToken>()));

    SpParser parser = SpParser(tokens);
    parser.Parse();
    auto programNode = parser.GetSourceProgramNode();

    REQUIRE(programNode != nullptr);
    REQUIRE(programNode->GetChildren().size() == 1);
    REQUIRE(programNode->GetChildren().at(0)->GetProcedureName() ==
            "computeAverage");

    auto procedureNode = programNode->GetChildren().at(0);
    REQUIRE(procedureNode->GetChildren().size() == 1);

    auto stmtListNode = procedureNode->GetChildren().at(0);
    REQUIRE(stmtListNode->GetChildren().size() == 5);

    auto readNode1 =
        std::dynamic_pointer_cast<ReadNode>(stmtListNode->GetChildren().at(0));
    REQUIRE(readNode1->GetVarName() == "num1");

    auto readNode2 =
        std::dynamic_pointer_cast<ReadNode>(stmtListNode->GetChildren().at(1));
    REQUIRE(readNode2->GetVarName() == "num2");

    auto readNode3 =
        std::dynamic_pointer_cast<ReadNode>(stmtListNode->GetChildren().at(2));
    REQUIRE(readNode3->GetVarName() == "num3");

    auto printNode =
        std::dynamic_pointer_cast<PrintNode>(stmtListNode->GetChildren().at(3));
    REQUIRE(printNode->GetVarName() == "ave");

    auto callNode =
        std::dynamic_pointer_cast<CallNode>(stmtListNode->GetChildren().at(4));
    REQUIRE(callNode->GetProcName() == "dummy");
  }

  SECTION("Test parse() method with while") {
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<WordToken>("procedure")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<WordToken>("computeAverage")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("{")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("read")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("num1")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(";")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("read")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("num2")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(";")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("read")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("num3")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(";")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("print")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("ave")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(";")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("call")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("dummy")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(";")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("while")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("(")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("(")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("x")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("!=")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<IntegerToken>("0")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(")")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("&&")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("(")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("y")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("!=")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<IntegerToken>("0")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(")")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(")")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("{")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("call")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<WordToken>("readPoint")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(";")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("}")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("}")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<EofToken>()));

    SpParser parser = SpParser(tokens);
    parser.Parse();
    auto programNode = parser.GetSourceProgramNode();

    REQUIRE(programNode != nullptr);
    REQUIRE(programNode->GetChildren().size() == 1);
    REQUIRE(programNode->GetChildren().at(0)->GetProcedureName() ==
            "computeAverage");

    auto procedureNode = programNode->GetChildren().at(0);
    REQUIRE(procedureNode->GetChildren().size() == 1);

    auto stmtListNode = procedureNode->GetChildren().at(0);
    REQUIRE(stmtListNode->GetChildren().size() == 6);

    auto readNode1 =
        std::dynamic_pointer_cast<ReadNode>(stmtListNode->GetChildren().at(0));
    REQUIRE(readNode1->GetVarName() == "num1");

    auto readNode2 =
        std::dynamic_pointer_cast<ReadNode>(stmtListNode->GetChildren().at(1));
    REQUIRE(readNode2->GetVarName() == "num2");

    auto readNode3 =
        std::dynamic_pointer_cast<ReadNode>(stmtListNode->GetChildren().at(2));
    REQUIRE(readNode3->GetVarName() == "num3");

    auto printNode =
        std::dynamic_pointer_cast<PrintNode>(stmtListNode->GetChildren().at(3));
    REQUIRE(printNode->GetVarName() == "ave");

    auto callNode =
        std::dynamic_pointer_cast<CallNode>(stmtListNode->GetChildren().at(4));
    REQUIRE(callNode->GetProcName() == "dummy");

    auto whileNode =
        std::dynamic_pointer_cast<WhileNode>(stmtListNode->GetChildren().at(5));

    auto whileStmtLstNode = whileNode->GetStmtLst();

    REQUIRE(whileStmtLstNode->GetChildren().size() == 1);
  }
}
