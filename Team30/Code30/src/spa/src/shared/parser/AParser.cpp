#include "AParser.h"

#include <stdexcept>

namespace a_parser_constant {
constexpr char LEFT_PARENTHESIS[] = "(";
constexpr char RIGHT_PARENTHESIS[] = ")";
}  // namespace a_parser_constant

namespace a_parser_mathematical_operator {
constexpr char PLUS[] = "+";
constexpr char MINUS[] = "-";
constexpr char MULTIPLY[] = "*";
constexpr char DIVIDE[] = "/";
constexpr char MODULO[] = "%";
}  // namespace a_parser_mathematical_operator

AParser::AParser(std::vector<std::shared_ptr<Token>> tokens)
    : tokens(std::move(tokens)) {}

std::shared_ptr<Token> AParser::nextToken() {
  if (currTokenIndex < tokens.size()) {
    return tokens[++currTokenIndex];
  }
}

std::shared_ptr<Token> AParser::peekToken() {
  if (currTokenIndex + 1 < tokens.size()) {
    return tokens[currTokenIndex + 1];
  }
}

std::shared_ptr<Token> AParser::peekBackToken() {
  if (currTokenIndex - 1 >= 0) {
    return tokens[currTokenIndex - 1];
  }
}

std::shared_ptr<Token> AParser::getCurrToken() {
  return tokens[currTokenIndex];
}

std::string AParser::getCurrTokenValue() {
  return getCurrToken()->getTokenVal();
}

std::string AParser::getPeekTokenValue() { return peekToken()->getTokenVal(); }

std::string AParser::getPeekBackTokenValue() {
  return peekBackToken()->getTokenVal();
}

bool AParser::IsTokenType(std::shared_ptr<Token> token, TokenType token_type) {
  if (token->getTokenType() != token_type) {
    return false;
  }
  return true;
}

bool AParser::IsWordToken(std::shared_ptr<Token> token) {
  return AParser::IsTokenType(token, TokenType::WORD_TOKEN);
}

bool AParser::IsIntegerToken(std::shared_ptr<Token> token) {
  return AParser::IsTokenType(token, TokenType::INTEGER_TOKEN);
}

bool AParser::IsWordOrIntegerToken(std::shared_ptr<Token> token) {
  return AParser::IsWordToken(token) || AParser::IsIntegerToken(token);
}

bool AParser::IsSpecialCharToken(std::shared_ptr<Token> token) {
  return AParser::IsTokenType(token, TokenType::SPECIAL_CHAR_TOKEN);
}

bool AParser::IsEOFToken(std::shared_ptr<Token> token) {
  return AParser::IsTokenType(token, TokenType::EOF_TOKEN);
}

bool AParser::IsTokenValue(std::shared_ptr<Token> token,
                           std::string const& tokenValue) {
  return tokenValue == token->getTokenVal();
}

bool AParser::IsMathematicalOperator(std::string const& tokenValue) {
  return tokenValue == a_parser_mathematical_operator::PLUS ||
         tokenValue == a_parser_mathematical_operator::MINUS ||
         tokenValue == a_parser_mathematical_operator::MULTIPLY ||
         tokenValue == a_parser_mathematical_operator::DIVIDE ||
         tokenValue == a_parser_mathematical_operator::MODULO;
}

bool AParser::isCurrTokenType(TokenType tokenType) {
  return IsTokenType(getCurrToken(), tokenType);
}

bool AParser::isCurrTokenValue(std::string const& tokenValue) {
  return tokenValue == getCurrToken()->getTokenVal();
}

bool AParser::isPeekTokenValue(std::string const& tokenValue) {
  return tokenValue == peekToken()->getTokenVal();
}

bool AParser::isPeekBackTokenValue(std::string const& tokenValue) {
  return tokenValue == peekBackToken()->getTokenVal();
}

bool AParser::isCurrTokenTypeAndValue(TokenType tokenType,
                                      std::string const& tokenValue) {
  return IsTokenType(getCurrToken(), tokenType) &&
         tokenValue == getCurrToken()->getTokenVal();
}

void AParser::assertCurrTokenTypeAndValue(TokenType expectedType,
                                          std::string const& expectedValue,
                                          std::string const& errorMessage) {
  if (!isCurrTokenTypeAndValue(expectedType, expectedValue)) {
    throw std::invalid_argument(errorMessage);
  }
}

void AParser::HandleInfixWordOrIntegerToken(
    std::queue<std::shared_ptr<std::string>>& postFixQueue,
    std::shared_ptr<Token> token) {
  postFixQueue.push(std::make_shared<std::string>(token->getTokenVal()));
}

int AParser::Precedence(std::string const& operatorValue) {
  if (operatorValue == a_parser_mathematical_operator::MULTIPLY ||
      operatorValue == a_parser_mathematical_operator::DIVIDE ||
      operatorValue == a_parser_mathematical_operator::MODULO) {
    return 2;
  } else if (operatorValue == a_parser_mathematical_operator::PLUS ||
             operatorValue == a_parser_mathematical_operator::MINUS) {
    return 1;
  } else {
    return -1;
  }
}

bool AParser::IsGreaterOrEqualPrecedence(std::string const& operatorValue1,
                                  std::string const& operatorValue2) {
  return Precedence(operatorValue1) >= Precedence(operatorValue2);
}

void AParser::HandleInfixOperatorToken(
    std::stack<std::shared_ptr<std::string>>& operatorStack,
    std::queue<std::shared_ptr<std::string>>& postFixQueue) {

}

// convert infix to postfix
std::queue<std::shared_ptr<std::string>> AParser::ConvertInfixToPostfix(
    std::vector<std::shared_ptr<Token>> infixTokens) {
  std::queue<std::shared_ptr<std::string>> postFixQueue;
  std::stack<std::shared_ptr<std::string>> operatorStack;

  int parentCount = 0;

  for (auto const& token : infixTokens) {
    if (IsWordOrIntegerToken(token)) {
      HandleInfixWordOrIntegerToken(postFixQueue, token);
    } else if (IsMathematicalOperator(token->getTokenVal())) {
      // TODO: handle operator token
      HandleInfixOperatorToken(operatorStack, postFixQueue);
    }
  }
}