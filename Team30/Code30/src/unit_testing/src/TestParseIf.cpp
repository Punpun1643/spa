#include "../../spa/src/shared/tokenizer/token/EofToken.h"
#include "../../spa/src/shared/tokenizer/token/IntegerToken.h"
#include "../../spa/src/shared/tokenizer/token/SpecialCharToken.h"
#include "../../spa/src/shared/tokenizer/token/Token.h"
#include "../../spa/src/shared/tokenizer/token/WordToken.h"
#include "../../spa/src/source_processor/node/stmt_node/StmtType.h"
#include "../../spa/src/source_processor/parser/SpParser.h"
#include "catch.hpp"

using Catch::Matchers::Contains;

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

  SECTION("Missing open bracket should throw error") {
    /*
      if num1 > num2) then {
        read num1;
        read num2;
        print temp;
      } else {
        call noSwap;
      }
   */

    std::vector<std::shared_ptr<Token>> tokens;

    tokens.push_back(std::make_shared<WordToken>("if"));
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
    REQUIRE_THROWS_WITH(parser.parseIf(), Contains("Invalid if"));
  }

  SECTION(
      "Test valid if stmt with valid nested while stmt should not throw any "
      "error") {
    /*
      if (num1 > num2) then {
        read num1;
        read num2;
        print temp;
      } else {
        call noSwap;
        while (n1 == n2) {
          read n1;
          call readPoint;
          print temp;
        }
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

    tokens.push_back(std::make_shared<WordToken>("while"));
    tokens.push_back(std::make_shared<SpecialCharToken>("("));
    tokens.push_back(std::make_shared<WordToken>("n1"));
    tokens.push_back(std::make_shared<SpecialCharToken>("=="));
    tokens.push_back(std::make_shared<WordToken>("n2"));
    tokens.push_back(std::make_shared<SpecialCharToken>(")"));
    tokens.push_back(std::make_shared<SpecialCharToken>("{"));

    tokens.push_back(std::make_shared<WordToken>("read"));
    tokens.push_back(std::make_shared<WordToken>("n1"));
    tokens.push_back(std::make_shared<SpecialCharToken>(";"));

    tokens.push_back(std::make_shared<WordToken>("call"));
    tokens.push_back(std::make_shared<WordToken>("readPoint"));
    tokens.push_back(std::make_shared<SpecialCharToken>(";"));
    tokens.push_back(std::make_shared<WordToken>("print"));
    tokens.push_back(std::make_shared<WordToken>("temp"));
    tokens.push_back(std::make_shared<SpecialCharToken>(";"));
    tokens.push_back(std::make_shared<SpecialCharToken>("}"));
    tokens.push_back(std::make_shared<SpecialCharToken>("}"));
    tokens.push_back(std::make_shared<EofToken>());

    SpParser parser(tokens);
    REQUIRE_NOTHROW(parser.parseIf());
  }

  SECTION("Test other characters in place of open bracket should throw error") {
    /*
      if bracket num1 > num2) then {
        read num1;
        read num2;
        print temp;
      } else {
        call noSwap;
      }
   */

    std::vector<std::shared_ptr<Token>> tokens;

    tokens.push_back(std::make_shared<WordToken>("if"));
    tokens.push_back(std::make_shared<WordToken>("bracket"));
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
    REQUIRE_THROWS_WITH(parser.parseIf(), Contains("Invalid if"));
  }

  SECTION(
      "Test special character in place of open bracket should throw error") {
    /*
      if && num1 > num2) then {
        read num1;
        read num2;
        print temp;
      } else {
        call noSwap;
      }
   */

    std::vector<std::shared_ptr<Token>> tokens;

    tokens.push_back(std::make_shared<WordToken>("if"));
    tokens.push_back(std::make_shared<SpecialCharToken>("&&"));
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
    REQUIRE_THROWS_WITH(parser.parseIf(), Contains("Invalid if"));
  }

  SECTION("Test missing close bracket should throw error") {
    /*
      if (num1 > num2 then {
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
    REQUIRE_THROWS_WITH(parser.parseIf(),
                        Contains("Invalid"));  // will be invalid condExpr
  }

  SECTION("Test missing then keyword should throw error") {
    /*
      if (num1 > num2 {
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
    REQUIRE_THROWS_WITH(
        parser.parseIf(),
        Contains("Invalid condExpr"));  // will be invalid condExpr
  }

  SECTION("Test missing then keyword should throw error") {
    /*
      if (num1 > num2) {
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
    REQUIRE_THROWS_WITH(parser.parseIf(), Contains("Invalid if"));
  }

  SECTION("Test missing start then curly bracket should throw error") {
    /*
      if (num1 > num2) then
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
    tokens.push_back(std::make_shared<WordToken>("read"));
    tokens.push_back(std::make_shared<WordToken>("num1"));
    tokens.push_back(std::make_shared<SpecialCharToken>(";"));
    tokens.push_back(std::make_shared<WordToken>("read"));
    tokens.push_back(std::make_shared<WordToken>("num2"));
    tokens.push_back(std::make_shared<SpecialCharToken>(";"));
    tokens.push_back(std::make_shared<WordToken>("print"));
    tokens.push_back(std::make_shared<WordToken>("temp"));
    tokens.push_back(std::make_shared<SpecialCharToken>(";"));
    tokens.push_back(std::make_shared<WordToken>("else"));
    tokens.push_back(std::make_shared<SpecialCharToken>("{"));
    tokens.push_back(std::make_shared<WordToken>("call"));
    tokens.push_back(std::make_shared<WordToken>("noSwap"));
    tokens.push_back(std::make_shared<SpecialCharToken>(";"));
    tokens.push_back(std::make_shared<SpecialCharToken>("}"));
    tokens.push_back(std::make_shared<EofToken>());

    SpParser parser(tokens);
    REQUIRE_THROWS_WITH(parser.parseIf(), Contains("Invalid if"));
  }

  SECTION("Test missing end then curly bracket should throw error") {
    /*
      if (num1 > num2) then {
        read num1;
        read num2;
        print temp;
       else {
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
    tokens.push_back(std::make_shared<WordToken>("else"));
    tokens.push_back(std::make_shared<SpecialCharToken>("{"));
    tokens.push_back(std::make_shared<WordToken>("call"));
    tokens.push_back(std::make_shared<WordToken>("noSwap"));
    tokens.push_back(std::make_shared<SpecialCharToken>(";"));
    tokens.push_back(std::make_shared<SpecialCharToken>("}"));
    tokens.push_back(std::make_shared<EofToken>());

    SpParser parser(tokens);
    REQUIRE_THROWS_WITH(parser.parseIf(),
                        Contains("invalid"));  // will be invalid stmtLst
  }

  SECTION("Test missing else keyword should throw error") {
    /*
      if (num1 > num2) then {
        read num1;
        read num2;
        print temp;
      } {
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
    tokens.push_back(std::make_shared<SpecialCharToken>("{"));
    tokens.push_back(std::make_shared<WordToken>("call"));
    tokens.push_back(std::make_shared<WordToken>("noSwap"));
    tokens.push_back(std::make_shared<SpecialCharToken>(";"));
    tokens.push_back(std::make_shared<SpecialCharToken>("}"));
    tokens.push_back(std::make_shared<EofToken>());

    SpParser parser(tokens);
    REQUIRE_THROWS_WITH(parser.parseIf(), Contains("Invalid if"));
  }

  SECTION("Test missing open else curly bracket should throw error") {
    /*
      if (num1 > num2) then {
        read num1;
        read num2;
        print temp;
      } else
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
    tokens.push_back(std::make_shared<WordToken>("call"));
    tokens.push_back(std::make_shared<WordToken>("noSwap"));
    tokens.push_back(std::make_shared<SpecialCharToken>(";"));
    tokens.push_back(std::make_shared<SpecialCharToken>("}"));
    tokens.push_back(std::make_shared<EofToken>());

    SpParser parser(tokens);
    REQUIRE_THROWS_WITH(parser.parseIf(), Contains("Invalid if"));
  }

  SECTION("Test missing close else curly bracket should throw error") {
    /*
      if (num1 > num2) then {
        read num1;
        read num2;
        print temp;
      } else {
        call noSwap;
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
    tokens.push_back(std::make_shared<EofToken>());

    SpParser parser(tokens);
    REQUIRE_THROWS_WITH(parser.parseIf(),
                        Contains("invalid"));  // will be invalid stmtLst
  }
}