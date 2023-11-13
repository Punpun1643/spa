#include "Helper.h"

#include "../../shared/tokenizer/token/SpecialCharToken.h"
#include "../exceptions/InvalidSyntaxException.h"
#include "../references/AttrRef.h"
#include "../references/PqlDeclaration.h"
#include "../references/StmtRef.h"

// Constants
std::string const Helper::AND = "and";
std::string const Helper::AFFECTS = "Affects";
std::string const Helper::BOOLEAN = "BOOLEAN";
std::string const Helper::CALLS = "Calls";
std::string const Helper::CALLS_STAR = "Calls*";
std::string const Helper::FOLLOWS = "Follows";
std::string const Helper::FOLLOWS_STAR = "Follows*";
std::string const Helper::MODIFIES = "Modifies";
std::string const Helper::NEXT = "Next";
std::string const Helper::NEXT_STAR = "Next*";
std::string const Helper::PATTERN = "pattern";
std::string const Helper::PARENT = "Parent";
std::string const Helper::PARENT_STAR = "Parent*";
std::string const Helper::SELECT = "Select";
std::string const Helper::SUCH = "such";
std::string const Helper::USES = "Uses";
std::string const Helper::WITH = "with";
std::string const Helper::STMT_NUM = "stmt#";
std::string const Helper::PROC_NAME = "procName";
std::string const Helper::VAR_NAME = "varName";
std::string const Helper::VALUE = "value";
std::string const Helper::STMT = "stmt";
std::string const Helper::READ = "read";
std::string const Helper::PRINT = "print";
std::string const Helper::CALL = "call";
std::string const Helper::WHILE = "while";
std::string const Helper::IF = "if";
std::string const Helper::ASSIGN = "assign";
std::string const Helper::VARIABLE = "variable";
std::string const Helper::CONSTANT = "constant";
std::string const Helper::PROCEDURE = "procedure";
std::string const Helper::SELECT_ATTR_TYPE_PREFIX = ".";
std::string const Helper::SELECT_MULTIPLE_OPENING_BRACE = "<";
std::string const Helper::SELECT_MULTIPLE_CLOSING_BRACE = ">";
std::string const Helper::SELECT_MULTIPLE_DELIMITER = ",";
std::string const Helper::NOT = "not";
std::string const Helper::CLAUSE_ARG_DELIMITER = ",";
std::string const Helper::CLAUSE_OPENING_BRACE = "(";
std::string const Helper::CLAUSE_CLOSING_BRACE = ")";
std::string const Helper::WILD = "_";
std::string const Helper::QUOTE = "\"";

// Methods
bool Helper::IsEntRef(std::string const& name) {
  return (IsSynonym(name) || IsWildcard(name) || IsQuotedIdentifier(name));
}

bool Helper::IsQuotedIdentifier(std::string const& name) {
  if (name.size() >= 3) {
    return (name.substr(0, 1) == "\"" &&
            IsSynonym(name.substr(1, name.size() - 2)) &&
            name.substr(name.size() - 1, 1) == "\"");
  }
  return false;
}

bool Helper::IsStmtRef(std::string const& name) {
  if ((name != WILD) && (!IsValidInteger(name)) && (!IsSynonym(name))) {
    return false;
  }
  return true;
}

bool Helper::IsIdentifier(std::string const& name) {
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

bool Helper::IsRelRef(std::string const& name) {
  std::string arr[] = {FOLLOWS, FOLLOWS_STAR, PARENT_STAR, PARENT,
                       USES,    MODIFIES,     CALLS,       CALLS_STAR,
                       NEXT,    NEXT_STAR,    AFFECTS};
  int arr_size = sizeof(arr) / sizeof(*arr);
  if (std::find(arr, arr + arr_size, name) == arr + arr_size) {
    return false;
  }
  return true;
}

bool Helper::IsSynonym(std::string const& name) {
  return IsIdentifier(name);
}

bool Helper::IsTransitiveRelRef(std::string const& name) {
  std::string arr[] = {FOLLOWS, PARENT};
  int arr_size = sizeof(arr) / sizeof(*arr);
  if (std::find(arr, arr + arr_size, name) == arr + arr_size) {
    return false;
  }
  return true;
}

bool Helper::IsValidInteger(std::string const& int_string) {
  try {
    stoi(int_string);
    if (stoi(int_string) < 0) {
      return false;
    }
  } catch (std::invalid_argument& e) {
    return false;
  }
  if (int_string.front() == '0' && int_string.size() > 1) {
    return false;
  }
  return true;
}

bool Helper::IsWildcard(std::string const& name) {
  return (name == WILD);
}

AttrType Helper::GetDefaultAttrTypeFromEntityType(EntityType entity_type) {
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

AttrType Helper::StringToAttrType(std::string const& string) {
  if (string == PROC_NAME) {
    return AttrType::PROC_NAME;
  } else if (string == VAR_NAME) {
    return AttrType::VAR_NAME;
  } else if (string == VALUE) {
    return AttrType::VALUE;
  } else if (string == STMT_NUM) {
    return AttrType::STMT_NUM;
  } else {
    throw std::invalid_argument("String to be converted to attrType not valid");
  }
}

EntityType Helper::StringToEntityType(std::string const& entity_string) {
  if (entity_string == STMT) {
    return EntityType::STMT;
  } else if (entity_string == READ) {
    return EntityType::READ;
  } else if (entity_string == PRINT) {
    return EntityType::PRINT;
  } else if (entity_string == CALL) {
    return EntityType::CALL;
  } else if (entity_string == WHILE) {
    return EntityType::WHILE;
  } else if (entity_string == IF) {
    return EntityType::IF;
  } else if (entity_string == ASSIGN) {
    return EntityType::ASSIGN;
  } else if (entity_string == VARIABLE) {
    return EntityType::VARIABLE;
  } else if (entity_string == CONSTANT) {
    return EntityType::CONSTANT;
  } else if (entity_string == PROCEDURE) {
    return EntityType::PROCEDURE;
  } else {
    throw InvalidSyntaxException("Invalid design entity for declaration");
  }
}
