#include "../../spa/src/shared/parser/AParser.h"
#include "../../spa/src/shared/tokenizer/token/EofToken.h"
#include "../../spa/src/shared/tokenizer/token/IntegerToken.h"
#include "../../spa/src/shared/tokenizer/token/SpecialCharToken.h"
#include "../../spa/src/shared/tokenizer/token/Token.h"
#include "../../spa/src/shared/tokenizer/token/WordToken.h"
#include "../../spa/src/source_processor/parser/SpParser.h"
#include "catch.hpp"

class TestableParser : public AParser {
 public:
  TestableParser(std::vector<std::shared_ptr<Token>> tokens)
      : AParser(tokens) {}
  using AParser::ConvertInfixToPostfix;

  void parse() override {}
};

TEST_CASE("Test Convert infix expression to postfix expression",
          "[ConvertInfixToPostfix]") {
  SECTION("Test infix expression i.e. v") {
    std::vector<std::shared_ptr<Token>> infixTokens;

    infixTokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("v")));

    std::shared_ptr<TestableParser> parser =
        std::make_shared<TestableParser>(infixTokens);

    std::queue<std::shared_ptr<std::string>> postFixQueue =
        parser->ConvertInfixToPostfix(infixTokens);

    std::shared_ptr<std::string> first = postFixQueue.front();
    postFixQueue.pop();

    REQUIRE(*first == "v");
  }

  SECTION("Test infix expression i.e. x + y") {
    std::vector<std::shared_ptr<Token>> infixTokens;

    infixTokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("x")));
    infixTokens.push_back(std::static_pointer_cast<Token>(
        std::make_shared<SpecialCharToken>("+")));
    infixTokens.push_back(
        std::static_pointer_cast<Token>(std::make_shared<WordToken>("y")));

    std::shared_ptr<TestableParser> parser =
        std::make_shared<TestableParser>(infixTokens);

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

  SECTION("Test infix expression i.e. v + x * y") {
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

    std::shared_ptr<TestableParser> parser =
        std::make_shared<TestableParser>(infixTokens);

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

  SECTION("Test infix expression i.e. x * y + z * t") {
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

    std::shared_ptr<TestableParser> parser =
        std::make_shared<TestableParser>(infixTokens);

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

  SECTION("Test infix expression i.e. x / y + z * t") {
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

        std::shared_ptr<TestableParser> parser =
                std::make_shared<TestableParser>(infixTokens);

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

  SECTION("Test infix expression i.e. x * y / z * t") {
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

        std::shared_ptr<TestableParser> parser =
                std::make_shared<TestableParser>(infixTokens);

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
}