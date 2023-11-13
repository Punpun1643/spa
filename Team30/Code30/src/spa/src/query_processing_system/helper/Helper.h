#pragma once

#include <memory>
#include <string>

#include "shared/types/AttrType.h"
#include "shared/types/EntityType.h"

class Helper {
 public:
  // Constants
  static std::string const AND;
  static std::string const AFFECTS;
  static std::string const BOOLEAN;
  static std::string const CALLS;
  static std::string const CALLS_STAR;
  static std::string const FOLLOWS;
  static std::string const FOLLOWS_STAR;
  static std::string const MODIFIES;
  static std::string const NEXT;
  static std::string const NEXT_STAR;
  static std::string const PATTERN;
  static std::string const PARENT;
  static std::string const PARENT_STAR;
  static std::string const SELECT;
  static std::string const SUCH;
  static std::string const USES;
  static std::string const WITH;
  static std::string const STMT_NUM;
  static std::string const PROC_NAME;
  static std::string const VAR_NAME;
  static std::string const VALUE;
  static std::string const STMT;
  static std::string const READ;
  static std::string const PRINT;
  static std::string const CALL;
  static std::string const WHILE;
  static std::string const IF;
  static std::string const ASSIGN;
  static std::string const VARIABLE;
  static std::string const CONSTANT;
  static std::string const PROCEDURE;
  static std::string const SELECT_ATTR_TYPE_PREFIX;
  static std::string const SELECT_MULTIPLE_OPENING_BRACE;
  static std::string const SELECT_MULTIPLE_CLOSING_BRACE;
  static std::string const SELECT_MULTIPLE_DELIMITER;
  static std::string const NOT;
  static std::string const CLAUSE_ARG_DELIMITER;
  static std::string const CLAUSE_OPENING_BRACE;
  static std::string const CLAUSE_CLOSING_BRACE;
  static std::string const WILD;
  static std::string const QUOTE;

  // Methods
  static bool IsEntRef(std::string const& name);
  static bool IsQuotedIdentifier(std::string const& name);
  static bool IsStmtRef(std::string const& name);
  static bool IsIdentifier(std::string const& name);
  static bool IsRelRef(std::string const& name);
  static bool IsSynonym(std::string const& name);
  static bool IsTransitiveRelRef(std::string const& name);
  static bool IsValidInteger(std::string const& int_string);
  static bool IsWildcard(std::string const& name);
  static AttrType GetDefaultAttrTypeFromEntityType(EntityType entity_type);
  static AttrType StringToAttrType(std::string const& string);
  static EntityType StringToEntityType(std::string const& entity_string);
};
