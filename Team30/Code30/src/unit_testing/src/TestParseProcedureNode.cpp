#include "catch.hpp"
#include "../../spa/src/source_processor/parser/SpParser.h"
#include "../../spa/src/shared/tokenizer/token/Token.h"
#include "../../spa/src/shared/tokenizer/token/SpecialCharToken.h"
#include "../../spa/src/shared/tokenizer/token/EofToken.h"
#include "../../spa/src/shared/tokenizer/token/WordToken.h"


TEST_CASE("Parser parseProcedure") {
    std::vector<std::shared_ptr<Token>> tokens;


    SECTION("Invalid procedure with no statements throws exception") {
        tokens.push_back(std::static_pointer_cast<Token>(std::make_shared<SpecialCharToken>("}")));

        SpParser parser = SpParser(tokens);
        REQUIRE_THROWS_AS(parser.parseProcedure(), std::invalid_argument);
    }

    SECTION("Valid procedure with one statement has one statement node") {
        tokens.push_back(std::static_pointer_cast<Token>(std::make_shared<WordToken>("main")));
        tokens.push_back(std::static_pointer_cast<Token>(std::make_shared<SpecialCharToken>("{")));
        tokens.push_back(std::static_pointer_cast<Token>(std::make_shared<WordToken>("flag")));
        tokens.push_back(std::static_pointer_cast<Token>(std::make_shared<SpecialCharToken>("}")));

        SpParser parser = SpParser(tokens);
        auto procedureNode = parser.parseProcedure();

        REQUIRE(procedureNode != nullptr);
        REQUIRE(procedureNode->procedureName == "main");
        REQUIRE(procedureNode->getChildren().at(0) != nullptr);
    }
}

