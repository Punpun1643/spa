#include "QpParser.h"
#include "../../shared/tokenizer/token/SpecialCharToken.h"
#include "../../shared/tokenizer/token/Token.h"
#include "../../shared/tokenizer/token/TokenType.h"
#include "../common/SelectClause.h"
#include <map>

bool isWordToken(std::shared_ptr<Token> token) {
  if (token->getTokenType() != TokenType::WORD_TOKEN) {
    return false;
  }
  return true;
}

bool isSpecialCharToken(std::shared_ptr<Token> token) {
  if (token->getTokenType() != TokenType::SPECIAL_CHAR_TOKEN) {
    return false;
  }
  return true;
}

std::shared_ptr<ParsedQuery> parse() {
  map<std::string, std::shared_ptr<PqlDeclaration>> declarations;

  // get declarations
  while (getCurrToken()->getTokenVal() != "select") {
    std::shared_ptr<Token> currToken = getCurrToken();
    std::string tokenValue = currToken->getTokenVal();
    if (isWordToken(currToken) && tokenValue == "stmt") {
      nextToken();
      std::shared_ptr<Token> nameToken = getCurrToken();
      std::shared_ptr<PqlDeclaration> declaration = new PqlDeclaration(
          new std::string(nameToken->getTokenVal()), EntityType::STMT);
      declarations.insert(make_pair(nameToken->getTokenValue(), declaration));
      nextToken();
    }
    std::shared_ptr<Token> semicolonToken = getCurrToken();
    std::string tokenValue = semicolonToken->getTokenVal();
    if (isSpecialCharToken(currToken) && tokenValue) {
      nextToken();
      continue;
    } else {
      throw std::invalid_argument("Invalid declaration");
    }
  }

  ParsedQuery parsedQuery;
  // select
  while (getCurrToken()->getTokenType() != TokenType::EOF_TOKEN) {
    std::shared_ptr<Token> currToken = getCurrToken();
    EntityType selectionEntityType;
    nextToken();
    std::shared_ptr<Token> entityNameToken = getCurrToken();
    std::shared_ptr<PqlDeclaration> declaration =
        map[entityNametoken->getTokenVal()];
    std::shared_ptr<SelectClause> selectClause =
        new SelectClause(declaration) parsedQuery.selectClause = selectClause;
    nextToken();
    // such that clause stuff follows here. implement follows first. abstract
    // each clause algo into a function?
  }

  return parsedQuery;
}
