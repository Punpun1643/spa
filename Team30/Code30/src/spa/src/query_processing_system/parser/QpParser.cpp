#include "QpParser.h"

#include <map>

#include "../../shared/tokenizer/token/SpecialCharToken.h"
#include "../common/FollowsClause.h"
#include "../common/PqlDeclaration.h"
#include "../common/StmtRef.h"

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

std::queue<std::shared_ptr<Clause>> QpParser::parseQuery() {
  std::map<std::string, std::shared_ptr<PqlDeclaration>> declarations;

  // get declarations
  while (getCurrToken()->getTokenVal() != "Select") {
    std::shared_ptr<Token> currToken = getCurrToken();
    std::string currTokenValue = currToken->getTokenVal();
    if (isWordToken(currToken) && currTokenValue == "stmt") {
      nextToken();
      std::shared_ptr<Token> nameToken = getCurrToken();
      std::string name = nameToken->getTokenVal();
      std::shared_ptr<PqlDeclaration> declaration =
          std::make_shared<PqlDeclaration>(std::make_shared<std::string>(name),
                                           EntityType::STMT);

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

  /* std::shared_ptr<ParsedQuery> parsedQuery = std::make_shared<ParsedQuery>();
   */
  std::queue<std::shared_ptr<Clause>> clauseQueue;
  // select
  while (getCurrToken()->getTokenType() != TokenType::EOF_TOKEN) {
    std::shared_ptr<Token> currToken = getCurrToken();
    EntityType selectionEntityType;
    nextToken();
    std::shared_ptr<Token> entityNameToken = getCurrToken();
    std::shared_ptr<PqlDeclaration> declaration =
        declarations[entityNameToken->getTokenVal()];

    clauseQueue.push(std::make_shared<SelectClause>(declaration));
    nextToken();
    if (getCurrToken()->getTokenVal() == "such" &&
        peekToken()->getTokenVal() == "that") {
      nextToken();  // (
      nextToken();  // clause
      if (getCurrToken()->getTokenVal() == "Follows") {
        nextToken();  // (
        if (getCurrToken()->getTokenVal() != "(") {
          throw std::invalid_argument("Invalid Follows syntax");
        }
        printf("chkpt1!!!\n");
        nextToken();  // entity 1
        // TODO: logic to determine entity type?
        std::shared_ptr<PqlDeclaration> entity1 =
            std::make_shared<PqlDeclaration>(
                std::make_shared<std::string>(getCurrToken()->getTokenVal()),
                EntityType::STMT);
        std::unique_ptr<StmtRef> stmtRef1 = std::make_unique<StmtRef>(entity1);
        printf("chkpt2!!!\n");
        nextToken();  // ,
        printf("chkpt3!!!\n");
        if (getCurrToken()->getTokenVal() != ",") {
          throw std::invalid_argument("Invalid Follows syntax");
        }
        nextToken();  // entity 2
        // TODO: logic to determine entity type
        std::shared_ptr<PqlDeclaration> entity2 =
            std::make_shared<PqlDeclaration>(
                std::make_shared<std::string>(getCurrToken()->getTokenVal()),
                EntityType::STMT);
        std::unique_ptr<StmtRef> stmtRef2 = std::make_unique<StmtRef>(entity2);
        nextToken();  // )
        if (getCurrToken()->getTokenVal() != ")") {
          throw std::invalid_argument("Invalid Follows syntax");
        }
        clauseQueue.push(std::make_shared<FollowsClause>(std::move(stmtRef1),
                                                         std::move(stmtRef2)));
        /* clauseQueue.push(std::make_shared<Clause>(FollowsClause(std::move(stmtRef1),
         * std::move(stmtRef2)))); */
        nextToken();
      }
    }
    // such that clause stuff follows here. implement follows first. abstract
    // each clause algo into a function?
  }

  return clauseQueue;
}
