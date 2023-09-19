#include <memory>

#include "../../spa/src/shared/tokenizer/token/EofToken.h"
#include "../../spa/src/shared/tokenizer/token/IntegerToken.h"
#include "../../spa/src/shared/tokenizer/token/SpecialCharToken.h"
#include "../../spa/src/shared/tokenizer/token/Token.h"
#include "../../spa/src/shared/tokenizer/token/WordToken.h"
#include "../../spa/src/source_processor/node/util_node/CondExprNode.h"
#include "../../spa/src/source_processor/parser/SpParser.h"
#include "../../spa/src/source_processor/node/stmt_node/AssignNode.h"
#include "catch.hpp"

TEST_CASE("Test parse assign", "[parseAssign]") {
  SECTION(
      "Test valid assign stmt is able to be parsed correctly by parseStmtLst") {
    /*
     * x = 1 + y;
     */
    std::vector<std::shared_ptr<Token>> tokens;

    tokens.push_back(std::make_shared<WordToken>("x"));
    tokens.push_back(std::make_shared<SpecialCharToken>("="));
    tokens.push_back(std::make_shared<IntegerToken>("1"));
    tokens.push_back(std::make_shared<SpecialCharToken>("+"));
    tokens.push_back(std::make_shared<WordToken>("y"));
    tokens.push_back(std::make_shared<SpecialCharToken>(";"));
    tokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("}")));
    tokens.push_back(std::make_shared<EofToken>());

    SpParser parser = SpParser(tokens);
    REQUIRE_NOTHROW(parser.parseStmtLst());
  }

  SECTION(
      "Test valid assign stmt return the correct constants and variables") {
    /*
     * x = 1 + y;
     */
    std::vector<std::shared_ptr<Token>> tokens;

    tokens.push_back(std::make_shared<IntegerToken>("1"));
    tokens.push_back(std::make_shared<SpecialCharToken>("+"));
    tokens.push_back(std::make_shared<WordToken>("y"));
    tokens.push_back(std::make_shared<SpecialCharToken>(";"));
    tokens.push_back(std::make_shared<EofToken>());

    SpParser parser = SpParser(tokens);
    auto assignNode = parser.parseAssign("x");
    REQUIRE(assignNode->getConstants().size() == 1);
    REQUIRE(assignNode->getVariables().size() == 1);
    REQUIRE(assignNode->getVarName() == "x");
  }
}