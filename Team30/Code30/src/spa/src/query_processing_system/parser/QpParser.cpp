#include "QpParser.h"

#include <iostream>
#include <map>
#include <stdexcept>

#include "../../shared/tokenizer/token/SpecialCharToken.h"
#include "../common/FollowsClause.h"
#include "../common/PqlDeclaration.h"
#include "../common/StmtRef.h"

QpParser::QpParser(std::vector<std::shared_ptr<Token>> tokens)
    : AParser(tokens){};

bool QpParser::IsEntRef(std::string const& name) {
  return (IsSynonym(name) || IsWildcard(name) || IsIdentifier(name));
}

bool QpParser::IsIdentifier(std::string const& name) {
  /* std::cout << "qpp1: " << ((name.substr(0, 1) == "\"") ? "11" : "10") <<
   * "\n"; */
  /* std::cout << "qpp2: " << ((IsSynonym(name.substr(1, name.size() - 2))) ?
   * "21" : "20") << "\n"; */
  /* std::cout << "qpp3: " << ((name.substr(name.size()-1, 1) == "\"") ? "21" :
   * "20") << "\n"; */
  /* std::cout << "qpp4: " << (name.substr(name.size()-1, 1)) << "\n"; */
  if (name.size() >= 3) {
    return (name.substr(0, 1) == "\"" &&
            IsSynonym(name.substr(1, name.size() - 2)) &&
            name.substr(name.size() - 1, 1) == "\"");
  }
  return false;
}

bool QpParser::IsStmtRef(std::string const& name) {
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

bool QpParser::IsSynonym(std::string const& name) {
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

bool QpParser::IsTransitiveRelRef(std::string const& name) {
  std::string arr[] = {"Follows", "Parent"};
  int arr_size = sizeof(arr) / sizeof(*arr);
  if (std::find(arr, arr + arr_size, name) == arr + arr_size) {
    return false;
  }
  return true;
}

bool QpParser::IsRelRef(std::string const& name) {
  std::string arr[] = {"Follows", "Parent", "Uses", "Modifies"};
  int arr_size = sizeof(arr) / sizeof(*arr);
  if (std::find(arr, arr + arr_size, name) == arr + arr_size) {
    return false;
  }
  return true;
}

bool QpParser::IsWildcard(std::string const& name) { return (name == "_"); }

EntityType QpParser::StringToEntityType(std::string const& entity_string) {
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
