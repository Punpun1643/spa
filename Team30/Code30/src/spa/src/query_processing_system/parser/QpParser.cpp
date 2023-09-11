#include "QpParser.h"

#include <map>

#include "../../shared/tokenizer/token/SpecialCharToken.h"
#include "../common/PqlDeclaration.h"

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

void QpParser::parse(){};

QpParser::QpParser(std::vector<std::shared_ptr<Token>> tokens)
    : AParser(tokens) {}

std::shared_ptr<ParsedQuery> QpParser::parseQuery() {
  std::map<std::string, std::shared_ptr<PqlDeclaration>> declarations;

  // get declarations
  while (getCurrToken()->getTokenVal() != "select") {
    std::shared_ptr<Token> currToken = getCurrToken();
    std::string currTokenValue = currToken->getTokenVal();
    if (isWordToken(currToken) && currTokenValue == "stmt") {
      nextToken();
      std::shared_ptr<Token> nameToken = getCurrToken();
      std::string name = nameToken->getTokenVal();
      std::shared_ptr<PqlDeclaration> declaration =
          std::make_shared<PqlDeclaration>(std::make_shared<std::string>(name),
                                           EntityType::STMT);

      /* std::shared_ptr<PqlDeclaration> declaration = PqlDeclaration( */
      /*    std::shared_ptr<std::string>(nameToken->getTokenVal()),
       * EntityType::STMT); */
      declarations.insert(make_pair(nameToken->getTokenVal(), declaration));
      nextToken();
    }
    std::shared_ptr<Token> semicolonToken = getCurrToken();
    std::string semicolonTokenValue = semicolonToken->getTokenVal();
    if (isSpecialCharToken(semicolonToken) && semicolonTokenValue == ";") {
      nextToken();
      continue;
    } else {
      printf("cond a is %d\n", isSpecialCharToken(currToken));
      throw std::invalid_argument("Invalid declaration");
    }
  }

  /* std::shared_ptr<ParsedQuery> parsedQuery = {nullptr, nullptr}; */
  std::shared_ptr<ParsedQuery> parsedQuery = std::make_shared<ParsedQuery>();
  // select
  while (getCurrToken()->getTokenType() != TokenType::EOF_TOKEN) {
    std::shared_ptr<Token> currToken = getCurrToken();
    EntityType selectionEntityType;
    nextToken();
    std::shared_ptr<Token> entityNameToken = getCurrToken();
    std::shared_ptr<PqlDeclaration> declaration =
        declarations[entityNameToken->getTokenVal()];
    /* struct ParsedQuery parsedQuery = { */
    /*     SelectClause(declaration) */
    /* }; */
    parsedQuery->selectClause = SelectClause(declaration);
    nextToken();
    // such that clause stuff follows here. implement follows first. abstract
    // each clause algo into a function?
  }

  return parsedQuery;
}
