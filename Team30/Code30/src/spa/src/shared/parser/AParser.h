#pragma once

#include <memory>
#include <queue>
#include <stack>
#include <string>
#include <vector>

#include "../tokenizer/token/Token.h"
#include "constant/AParserConstant.h"
#include "node/TreeNode.h"

class AParser {
 public:
  AParser(std::vector<std::shared_ptr<Token>> tokens);

  /**
   * @brief Moves the pointer to the next token.
   */
  std::shared_ptr<Token> NextToken();

  /**
   * @brief Explores the next token without moving the pointer.
   */
  std::shared_ptr<Token> PeekToken();

  /**
   * @brief Explores the previous token without moving the pointer.
   */
  std::shared_ptr<Token> PeekBackToken();

  /**
   * @brief Returns the current token.
   */
  std::shared_ptr<Token> GetCurrToken();

  /**
   * @brief Returns the value of the current token.
   *
   * @return The value of the current token.
   */
  std::string GetCurrTokenValue();

  /**
   * @brief Returns the value of the next token.
   *
   * @return The value of the next token.
   */
  std::string GetPeekTokenValue();

  /**
   * @brief Returns the value of the previous token.
   *
   * @return The value of the previous token.
   */
  std::string GetPeekBackTokenValue();

  virtual void parse() = 0;

  virtual ~AParser() = default;

 protected:
  std::vector<std::shared_ptr<Token>> tokens;

  size_t currTokenIndex = 0;

  static bool IsTokenType(std::shared_ptr<Token> token, TokenType token_type);

  static bool IsWordToken(std::shared_ptr<Token> token);

  static bool IsIntegerToken(std::shared_ptr<Token> token);

  static bool IsWordOrIntegerToken(std::shared_ptr<Token> token);

  static bool IsSpecialCharToken(std::shared_ptr<Token> token);

  static bool IsEOFToken(std::shared_ptr<Token> token);

  static bool IsTokenValue(std::shared_ptr<Token> token,
                           std::string const& tokenValue);

  static bool IsMathematicalOperator(std::string const& tokenValue);

  /**
   * @brief Checks if the current tree stack has the expected size.
   *
   * @param treeStack The tree stack to check.
   * @param size The expected size of the tree stack.
   */
  static void ValidateTreeStackSize(
      std::stack<std::shared_ptr<TreeNode>>& treeStack, int size);

  /**
   * @brief Builds an expression tree from the given postfix queue.
   *
   * @param postFixQueue The postfix queue from an infix expression.
   * @return The root of the expression tree.
   */
  static std::shared_ptr<TreeNode> BuildExprTreeAndValidate(
      std::queue<std::shared_ptr<std::string>>& postFixQueue);

  /**
   * @brief Converts the given infix tokens to postfix.
   *
   * @param infixTokens The infix tokens from given expression.
   * @return The postfix queue.
   */
  std::queue<std::shared_ptr<std::string>> ConvertInfixToPostfix(
      std::vector<std::shared_ptr<Token>> infixTokens);

  /**
   * @brief Handles the given infix word or integer token.
   *
   * @param token The infix word or integer token.
   * @param postFixQueue The postfix queue to add the token to.
   */
  void HandleInfixWordOrIntegerToken(
      std::shared_ptr<Token> token,
      std::queue<std::shared_ptr<std::string>>& postFixQueue);

  /**
   * @brief Handles the given infix operator token.
   *
   * @param token The infix operator token.
   * @param operatorStack The operator stack to add the token to.
   */
  void HandleInfixOperatorToken(
      std::shared_ptr<Token> token,
      std::stack<std::shared_ptr<std::string>>& operatorStack,
      std::queue<std::shared_ptr<std::string>>& postFixQueue);

  /**
   * @brief Handles the given left parenthesis token.
   *
   * @param token The left parenthesis token.
   * @param operatorStack The operator stack to add the token to.
   */
  void HandleLeftParenthesisToken(
      std::shared_ptr<Token> token,
      std::stack<std::shared_ptr<std::string>>& operatorStack, int& parenCount);

  /**
   * @brief Handles the given right parenthesis token.
   *
   * @param operatorStack The operator stack to add the token to.
   * @param postFixQueue The postfix queue to add the token to.
   * @param parenCount The number of left parenthesis tokens.
   */
  void HandleRightParenthesisToken(
      std::stack<std::shared_ptr<std::string>>& operatorStack,
      std::queue<std::shared_ptr<std::string>>& postFixQueue, int& parenCount);

  /**
   * @brief The precedence definition of the given operator.
   *
   * @param operatorValue The operator value.
   * @return The precedence of the given operator.
   */
  virtual int Precedence(std::string const& operatorValue);

  /**
   * @brief Checks if the first operator has a higher or equal precedence than
   * the second operator.
   *
   * @param operatorValue1 The first operator value.
   * @param operatorValue2 The second operator value.
   * @return True if the first operator has a higher or equal precedence than
   */
  bool IsGreaterOrEqualPrecedence(std::string const& operatorValue1,
                                  std::string const& operatorValue2);

  /**
   * @brief Checks if the current token is of the given token type.
   *
   * @param tokenType The token type to check against.
   *
   * @return True if the current token is of the given token type, false
   * otherwise.
   */
  bool IsCurrTokenType(TokenType tokenType);

  /**
   * @brief Checks if the current token is of the given token value.
   *
   * @param tokenValue The token value to check against.
   *
   * @return True if the current token is of the given token value,
   * false otherwise.
   */
  bool IsCurrTokenValue(std::string const& tokenValue);

  /**
   * @brief Checks if the peek token is of the given token value.
   *
   * @param tokenValue The token value to check against.
   *
   * @return True if the current token is of the given token value,
   * false otherwise.
   */
  bool IsPeekTokenValue(std::string const& tokenValue);

  bool IsPeekBackTokenValue(std::string const& tokenValue);
  /**
   * @brief Checks if the current token and type value is equal to the given
   * token value.
   *
   * Also returns false if the current token is an EOF token.
   *
   * @param tokenType The token type to check against.
   * @param tokenValue The token value to check against.
   *
   * @return True if the current token value is equal to the given token value,
   * false otherwise.
   */
  bool IsCurrTokenTypeAndValue(TokenType tokenType,
                               std::string const& tokenValue);

  /**
   * @brief Asserts that the current token is of the given token type.
   *
   * @param expectedType
   * @param expectedValue
   * @param errorMessage
   *
   * @throw std::runtime_error if the current token is not of the given token
   * type.
   */
  void AssertCurrTokenTypeAndValue(TokenType expectedType,
                                   std::string const& expectedValue,
                                   std::string const& errorMessage);

  /**
   * @brief Checks if the current token is of the given token type.
   *
   * @tparam ExceptionType
   * @param expectedType
   *
   * @throw ExceptionType if the current token is not of the given token type.
   */
  template <typename ExceptionType>
  void CheckCurrTokenType(TokenType expectedType) {
    if (!IsCurrTokenType(expectedType)) {
      throw ExceptionType();
    }
  }

  /**
   * @brief Checks if the current token is of the given token value and advance
   * to the next token.
   *
   * @tparam ExceptionType
   * @param expectedValue
   *
   * @throw ExceptionType if the current token is not of the given token value.
   */
  template <typename ExceptionType>
  void CheckAndAdvanceCurrToken(std::string const& expectedValue) {
    if (!IsCurrTokenValue(expectedValue)) {
      throw ExceptionType();
    }
    NextToken();
  }

  /**
   * @brief Parse and validate a variable/procedure name, and advance
   * to the next token.
   *
   * @tparam ExceptionType The exception type to throw if the current token is
   * not a variable name.
   * @return The variable/procedure name.
   */
  template <typename ExceptionType>
  std::string ParseAndValidateVarName() {
    CheckCurrTokenType<ExceptionType>(TokenType::WORD_TOKEN);
    std::string varName = GetCurrTokenValue();
    NextToken();
    CheckAndAdvanceCurrToken<ExceptionType>(AParserConstant::STMT_TERMINATOR);
    return varName;
  }

  std::string GetCurrTokenValueAndAdvance();
};
