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

bool QpParser::IsSynonym(std::string name) {
  if (!std::isalpha(name[0])) {
    return false;
  }
  for (size_t i = 1; i < name.length(); i++) {
    if (!std::isalnum(name[i])) {
      return false;
    }
  }
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
  } else if (entity_string == "read") {
    return EntityType::READ;
  } else if (entity_string == "print") {
    return EntityType::PRINT;
  } else if (entity_string == "call") {
    return EntityType::CALL;
  } else if (entity_string == "while") {
    return EntityType::WHILE;
  } else if (entity_string == "if") {
    return EntityType::IF;
  } else if (entity_string == "assign") {
    return EntityType::ASSIGN;
  } else if (entity_string == "variable") {
    return EntityType::VARIABLE;
  } else if (entity_string == "constant") {
    return EntityType::CONSTANT;
  } else if (entity_string == "procedure") {
    return EntityType::PROCEDURE;
  } else {
    throw std::runtime_error("Invalid design entity for declaration");
  }
}
