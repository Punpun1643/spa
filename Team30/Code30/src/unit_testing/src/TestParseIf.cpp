#include "../../spa/src/shared/tokenizer/token/EofToken.h"
#include "../../spa/src/shared/tokenizer/token/IntegerToken.h"
#include "../../spa/src/shared/tokenizer/token/SpecialCharToken.h"
#include "../../spa/src/shared/tokenizer/token/Token.h"
#include "../../spa/src/shared/tokenizer/token/WordToken.h"
#include "../../spa/src/source_processor/node/stmt_node/StmtType.h"
#include "../../spa/src/source_processor/parser/SpParser.h"
#include "catch.hpp"

TEST_CASE("Test parseIf", "[parseIf]") {
  SECTION("Test valid if stmt should not throw any error") {
    /*
      if (num1 > num2) then {
        read num1;
        read num2;
        print temp;
      } else {
        call noSwap;
      }
   */

    std::vector<std::shared_ptr<Token>> tokens;


    tokens.push_back(std::make_shared<SpecialCharToken>("("));
    tokens.push_back(std::make_shared<WordToken>("num1"));
    tokens.push_back(std::make_shared<SpecialCharToken>(">"));
    tokens.push_back(std::make_shared<WordToken>("num2"));
    tokens.push_back(std::make_shared<SpecialCharToken>(")"));
    tokens.push_back(std::make_shared<WordToken>("then"));
    tokens.push_back(std::make_shared<SpecialCharToken>("{"));
    tokens.push_back(std::make_shared<WordToken>("read"));
    tokens.push_back(std::make_shared<WordToken>("num1"));
    tokens.push_back(std::make_shared<SpecialCharToken>(";"));
    tokens.push_back(std::make_shared<WordToken>("read"));
    tokens.push_back(std::make_shared<WordToken>("num2"));
    tokens.push_back(std::make_shared<SpecialCharToken>(";"));
    tokens.push_back(std::make_shared<WordToken>("print"));
    tokens.push_back(std::make_shared<WordToken>("temp"));
    tokens.push_back(std::make_shared<SpecialCharToken>(";"));
    tokens.push_back(std::make_shared<SpecialCharToken>("}"));
    tokens.push_back(std::make_shared<WordToken>("else"));
    tokens.push_back(std::make_shared<SpecialCharToken>("{"));
    tokens.push_back(std::make_shared<WordToken>("call"));
    tokens.push_back(std::make_shared<WordToken>("noSwap"));
    tokens.push_back(std::make_shared<SpecialCharToken>(";"));
    tokens.push_back(std::make_shared<SpecialCharToken>("}"));
    tokens.push_back(std::make_shared<EofToken>());

        SpParser parser(tokens);
        REQUIRE_NOTHROW(parser.parseIf());
  }
}