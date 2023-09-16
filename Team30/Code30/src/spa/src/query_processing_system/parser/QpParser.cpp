#include "QpParser.h"

#include <iostream>
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

