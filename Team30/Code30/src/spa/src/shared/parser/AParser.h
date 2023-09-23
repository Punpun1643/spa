#pragma once

#include <memory>
#include <vector>

#include "../tokenizer/token/Token.h"

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
