#include "QpParser.h"

#include <iostream>
#include <map>
#include <stdexcept>

#include "../../shared/tokenizer/token/SpecialCharToken.h"
#include "../common/AttrRef.h"
#include "../common/FollowsClause.h"
#include "../common/PqlDeclaration.h"
#include "../common/StmtRef.h"
#include "../exceptions/InvalidSyntaxException.h"

QpParser::QpParser(std::vector<std::shared_ptr<Token>> tokens)
    : AParser(tokens){};

// Constants
std::string const QpParser::AND = "and";
std::string const QpParser::BOOLEAN = "BOOLEAN";
std::string const QpParser::CALLS = "Calls";
std::string const QpParser::CALLS_STAR = "Calls*";
std::string const QpParser::FOLLOWS = "Follows";
std::string const QpParser::FOLLOWS_STAR = "Follows*";
std::string const QpParser::MODIFIES = "Modifies";
std::string const QpParser::NEXT = "Next";
std::string const QpParser::NEXT_STAR = "Next*";
std::string const QpParser::PATTERN = "pattern";
std::string const QpParser::PARENT = "Parent";
std::string const QpParser::PARENT_STAR = "Parent*";
std::string const QpParser::SELECT = "Select";
std::string const QpParser::SUCH = "such";
std::string const QpParser::USES = "Uses";
std::string const QpParser::WITH = "with";

bool QpParser::IsEntRef(std::string const& name) {
  return (IsSynonym(name) || IsWildcard(name) || IsQuotedIdentifier(name));
}

bool QpParser::IsInteger(std::string const& name) {
  try {
    stoi(name);
  } catch (std::invalid_argument& e) {
    return false;
  }
  return true;
}

bool QpParser::IsQuotedIdentifier(std::string const& name) {
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

bool QpParser::IsIdentifier(std::string const& name) {
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

bool QpParser::IsSynonym(std::string const& name) {
  return this->IsIdentifier(name);
}

bool QpParser::IsTransitiveRelRef(std::string const& name) {
  std::string arr[] = {FOLLOWS, PARENT};
  int arr_size = sizeof(arr) / sizeof(*arr);
  if (std::find(arr, arr + arr_size, name) == arr + arr_size) {
    return false;
  }
  return true;
}

bool QpParser::IsRelRef(std::string const& name) {
  std::string arr[] = {FOLLOWS,  FOLLOWS_STAR, PARENT_STAR, PARENT, USES,
                       MODIFIES, CALLS,        CALLS_STAR,  NEXT,   NEXT_STAR};
  int arr_size = sizeof(arr) / sizeof(*arr);
  if (std::find(arr, arr + arr_size, name) == arr + arr_size) {
    return false;
  }
  return true;
}

bool QpParser::IsWildcard(std::string const& name) { return (name == "_"); }

AttrType QpParser::GetDefaultAttrTypeFromEntityType(EntityType entity_type) {
  std::unordered_map<EntityType, AttrType> DEFAULT_ATTR_TYPES = {
      {EntityType::PROCEDURE, AttrType::PROC_NAME},
      {EntityType::VARIABLE, AttrType::VAR_NAME},
      {EntityType::CONSTANT, AttrType::VALUE},
      {EntityType::STMT, AttrType::STMT_NUM},
      {EntityType::ASSIGN, AttrType::STMT_NUM},
      {EntityType::IF, AttrType::STMT_NUM},
      {EntityType::WHILE, AttrType::STMT_NUM},
      {EntityType::PRINT, AttrType::STMT_NUM},
      {EntityType::READ, AttrType::STMT_NUM},
      {EntityType::CALL, AttrType::STMT_NUM}};
  return DEFAULT_ATTR_TYPES.at(entity_type);
}

AttrType QpParser::StringToAttrType(std::string const& string) {
  if (string == "procName") {
    return AttrType::PROC_NAME;
  } else if (string == "varName") {
    return AttrType::VAR_NAME;
  } else if (string == "value") {
    return AttrType::VALUE;
  } else if (string == "stmt#") {
    return AttrType::STMT_NUM;
  } else {
    throw std::invalid_argument("String to be converted to attrType not valid");
  }
}

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
    throw InvalidSyntaxException("Invalid design entity for declaration");
  }
}
