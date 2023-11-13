#include "../../../../spa/src/shared/parser/AParser.h"
#include "../../../../spa/src/shared/tokenizer/token/EofToken.h"
#include "../../../../spa/src/shared/tokenizer/token/IntegerToken.h"
#include "../../../../spa/src/shared/tokenizer/token/SpecialCharToken.h"
#include "../../../../spa/src/shared/tokenizer/token/Token.h"
#include "../../../../spa/src/shared/tokenizer/token/WordToken.h"
#include "../../../../spa/src/source_processor/parser/SpParser.h"
#include "catch.hpp"

namespace TestConvertInfixToPostfix {
class TestableParser : public AParser {
 public:
  TestableParser(std::vector<std::shared_ptr<Token>> tokens)
      : AParser(tokens) {}

  using AParser::ConvertInfixToPostfix;

  void Parse() override {}
};
}  // namespace TestConvertInfixToPostfix

TEST_CASE("Test Convert infix expression to postfix expression",
          "[ConvertInfixToPostfix]") {
  SECTION(
      "Test infix expression should correctly convert to postfix expression "
      "i.e. v") {
    std::vector<std::shared_ptr<Token>> infixTokens;

    infixTokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("v")));

    std::shared_ptr<TestConvertInfixToPostfix::TestableParser> parser =
        std::make_shared<TestConvertInfixToPostfix::TestableParser>(
            infixTokens);

    std::queue<std::shared_ptr<std::string>> postFixQueue =
        parser->ConvertInfixToPostfix(infixTokens);

    std::shared_ptr<std::string> first = postFixQueue.front();
    postFixQueue.pop();

    REQUIRE(*first == "v");
  }

  SECTION(
      "Test infix expression should correctly convert to postfix expression "
      "i.e. x + y") {
    std::vector<std::shared_ptr<Token>> infixTokens;

    infixTokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("x")));
    infixTokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("+")));
    infixTokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("y")));

    std::shared_ptr<TestConvertInfixToPostfix::TestableParser> parser =
        std::make_shared<TestConvertInfixToPostfix::TestableParser>(
            infixTokens);

    std::queue<std::shared_ptr<std::string>> postFixQueue =
        parser->ConvertInfixToPostfix(infixTokens);

    std::shared_ptr<std::string> first = postFixQueue.front();
    postFixQueue.pop();
    std::shared_ptr<std::string> second = postFixQueue.front();
    postFixQueue.pop();
    std::shared_ptr<std::string> third = postFixQueue.front();
    postFixQueue.pop();

    REQUIRE(*first == "x");
    REQUIRE(*second == "y");
    REQUIRE(*third == "+");
  }

  SECTION(
      "Test infix expression should correctly convert to postfix expression "
      "i.e. v + x * y") {
    std::vector<std::shared_ptr<Token>> infixTokens;

    infixTokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("v")));
    infixTokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("+")));
    infixTokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("x")));
    infixTokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("*")));
    infixTokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("y")));

    std::shared_ptr<TestConvertInfixToPostfix::TestableParser> parser =
        std::make_shared<TestConvertInfixToPostfix::TestableParser>(
            infixTokens);

    std::queue<std::shared_ptr<std::string>> postFixQueue =
        parser->ConvertInfixToPostfix(infixTokens);

    std::shared_ptr<std::string> first = postFixQueue.front();
    postFixQueue.pop();
    std::shared_ptr<std::string> second = postFixQueue.front();
    postFixQueue.pop();
    std::shared_ptr<std::string> third = postFixQueue.front();
    postFixQueue.pop();
    std::shared_ptr<std::string> fourth = postFixQueue.front();
    postFixQueue.pop();
    std::shared_ptr<std::string> fifth = postFixQueue.front();
    postFixQueue.pop();

    REQUIRE(*first == "v");
    REQUIRE(*second == "x");
    REQUIRE(*third == "y");
    REQUIRE(*fourth == "*");
    REQUIRE(*fifth == "+");
  }

  SECTION(
      "Test infix expression should correctly convert to postfix expression "
      "i.e. x * y + z * t") {
    std::vector<std::shared_ptr<Token>> infixTokens;

    infixTokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("x")));
    infixTokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("*")));
    infixTokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("y")));
    infixTokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("+")));
    infixTokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("z")));
    infixTokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("*")));
    infixTokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("t")));

    std::shared_ptr<TestConvertInfixToPostfix::TestableParser> parser =
        std::make_shared<TestConvertInfixToPostfix::TestableParser>(
            infixTokens);

    std::queue<std::shared_ptr<std::string>> postFixQueue =
        parser->ConvertInfixToPostfix(infixTokens);

    std::shared_ptr<std::string> first = postFixQueue.front();
    postFixQueue.pop();
    std::shared_ptr<std::string> second = postFixQueue.front();
    postFixQueue.pop();
    std::shared_ptr<std::string> third = postFixQueue.front();
    postFixQueue.pop();
    std::shared_ptr<std::string> fourth = postFixQueue.front();
    postFixQueue.pop();
    std::shared_ptr<std::string> fifth = postFixQueue.front();
    postFixQueue.pop();
    std::shared_ptr<std::string> sixth = postFixQueue.front();
    postFixQueue.pop();
    std::shared_ptr<std::string> seventh = postFixQueue.front();
    postFixQueue.pop();

    REQUIRE(*first == "x");
    REQUIRE(*second == "y");
    REQUIRE(*third == "*");
    REQUIRE(*fourth == "z");
    REQUIRE(*fifth == "t");
    REQUIRE(*sixth == "*");
    REQUIRE(*seventh == "+");
  }

  SECTION(
      "Test infix expression should correctly convert to postfix expression "
      "i.e. x / y + z * t") {
    std::vector<std::shared_ptr<Token>> infixTokens;

    infixTokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("x")));
    infixTokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("/")));
    infixTokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("y")));
    infixTokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("+")));
    infixTokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("z")));
    infixTokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("*")));
    infixTokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("t")));

    std::shared_ptr<TestConvertInfixToPostfix::TestableParser> parser =
        std::make_shared<TestConvertInfixToPostfix::TestableParser>(
            infixTokens);

    std::queue<std::shared_ptr<std::string>> postFixQueue =
        parser->ConvertInfixToPostfix(infixTokens);

    std::shared_ptr<std::string> first = postFixQueue.front();
    postFixQueue.pop();
    std::shared_ptr<std::string> second = postFixQueue.front();
    postFixQueue.pop();
    std::shared_ptr<std::string> third = postFixQueue.front();
    postFixQueue.pop();
    std::shared_ptr<std::string> fourth = postFixQueue.front();
    postFixQueue.pop();
    std::shared_ptr<std::string> fifth = postFixQueue.front();
    postFixQueue.pop();
    std::shared_ptr<std::string> sixth = postFixQueue.front();
    postFixQueue.pop();
    std::shared_ptr<std::string> seventh = postFixQueue.front();
    postFixQueue.pop();

    REQUIRE(*first == "x");
    REQUIRE(*second == "y");
    REQUIRE(*third == "/");
    REQUIRE(*fourth == "z");
    REQUIRE(*fifth == "t");
    REQUIRE(*sixth == "*");
    REQUIRE(*seventh == "+");
  }

  SECTION(
      "Test infix expression should correctly convert to postfix expression "
      "i.e. x * y / z * t") {
    std::vector<std::shared_ptr<Token>> infixTokens;

    infixTokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("x")));
    infixTokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("*")));
    infixTokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("y")));
    infixTokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("/")));
    infixTokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("z")));
    infixTokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("*")));
    infixTokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("t")));

    std::shared_ptr<TestConvertInfixToPostfix::TestableParser> parser =
        std::make_shared<TestConvertInfixToPostfix::TestableParser>(
            infixTokens);

    std::queue<std::shared_ptr<std::string>> postFixQueue =
        parser->ConvertInfixToPostfix(infixTokens);

    std::shared_ptr<std::string> first = postFixQueue.front();
    postFixQueue.pop();
    std::shared_ptr<std::string> second = postFixQueue.front();
    postFixQueue.pop();
    std::shared_ptr<std::string> third = postFixQueue.front();
    postFixQueue.pop();
    std::shared_ptr<std::string> fourth = postFixQueue.front();
    postFixQueue.pop();
    std::shared_ptr<std::string> fifth = postFixQueue.front();
    postFixQueue.pop();
    std::shared_ptr<std::string> sixth = postFixQueue.front();
    postFixQueue.pop();
    std::shared_ptr<std::string> seventh = postFixQueue.front();
    postFixQueue.pop();

    REQUIRE(*first == "x");
    REQUIRE(*second == "y");
    REQUIRE(*third == "*");
    REQUIRE(*fourth == "z");
    REQUIRE(*fifth == "/");
    REQUIRE(*sixth == "t");
    REQUIRE(*seventh == "*");
  }

  SECTION(
      "Test infix expression should correctly convert to postfix expression "
      "i.e. (x * y) / (z * t)") {
    std::vector<std::shared_ptr<Token>> infixTokens;
    infixTokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("(")));
    infixTokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("x")));
    infixTokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("*")));
    infixTokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("y")));
    infixTokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(")")));
    infixTokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("/")));
    infixTokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("(")));
    infixTokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("z")));
    infixTokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("*")));
    infixTokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("t")));
    infixTokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(")")));

    std::shared_ptr<TestConvertInfixToPostfix::TestableParser> parser =
        std::make_shared<TestConvertInfixToPostfix::TestableParser>(
            infixTokens);

    std::queue<std::shared_ptr<std::string>> postFixQueue =
        parser->ConvertInfixToPostfix(infixTokens);

    std::shared_ptr<std::string> first = postFixQueue.front();
    postFixQueue.pop();
    std::shared_ptr<std::string> second = postFixQueue.front();
    postFixQueue.pop();
    std::shared_ptr<std::string> third = postFixQueue.front();
    postFixQueue.pop();
    std::shared_ptr<std::string> fourth = postFixQueue.front();
    postFixQueue.pop();
    std::shared_ptr<std::string> fifth = postFixQueue.front();
    postFixQueue.pop();
    std::shared_ptr<std::string> sixth = postFixQueue.front();
    postFixQueue.pop();
    std::shared_ptr<std::string> seventh = postFixQueue.front();
    postFixQueue.pop();

    REQUIRE(*first == "x");
    REQUIRE(*second == "y");
    REQUIRE(*third == "*");
    REQUIRE(*fourth == "z");
    REQUIRE(*fifth == "t");
    REQUIRE(*sixth == "*");
    REQUIRE(*seventh == "/");
  }

  SECTION(
      "Test infix expression should correctly convert to postfix expression "
      "i.e. (((v) + ((x) * (y))) + ((z) * (t)))") {
    std::vector<std::shared_ptr<Token>> infixTokens;

    infixTokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("(")));
    infixTokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("(")));
    infixTokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("(")));
    infixTokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("v")));
    infixTokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(")")));
    infixTokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("+")));
    infixTokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("(")));
    infixTokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("(")));
    infixTokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("x")));
    infixTokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(")")));
    infixTokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("*")));
    infixTokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("(")));
    infixTokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("y")));
    infixTokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(")")));
    infixTokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(")")));
    infixTokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(")")));
    infixTokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("+")));
    infixTokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("(")));
    infixTokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("(")));
    infixTokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("z")));
    infixTokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(")")));
    infixTokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("*")));
    infixTokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("(")));
    infixTokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("t")));
    infixTokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(")")));
    infixTokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(")")));
    infixTokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(")")));

    std::shared_ptr<TestConvertInfixToPostfix::TestableParser> parser =
        std::make_shared<TestConvertInfixToPostfix::TestableParser>(
            infixTokens);

    std::queue<std::shared_ptr<std::string>> postFixQueue =
        parser->ConvertInfixToPostfix(infixTokens);

    std::shared_ptr<std::string> first = postFixQueue.front();
    postFixQueue.pop();
    std::shared_ptr<std::string> second = postFixQueue.front();
    postFixQueue.pop();
    std::shared_ptr<std::string> third = postFixQueue.front();
    postFixQueue.pop();
    std::shared_ptr<std::string> fourth = postFixQueue.front();
    postFixQueue.pop();
    std::shared_ptr<std::string> fifth = postFixQueue.front();
    postFixQueue.pop();
    std::shared_ptr<std::string> sixth = postFixQueue.front();
    postFixQueue.pop();
    std::shared_ptr<std::string> seventh = postFixQueue.front();
    postFixQueue.pop();
    std::shared_ptr<std::string> eighth = postFixQueue.front();
    postFixQueue.pop();
    std::shared_ptr<std::string> ninth = postFixQueue.front();

    REQUIRE(*first == "v");
    REQUIRE(*second == "x");
    REQUIRE(*third == "y");
    REQUIRE(*fourth == "*");
    REQUIRE(*fifth == "+");
    REQUIRE(*sixth == "z");
    REQUIRE(*seventh == "t");
    REQUIRE(*eighth == "*");
    REQUIRE(*ninth == "+");
  }

  SECTION("Test invalid infix expression should throw error i.e. (x + y * z") {
    std::vector<std::shared_ptr<Token>> infixTokens;

    infixTokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("(")));
    infixTokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("x")));
    infixTokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("+")));
    infixTokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("y")));
    infixTokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("*")));
    infixTokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("z")));

    std::shared_ptr<TestConvertInfixToPostfix::TestableParser> parser =
        std::make_shared<TestConvertInfixToPostfix::TestableParser>(
            infixTokens);

    REQUIRE_THROWS_AS(parser->ConvertInfixToPostfix(infixTokens),
                      std::invalid_argument);
  }

  SECTION(
      "Test valid infix expression should not throw error i.e. ((x + y)) * z") {
    std::vector<std::shared_ptr<Token>> infixTokens;

    infixTokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("(")));
    infixTokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("(")));
    infixTokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("x")));
    infixTokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("+")));
    infixTokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("y")));
    infixTokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(")")));
    infixTokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(")")));
    infixTokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("*")));
    infixTokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("z")));

    std::shared_ptr<TestConvertInfixToPostfix::TestableParser> parser =
        std::make_shared<TestConvertInfixToPostfix::TestableParser>(
            infixTokens);

    REQUIRE_NOTHROW(parser->ConvertInfixToPostfix(infixTokens));
  }

  SECTION("Test valid infix expression should not throw error i.e. (a)(b)") {
    std::vector<std::shared_ptr<Token>> infixTokens;

    infixTokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("(")));
    infixTokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("a")));
    infixTokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(")")));
    infixTokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("(")));
    infixTokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("b")));

    infixTokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(")")));

    std::shared_ptr<TestConvertInfixToPostfix::TestableParser> parser =
        std::make_shared<TestConvertInfixToPostfix::TestableParser>(
            infixTokens);

    REQUIRE_NOTHROW(parser->ConvertInfixToPostfix(infixTokens));
  }

  SECTION(
      "Test invalid infix expression should throw error i.e. a + b + c + (") {
    std::vector<std::shared_ptr<Token>> infixTokens;

    infixTokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("a")));
    infixTokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("+")));
    infixTokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("b")));
    infixTokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("+")));
    infixTokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("c")));
    infixTokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("+")));
    infixTokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("(")));

    std::shared_ptr<TestConvertInfixToPostfix::TestableParser> parser =
        std::make_shared<TestConvertInfixToPostfix::TestableParser>(
            infixTokens);

    REQUIRE_THROWS_AS(parser->ConvertInfixToPostfix(infixTokens),
                      std::invalid_argument);
  }

  SECTION(
      "Test invalid infix expression should throw error i.e. ((x + y))) * z") {
    std::vector<std::shared_ptr<Token>> infixTokens;

    infixTokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("(")));
    infixTokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("(")));
    infixTokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("x")));
    infixTokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("+")));
    infixTokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("y")));
    infixTokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(")")));
    infixTokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(")")));
    infixTokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>(")")));
    infixTokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("*")));
    infixTokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("z")));

    std::shared_ptr<TestConvertInfixToPostfix::TestableParser> parser =
        std::make_shared<TestConvertInfixToPostfix::TestableParser>(
            infixTokens);

    REQUIRE_THROWS_AS(parser->ConvertInfixToPostfix(infixTokens),
                      std::invalid_argument);
  }
}
