#include "AParser.h"

#include "exceptions/EmptyParenthesesException.h"
#include "exceptions/EndOfFileException.h"
#include "exceptions/InvalidExprException.h"
#include "exceptions/InvalidExprSpecException.h"
#include "exceptions/InvalidTokenException.h"
#include "exceptions/StartOfFileException.h"
#include "exceptions/UnmatchedParenthesesException.h"

AParser::AParser(std::vector<std::shared_ptr<Token>> tokens)
    : tokens(std::move(tokens)) {}

std::shared_ptr<Token> AParser::NextToken() {
  if (currTokenIndex < tokens.size()) {
    return tokens[++currTokenIndex];
  }
  throw EndOfFileException();
}

std::shared_ptr<Token> AParser::PeekToken() {
  if (currTokenIndex + 1 < tokens.size()) {
    return tokens[currTokenIndex + 1];
  }
  throw EndOfFileException();
}

std::shared_ptr<Token> AParser::PeekBackToken() {
  if (currTokenIndex - 1 >= 0) {
    return tokens[currTokenIndex - 1];
  }
  throw StartOfFileException();
}

std::shared_ptr<Token> AParser::GetCurrToken() {
  return tokens[currTokenIndex];
}

std::string AParser::GetCurrTokenValue() {
  return GetCurrToken()->getTokenVal();
}

std::string AParser::GetPeekTokenValue() { return PeekToken()->getTokenVal(); }

std::string AParser::GetPeekBackTokenValue() {
  return PeekBackToken()->getTokenVal();
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
  return tokenValue == AParserConstant::PLUS ||
         tokenValue == AParserConstant::MINUS ||
         tokenValue == AParserConstant::MULTIPLY ||
         tokenValue == AParserConstant::DIVIDE ||
         tokenValue == AParserConstant::MODULO;
}

bool AParser::IsCurrTokenType(TokenType tokenType) {
  return IsTokenType(GetCurrToken(), tokenType);
}

bool AParser::IsCurrTokenValue(std::string const& tokenValue) {
  return tokenValue == GetCurrToken()->getTokenVal();
}

bool AParser::IsPeekTokenValue(std::string const& tokenValue) {
  return tokenValue == PeekToken()->getTokenVal();
}

bool AParser::IsPeekBackTokenValue(std::string const& tokenValue) {
  return tokenValue == PeekBackToken()->getTokenVal();
}

bool AParser::IsCurrTokenTypeAndValue(TokenType tokenType,
                                      std::string const& tokenValue) {
  return IsTokenType(GetCurrToken(), tokenType) &&
         tokenValue == GetCurrToken()->getTokenVal();
}

void AParser::AssertCurrTokenTypeAndValue(TokenType expectedType,
                                          std::string const& expectedValue,
                                          std::string const& errorMessage) {
  if (!IsCurrTokenTypeAndValue(expectedType, expectedValue)) {
    throw InvalidTokenException(errorMessage);
  }
}

std::string AParser::GetCurrTokenValueAndAdvance() {
  std::string tokenValue = GetCurrTokenValue();
  NextToken();
  return tokenValue;
}

void AParser::HandleInfixWordOrIntegerToken(
    std::shared_ptr<Token> token,
    std::queue<std::shared_ptr<std::string>>& postFixQueue) {
  postFixQueue.push(std::make_shared<std::string>(token->getTokenVal()));
}

int AParser::Precedence(std::string const& operatorValue) {
  if (operatorValue == AParserConstant::MULTIPLY ||
      operatorValue == AParserConstant::DIVIDE ||
      operatorValue == AParserConstant::MODULO) {
    return 2;
  } else if (operatorValue == AParserConstant::PLUS ||
             operatorValue == AParserConstant::MINUS) {
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
    std::shared_ptr<Token> token,
    std::stack<std::shared_ptr<std::string>>& operatorStack,
    std::queue<std::shared_ptr<std::string>>& postFixQueue) {
  while (!operatorStack.empty() &&
         IsGreaterOrEqualPrecedence(operatorStack.top()->c_str(),
                                    token->getTokenVal())) {
    postFixQueue.push(operatorStack.top());
    operatorStack.pop();
  }
  operatorStack.push(std::make_shared<std::string>(token->getTokenVal()));
}

void AParser::HandleLeftParenthesisToken(
    std::shared_ptr<Token> token,
    std::stack<std::shared_ptr<std::string>>& operatorStack, int& parenCount) {
  ++parenCount;
  operatorStack.push(std::make_shared<std::string>(token->getTokenVal()));
}

void AParser::HandleRightParenthesisToken(
    std::stack<std::shared_ptr<std::string>>& operatorStack,
    std::queue<std::shared_ptr<std::string>>& postFixQueue, int& parenCount) {
  if (parenCount <= 0) {
    throw UnmatchedParenthesesException();
  }

  while (operatorStack.top()->compare(AParserConstant::LEFT_PARENTHESIS) != 0) {
    postFixQueue.push(operatorStack.top());
    operatorStack.pop();
  }

  --parenCount;
  operatorStack.pop();
}

std::queue<std::shared_ptr<std::string>> AParser::ConvertInfixToPostfix(
    std::vector<std::shared_ptr<Token>> infixTokens) {
  std::queue<std::shared_ptr<std::string>> postFixQueue;
  std::stack<std::shared_ptr<std::string>> operatorStack;

  int parenCount = 0;

  for (auto const& token : infixTokens) {
    if (IsWordOrIntegerToken(token)) {
      HandleInfixWordOrIntegerToken(token, postFixQueue);
    } else if (IsMathematicalOperator(token->getTokenVal())) {
      HandleInfixOperatorToken(token, operatorStack, postFixQueue);
    } else if (IsTokenValue(token, AParserConstant::LEFT_PARENTHESIS)) {
      HandleLeftParenthesisToken(token, operatorStack, parenCount);
    } else if (IsTokenValue(token, AParserConstant::RIGHT_PARENTHESIS)) {
      HandleRightParenthesisToken(operatorStack, postFixQueue, parenCount);
    } else {
      throw InvalidExprSpecException();
    }
  }

  if (parenCount != 0) {
    throw UnmatchedParenthesesException();
  }

  while (!operatorStack.empty()) {
    postFixQueue.push(operatorStack.top());
    operatorStack.pop();
  }

  return postFixQueue;
}

void AParser::ValidateTreeStackSize(
    std::stack<std::shared_ptr<TreeNode>>& treeStack, int size) {
  if (treeStack.size() < size) {
    throw InvalidExprException();
  }
}

std::shared_ptr<TreeNode> AParser::BuildExprTreeAndValidate(
    std::queue<std::shared_ptr<std::string>>& postFixQueue) {
  std::stack<std::shared_ptr<TreeNode>> treeStack;

  while (!postFixQueue.empty()) {
    std::shared_ptr<std::string> element = postFixQueue.front();
    postFixQueue.pop();

    if (IsMathematicalOperator(element->c_str())) {
      ValidateTreeStackSize(treeStack, AParserConstant::MINIMUM_OPERATOR_SIZE);

      std::shared_ptr<TreeNode> rightSubTree = treeStack.top();
      treeStack.pop();
      std::shared_ptr<TreeNode> leftSubTree = treeStack.top();
      treeStack.pop();

      treeStack.push(
          std::make_shared<TreeNode>(*element, leftSubTree, rightSubTree));
    } else {
      treeStack.push(std::make_shared<TreeNode>(*element, nullptr, nullptr));
    }
  }

  ValidateTreeStackSize(treeStack, 1);

  return treeStack.top();
}
