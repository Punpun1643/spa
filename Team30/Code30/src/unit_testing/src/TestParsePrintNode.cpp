#include "../../spa/src/shared/tokenizer/token/EofToken.h"
#include "../../spa/src/shared/tokenizer/token/IntegerToken.h"
#include "../../spa/src/shared/tokenizer/token/SpecialCharToken.h"
#include "../../spa/src/shared/tokenizer/token/Token.h"
#include "../../spa/src/shared/tokenizer/token/WordToken.h"
#include "../../spa/src/source_processor/node/stmt_node/StmtType.h"
#include "../../spa/src/source_processor/parser/SpParser.h"
#include "catch.hpp"

TEST_CASE("Test parse print", "[parsePrint]") {
  SECTION("Test valid print stmt should not throw error (i.e. print variable;)") {
    std::vector<std::shared_ptr<Token>> tokens;

    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<WordToken>("variable")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(";")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<EofToken>()));

    SpParser parser = SpParser(tokens);
    REQUIRE_NOTHROW(parser.parsePrint());
  }

  SECTION("Test valid print stmt should have the correct variable name (i.e. print variable;)") {
    std::vector<std::shared_ptr<Token>> tokens;

    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<WordToken>("variable")));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(";")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<EofToken>()));

    SpParser parser = SpParser(tokens);
    std::shared_ptr<PrintNode> printNode = parser.parsePrint();

    REQUIRE(printNode->getVarName() == "variable");
  }

  SECTION("Test invalid print stmt should throw error (i.e. print variable)") {
    std::vector<std::shared_ptr<Token>> tokens;

    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<WordToken>("variable")));
    tokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<EofToken>()));

    SpParser parser = SpParser(tokens);
    REQUIRE_THROWS(parser.parsePrint());
  }
}