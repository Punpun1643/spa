#include "catch.hpp"
#include "../../spa/src/source_processor/parser/SpParser.h"
#include "../../spa/src/shared/tokenizer/token/Token.h"
#include "../../spa/src/shared/tokenizer/token/WordToken.h"
#include "../../spa/src/shared/tokenizer/token/SpecialCharToken.h"
#include "../../spa/src/shared/tokenizer/token/EofToken.h"

TEST_CASE("Parser parseProgram") {
    std::vector<std::shared_ptr<Token>> tokens;

    SECTION("Valid program with one procedure has one procedure node") {
        tokens.push_back(std::static_pointer_cast<Token>(std::make_shared<WordToken>("procedure")));
        tokens.push_back(std::static_pointer_cast<Token>(std::make_shared<WordToken>("main")));
        tokens.push_back(std::static_pointer_cast<Token>(std::make_shared<SpecialCharToken>("{")));
        tokens.push_back(std::static_pointer_cast<Token>(std::make_shared<WordToken>("dummy")));
        tokens.push_back(std::static_pointer_cast<Token>(std::make_shared<SpecialCharToken>("}")));
        tokens.push_back(std::static_pointer_cast<Token>(std::make_shared<EofToken>("")));

        SpParser parser = SpParser(tokens);
        auto programNode = parser.parseProgram();

        REQUIRE(programNode != nullptr);
        REQUIRE(programNode->getChildren().size() == 1);
        REQUIRE(programNode->getChildren().at(0)->procedureName == "main");
    }

    SECTION("Invalid program with no procedures throws exception") {
        tokens.push_back(std::static_pointer_cast<Token>(std::make_shared<WordToken>("not_procedure")));
        tokens.push_back(std::static_pointer_cast<Token>(std::make_shared<WordToken>("main")));
        tokens.push_back(std::static_pointer_cast<Token>(std::make_shared<EofToken>("")));

        SpParser parser = SpParser(tokens);
        REQUIRE_THROWS_AS(parser.parseProgram(), std::invalid_argument);
    }
}

