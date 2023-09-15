#include "QpParser.h"

#include <map>
#include <stdexcept>

#include "../../shared/tokenizer/token/SpecialCharToken.h"
#include "../common/FollowsClause.h"
#include "../common/PqlDeclaration.h"
#include "../common/StmtRef.h"

QpParser::QpParser(std::vector<std::shared_ptr<Token>> tokens)
    : AParser(tokens) {}

bool QpParser::IsTokenType(std::shared_ptr<Token> token, TokenType token_type) {
  if (token->getTokenType() != token_type) {
    return false;
  }
  return true;
}

bool QpParser::IsWordToken(std::shared_ptr<Token> token) {
  return QpParser::IsTokenType(token, TokenType::WORD_TOKEN);
}

bool QpParser::IsIntegerToken(std::shared_ptr<Token> token) {
  return QpParser::IsTokenType(token, TokenType::INTEGER_TOKEN);
}

bool QpParser::IsSpecialCharToken(std::shared_ptr<Token> token) {
  return QpParser::IsTokenType(token, TokenType::SPECIAL_CHAR_TOKEN);
}

bool QpParser::IsEOFToken(std::shared_ptr<Token> token) {
  return QpParser::IsTokenType(token, TokenType::EOF_TOKEN);
}

bool QpParser::IsSynonym(std::string name) {
  // dummy for now
  return true;
}

bool QpParser::IsRelRef(std::string name) {
  std::string arr[] = {"Follows", "Parent", "Uses", "Modifies"};
  int arr_size = sizeof(arr) / sizeof(*arr);
  if (std::find(arr, arr + arr_size, name) == arr + arr_size) {
    return false;
  }
  return true;
}

bool QpParser::IsTransitiveRelRef(std::string name) {
  std::string arr[] = {"Follows", "Parent"};
  int arr_size = sizeof(arr) / sizeof(*arr);
  if (std::find(arr, arr + arr_size, name) == arr + arr_size) {
    return false;
  }
  return true;
}

bool QpParser::IsStmtRef(std::string name) {
  bool is_integer;
  try {
    stoi(name);
    is_integer = true;
  } catch (std::invalid_argument ex) {
    is_integer = false;
  }
  if ((name != "_") && (!is_integer) && (!IsSynonym(name))) {
    return false;
  }
  return true;
}

EntityType QpParser::StringToEntityType(std::string entity_string) {
  if (entity_string == "stmt") {
    return EntityType::STMT;
  } else {
    throw std::runtime_error("Invalid design entity for declaration");
  }
}

/* std::unique_ptr<AExpression> QpParser::GetQueryExprerrion() { */
/*   return this->queryExpression; */
/* } */

/* void QpParser::parse() { */
/*   std::unique_ptr<AExpression> queryExpression =
 * std::make_unique<AExpression>(); */

/*   while (getCurrToken()->getTokenType() != TokenType::EOF_TOKEN) { */
/*   } */
/* } */

/* void QpParser::parse(){}; */

/* QpParser::QpParser(std::vector<std::shared_ptr<Token>> tokens) */
/*     : AParser(tokens) {} */

/* std::queue<std::shared_ptr<Clause>> QpParser::parseQuery() { */
/*   std::map<std::string, std::shared_ptr<PqlDeclaration>> declarations; */

/*   // get declarations */
/*   while (getCurrToken()->getTokenVal() != "Select") { */
/*     std::shared_ptr<Token> currToken = getCurrToken(); */
/*     std::string currTokenValue = currToken->getTokenVal(); */
/*     if (isWordToken(currToken) && currTokenValue == "stmt") { */
/*       nextToken(); */
/*       std::shared_ptr<Token> nameToken = getCurrToken(); */
/*       std::string name = nameToken->getTokenVal(); */
/*       std::shared_ptr<PqlDeclaration> declaration = */
/*           std::make_shared<PqlDeclaration>(std::make_shared<std::string>(name),
 */
/*                                            EntityType::STMT); */

/*       declarations.insert(make_pair(nameToken->getTokenVal(), declaration));
 */
/*       nextToken(); */
/*     } */
/*     std::shared_ptr<Token> semicolonToken = getCurrToken(); */
/*     std::string semicolonTokenValue = semicolonToken->getTokenVal(); */
/*     if (isSpecialCharToken(semicolonToken) && semicolonTokenValue == ";") {
 */
/*       nextToken(); */
/*       continue; */
/*     } else { */
/*       throw std::invalid_argument("Invalid declaration"); */
/*     } */
/*   } */

/*   std::queue<std::shared_ptr<Clause>> clauseQueue; */
/*   // select */
/*   while (getCurrToken()->getTokenType() != TokenType::EOF_TOKEN) { */
/*     std::shared_ptr<Token> currToken = getCurrToken(); */
/*     EntityType selectionEntityType; */
/*     nextToken(); */
/*     std::shared_ptr<Token> entityNameToken = getCurrToken(); */
/*     std::shared_ptr<PqlDeclaration> declaration = */
/*         declarations[entityNameToken->getTokenVal()]; */

/*     clauseQueue.push(std::make_shared<SelectClause>(declaration)); */
/*     nextToken(); */
/*     if (getCurrToken()->getTokenVal() == "such" && */
/*         peekToken()->getTokenVal() == "that") { */
/*       nextToken();  // ( */
/*       nextToken();  // clause */
/*       if (getCurrToken()->getTokenVal() == "Follows") { */
/*         nextToken();  // ( */
/*         if (getCurrToken()->getTokenVal() != "(") { */
/*           throw std::invalid_argument("Invalid Follows syntax"); */
/*         } */

/*         nextToken();  // entity 1 */
/*         std::unique_ptr<StmtRef> stmtRef1; */
/*         if (isIntegerToken(getCurrToken())) { */
/*           stmtRef1 = */
/*               std::make_unique<StmtRef>(stoi(getCurrToken()->getTokenVal()));
 */
/*         } else if (isSpecialCharToken(getCurrToken())) { */
/*           if (getCurrToken()->getTokenVal() != "_") { */
/*             throw std::invalid_argument("Invalid Follows syntax"); */
/*           } */
/*           stmtRef1 = std::make_unique<StmtRef>(); */
/*         } else { */
/*           const std::string tokenValue1 = getCurrToken()->getTokenVal(); */
/*           std::shared_ptr<PqlDeclaration> declaration1 = */
/*               declarations[tokenValue1]; */
/*           if (!declaration1) { */
/*             throw std::invalid_argument(tokenValue1 + " was not declared");
 */
/*           } */
/*           stmtRef1 = std::make_unique<StmtRef>(declaration1); */
/*         } */

/*         nextToken();  // , */
/*         if (getCurrToken()->getTokenVal() != ",") { */
/*           throw std::invalid_argument("Invalid Follows syntax"); */
/*         } */

/*         nextToken();  // entity 2 */
/*         std::unique_ptr<StmtRef> stmtRef2; */
/*         if (isIntegerToken(getCurrToken())) { */
/*           stmtRef2 = */
/*               std::make_unique<StmtRef>(stoi(getCurrToken()->getTokenVal()));
 */
/*         } else if (isSpecialCharToken(getCurrToken())) { */
/*           if (getCurrToken()->getTokenVal() != "_") { */
/*             throw std::invalid_argument("Invalid Follows syntax"); */
/*           } */
/*           stmtRef2 = std::make_unique<StmtRef>(); */
/*         } else { */
/*           const std::string tokenValue2 = getCurrToken()->getTokenVal(); */
/*           std::shared_ptr<PqlDeclaration> declaration2 = */
/*               declarations[tokenValue2]; */
/*           if (!declaration2) { */
/*             throw std::invalid_argument(tokenValue2 + " was not declared");
 */
/*           } */
/*           stmtRef2 = std::make_unique<StmtRef>(declaration2); */
/*         } */

/*         nextToken();  // ) */
/*         if (getCurrToken()->getTokenVal() != ")") { */
/*           throw std::invalid_argument("Invalid Follows syntax"); */
/*         } */
/*         clauseQueue.push(std::make_shared<FollowsClause>(std::move(stmtRef1),
 */
/*                                                          std::move(stmtRef2)));
 */
/*         nextToken(); */
/*       } */
/*     } */
/*     // such that clause stuff follows here. implement follows first. abstract
 */
/*     // each clause algo into a function? */
/*   } */

/*   return clauseQueue; */
/* } */
