#include "../../spa/src/shared/parser/node/TreeNode.h"
#include "catch.hpp"

TEST_CASE("Test if a string is the substring of another string",
          "[IsSubString]") {
  SECTION(
      "Test base string: x + y, substring: x + y + z, should return false") {
    std::vector<std::string> baseString = {"x", "+", "y"};
    std::vector<std::string> subString = {"x", "+",
                                          "y"
                                          "+",
                                          "z"};

    bool isSubString = TreeNode::IsSubString(baseString, subString);

    REQUIRE(isSubString == false);
  }

  SECTION("Test base string: x + y, substring: x + y, should return true") {
    std::vector<std::string> baseString = {"x", "+", "y"};
    std::vector<std::string> subString = {"x", "+", "y"};

    bool isSubString = TreeNode::IsSubString(baseString, subString);

    REQUIRE(isSubString);
  }

  SECTION(
      "Test base string: v + x * y + z * t, substring: v + x * y, should "
      "return true") {
    std::vector<std::string> baseString = {"v", "+", "x", "*", "y",
                                           "+", "z", "*", "t"};
    std::vector<std::string> subString = {"v", "+", "x", "*", "y"};

    bool isSubString = TreeNode::IsSubString(baseString, subString);

    REQUIRE(isSubString);
  }

  SECTION(
      "Test base string: + + v * x y * z t, substring: v * x y, should return "
      "true") {
    std::vector<std::string> baseString = {"+", "+", "v", "*", "x",
                                           "y", "*", "z", "t"};
    std::vector<std::string> subString = {"v", "*", "x", "y"};

    bool isSubString = TreeNode::IsSubString(baseString, subString);

    REQUIRE(isSubString);
  }

  SECTION(
      "Test base string: + + v * x y * z t, substring: y + z * t, should "
      "return false") {
    std::vector<std::string> baseString = {"+", "+", "v", "*", "x",
                                           "y", "*", "z", "t"};
    std::vector<std::string> subString = {"y", "+", "z", "*", "t"};

    bool isSubString = TreeNode::IsSubString(baseString, subString);

    REQUIRE(isSubString == false);
  }
}