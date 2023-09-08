#include "catch.hpp"
#include "../../spa/src/source_processor/parser/SpParser.h"
#include "../../spa/src/shared/tokenizer/token/Token.h"
#include "../../spa/src/shared/tokenizer/token/SpecialCharToken.h"
#include "../../spa/src/shared/tokenizer/token/EofToken.h"
#include "../../spa/src/shared/tokenizer/token/WordToken.h"


TEST_CASE("Test parse() method") {
    std::vector<std::shared_ptr<Token>> tokens;

    SECTION("Test parse() method") {
        tokens.push_back(std::static_pointer_cast<Token>(std::make_shared<WordToken>("procedure")));
        tokens.push_back(std::static_pointer_cast<Token>(std::make_shared<WordToken>("computeAverage")));
        tokens.push_back(std::static_pointer_cast<Token>(std::make_shared<SpecialCharToken>("{")));
        tokens.push_back(std::static_pointer_cast<Token>(std::make_shared<WordToken>("read")));
        tokens.push_back(std::static_pointer_cast<Token>(std::make_shared<WordToken>("num1")));
        tokens.push_back(std::static_pointer_cast<Token>(std::make_shared<SpecialCharToken>(";")));
        tokens.push_back(std::static_pointer_cast<Token>(std::make_shared<WordToken>("read")));
        tokens.push_back(std::static_pointer_cast<Token>(std::make_shared<WordToken>("num2")));
        tokens.push_back(std::static_pointer_cast<Token>(std::make_shared<SpecialCharToken>(";")));
        tokens.push_back(std::static_pointer_cast<Token>(std::make_shared<WordToken>("read")));
        tokens.push_back(std::static_pointer_cast<Token>(std::make_shared<WordToken>("num3")));
        tokens.push_back(std::static_pointer_cast<Token>(std::make_shared<SpecialCharToken>(";")));
        tokens.push_back(std::static_pointer_cast<Token>(std::make_shared<WordToken>("print")));
        tokens.push_back(std::static_pointer_cast<Token>(std::make_shared<WordToken>("ave")));
        tokens.push_back(std::static_pointer_cast<Token>(std::make_shared<SpecialCharToken>(";")));
        tokens.push_back(std::static_pointer_cast<Token>(std::make_shared<WordToken>("call")));
        tokens.push_back(std::static_pointer_cast<Token>(std::make_shared<WordToken>("dummy")));
        tokens.push_back(std::static_pointer_cast<Token>(std::make_shared<SpecialCharToken>(";")));
        tokens.push_back(std::static_pointer_cast<Token>(std::make_shared<SpecialCharToken>("}")));
        tokens.push_back(std::static_pointer_cast<Token>(std::make_shared<EofToken>("")));

        SpParser parser = SpParser(tokens);
        parser.parse();
        auto programNode = parser.getSourceProgramNode();

        REQUIRE(programNode != nullptr);
        REQUIRE(programNode->getChildren().size() == 1);
        REQUIRE(programNode->getChildren().at(0)->getProcedureName() == "computeAverage");

        auto procedureNode = programNode->getChildren().at(0);
        REQUIRE(procedureNode->getChildren().size() == 1);

        auto stmtListNode = procedureNode->getChildren().at(0);
        REQUIRE(stmtListNode->getChildren().size() == 5);

        auto readNode1 = std::dynamic_pointer_cast<ReadNode>(stmtListNode->getChildren().at(0));
        REQUIRE(readNode1->getVarName() == "num1");

        auto readNode2 = std::dynamic_pointer_cast<ReadNode>(stmtListNode->getChildren().at(1));
        REQUIRE(readNode2->getVarName() == "num2");

        auto readNode3 = std::dynamic_pointer_cast<ReadNode>(stmtListNode->getChildren().at(2));
        REQUIRE(readNode3->getVarName() == "num3");

        auto printNode = std::dynamic_pointer_cast<PrintNode>(stmtListNode->getChildren().at(3));
        REQUIRE(printNode->getVarName() == "ave");

        auto callNode = std::dynamic_pointer_cast<CallNode>(stmtListNode->getChildren().at(4));
        REQUIRE(callNode->getProcName() == "dummy");
    }
}
