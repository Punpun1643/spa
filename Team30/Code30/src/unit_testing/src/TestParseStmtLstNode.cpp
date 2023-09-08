#include "catch.hpp"
#include "../../spa/src/source_processor/parser/SpParser.h"
#include "../../spa/src/shared/tokenizer/token/Token.h"
#include "../../spa/src/shared/tokenizer/token/SpecialCharToken.h"
#include "../../spa/src/shared/tokenizer/token/EofToken.h"
#include "../../spa/src/shared/tokenizer/token/WordToken.h"
#include "../../spa/src/source_processor/node/stmt_node/PrintNode.h"

TEST_CASE("Parser parseStmtLst") {
    std::vector<std::shared_ptr<Token>> tokens;

    SECTION("StmtLst with no statement return empty vector") {
        tokens.push_back(std::static_pointer_cast<Token>(std::make_shared<SpecialCharToken>("}")));

        SpParser parser = SpParser(tokens);
        REQUIRE(parser.parseStmtLst()->getChildren().empty());
    }

    SECTION("StmtLst with one statement return one statement node") {
        tokens.push_back(std::static_pointer_cast<Token>(std::make_shared<WordToken>("print")));
        tokens.push_back(std::static_pointer_cast<Token>(std::make_shared<WordToken>("variable")));
        tokens.push_back(std::static_pointer_cast<Token>(std::make_shared<SpecialCharToken>(";")));
        tokens.push_back(std::static_pointer_cast<Token>(std::make_shared<SpecialCharToken>("}")));

        SpParser parser = SpParser(tokens);
        auto stmtLstNode = parser.parseStmtLst();
        auto printNode = std::dynamic_pointer_cast<PrintNode>(stmtLstNode->getChildren().at(0));

        REQUIRE(stmtLstNode != nullptr);
        REQUIRE(stmtLstNode->getChildren().size() == 1);
        REQUIRE(printNode != nullptr);
        REQUIRE(printNode->varName == "variable");
    }
}