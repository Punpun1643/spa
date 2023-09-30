#pragma once

#include <memory>
#include <queue>
#include <stack>
#include <string>
#include <vector>

#include "../tokenizer/token/Token.h"
#include "node/TreeNode.h"

class AParser {
 public:
  AParser(std::vector<std::shared_ptr<Token>> tokens);

  // move pointer to the next token
  std::shared_ptr<Token> nextToken();

  // explore the next token without moving the pointer
  std::shared_ptr<Token> peekToken();

  std::shared_ptr<Token> peekBackToken();

  // get the current token
  std::shared_ptr<Token> getCurrToken();

  /**
   * @brief Returns the value of the current token.
   *
   * @return The value of the current token.
   */
  std::string getCurrTokenValue();

  /**
   * @brief Parses the tokens.
   *
   * @throw std::runtime_error if the tokens are not valid.
   */
  std::string getPeekTokenValue();

  std::string getPeekBackTokenValue();

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

  static std::shared_ptr<TreeNode> BuildExprTreeAndValidate(
      std::queue<std::shared_ptr<std::string>>& postFixQueue);

  std::queue<std::shared_ptr<std::string>> ConvertInfixToPostfix(
      std::vector<std::shared_ptr<Token>> infixTokens);

  void HandleInfixWordOrIntegerToken(std::shared_ptr<Token> token,
      std::queue<std::shared_ptr<std::string>>& postFixQueue);

  void HandleInfixOperatorToken(
      std::shared_ptr<Token> token,
      std::stack<std::shared_ptr<std::string>>& operatorStack,
      std::queue<std::shared_ptr<std::string>>& postFixQueue);

  void HandleLeftParenthesisToken(
      std::shared_ptr<Token> token,
      std::stack<std::shared_ptr<std::string>>& operatorStack, int& parenCount);

  void HandleRightParenthesisToken(
      std::stack<std::shared_ptr<std::string>>& operatorStack,
      std::queue<std::shared_ptr<std::string>>& postFixQueue, int& parenCount);

  virtual int Precedence(std::string const& operatorValue);

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
  bool isCurrTokenType(TokenType tokenType);

  /**
   * @brief Checks if the current token is of the given token value.
   *
   * @param tokenValue The token value to check against.
   *
   * @return True if the current token is of the given token value,
   * false otherwise.
   */
  bool isCurrTokenValue(std::string const& tokenValue);

  /**
   * @brief Checks if the peek token is of the given token value.
   *
   * @param tokenValue The token value to check against.
   *
   * @return True if the current token is of the given token value,
   * false otherwise.
   */
  bool isPeekTokenValue(std::string const& tokenValue);

  bool isPeekBackTokenValue(std::string const& tokenValue);
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
  bool isCurrTokenTypeAndValue(TokenType tokenType,
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
  void assertCurrTokenTypeAndValue(TokenType expectedType,
                                   std::string const& expectedValue,
                                   std::string const& errorMessage);
};
