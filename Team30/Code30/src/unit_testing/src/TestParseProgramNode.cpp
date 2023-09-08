#include "catch.hpp"
#include "../../spa/src/source_processor/parser/SpParser.h"
#include "../../spa/src/shared/tokenizer/token/Token.h"
#include "../../spa/src/shared/tokenizer/token/WordToken.h"
#include "../../spa/src/shared/tokenizer/token/SpecialCharToken.h"
#include "../../spa/src/shared/tokenizer/token/EofToken.h"

TEST_CASE("Parser parseProgram") {
    std::vector<std::shared_ptr<Token>> tokens;

    SECTION("Invalid program with no procedures throws exception") {
        tokens.push_back(std::static_pointer_cast<Token>(std::make_shared<WordToken>("not_procedure")));
        tokens.push_back(std::static_pointer_cast<Token>(std::make_shared<WordToken>("main")));
        tokens.push_back(std::static_pointer_cast<Token>(std::make_shared<EofToken>("")));

        SpParser parser = SpParser(tokens);
        REQUIRE_THROWS_AS(parser.parseProgram(), std::invalid_argument);
    }

    SECTION("Valid program with one procedure has one procedure node") {
        tokens.push_back(std::static_pointer_cast<Token>(std::make_shared<WordToken>("procedure")));
        tokens.push_back(std::static_pointer_cast<Token>(std::make_shared<WordToken>("main")));
        tokens.push_back(std::static_pointer_cast<Token>(std::make_shared<SpecialCharToken>("{")));
        tokens.push_back(std::static_pointer_cast<Token>(std::make_shared<WordToken>("print")));
        tokens.push_back(std::static_pointer_cast<Token>(std::make_shared<WordToken>("variable")));
        tokens.push_back(std::static_pointer_cast<Token>(std::make_shared<SpecialCharToken>(";")));
        tokens.push_back(std::static_pointer_cast<Token>(std::make_shared<SpecialCharToken>("}")));
        tokens.push_back(std::static_pointer_cast<Token>(std::make_shared<EofToken>("")));

        SpParser parser = SpParser(tokens);
        auto programNode = parser.parseProgram();

        REQUIRE(programNode != nullptr);
        REQUIRE(programNode->getChildren().size() == 1);
        REQUIRE(programNode->getChildren().at(0)->getProcedureName() == "main");
    }

    SECTION("Valid program with 2 procedures") {
        tokens.push_back(std::static_pointer_cast<Token>(std::make_shared<WordToken>("procedure")));
        tokens.push_back(std::static_pointer_cast<Token>(std::make_shared<WordToken>("main")));
        tokens.push_back(std::static_pointer_cast<Token>(std::make_shared<SpecialCharToken>("{")));
        tokens.push_back(std::static_pointer_cast<Token>(std::make_shared<WordToken>("print")));
        tokens.push_back(std::static_pointer_cast<Token>(std::make_shared<WordToken>("variable")));
        tokens.push_back(std::static_pointer_cast<Token>(std::make_shared<SpecialCharToken>(";")));
        tokens.push_back(std::static_pointer_cast<Token>(std::make_shared<SpecialCharToken>("}")));
        tokens.push_back(std::static_pointer_cast<Token>(std::make_shared<WordToken>("procedure")));
        tokens.push_back(std::static_pointer_cast<Token>(std::make_shared<WordToken>("proc")));
        tokens.push_back(std::static_pointer_cast<Token>(std::make_shared<SpecialCharToken>("{")));
        tokens.push_back(std::static_pointer_cast<Token>(std::make_shared<WordToken>("print")));
        tokens.push_back(std::static_pointer_cast<Token>(std::make_shared<WordToken>("variable")));
        tokens.push_back(std::static_pointer_cast<Token>(std::make_shared<SpecialCharToken>(";")));
        tokens.push_back(std::static_pointer_cast<Token>(std::make_shared<SpecialCharToken>("}")));
        tokens.push_back(std::static_pointer_cast<Token>(std::make_shared<EofToken>("")));

        SpParser parser = SpParser(tokens);
        auto programNode = parser.parseProgram();

        REQUIRE(programNode != nullptr);
        REQUIRE(programNode->getChildren().size() == 2);
        REQUIRE(programNode->getChildren().at(0)->getProcedureName() == "main");
        REQUIRE(programNode->getChildren().at(1)->getProcedureName() == "proc");
    }

    SECTION("Invalid program with one valid procedure and one invalid procedure") {
        tokens.push_back(std::static_pointer_cast<Token>(std::make_shared<WordToken>("procedure")));
        tokens.push_back(std::static_pointer_cast<Token>(std::make_shared<WordToken>("main")));
        tokens.push_back(std::static_pointer_cast<Token>(std::make_shared<SpecialCharToken>("{")));
        tokens.push_back(std::static_pointer_cast<Token>(std::make_shared<WordToken>("print")));
        tokens.push_back(std::static_pointer_cast<Token>(std::make_shared<WordToken>("variable")));
        tokens.push_back(std::static_pointer_cast<Token>(std::make_shared<SpecialCharToken>(";")));
        tokens.push_back(std::static_pointer_cast<Token>(std::make_shared<SpecialCharToken>("}")));
        tokens.push_back(std::static_pointer_cast<Token>(std::make_shared<WordToken>("not_procedure")));
        tokens.push_back(std::static_pointer_cast<Token>(std::make_shared<WordToken>("proc")));
        tokens.push_back(std::static_pointer_cast<Token>(std::make_shared<SpecialCharToken>("{")));
        tokens.push_back(std::static_pointer_cast<Token>(std::make_shared<WordToken>("print")));
        tokens.push_back(std::static_pointer_cast<Token>(std::make_shared<SpecialCharToken>("}")));
        tokens.push_back(std::static_pointer_cast<Token>(std::make_shared<EofToken>("")));

        SpParser parser = SpParser(tokens);
        REQUIRE_THROWS_AS(parser.parseProgram(), std::invalid_argument);
    }
}

