#pragma once

#include "../../shared/parser/AParser.h"
#include "../../shared/tokenizer/token/Token.h"
#include "../common/AttrType.h"
#include "../common/EntityType.h"

namespace attr_name {
std::string const STMT_NUM = "stmt#";
std::string const PROC_NAME = "procName";
std::string const VAR_NAME = "varName";
std::string const VALUE = "value";
}  // namespace attr_name

class QpParser : public AParser {
 public:
  QpParser(std::vector<std::shared_ptr<Token>> tokens);

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

  virtual ~QpParser() = default;

 protected:
  bool IsEntRef(std::string const& name);
  bool IsQuotedIdentifier(std::string const& name);
  bool IsStmtRef(std::string const& name);
  bool IsIdentifier(std::string const& name);
  bool IsRelRef(std::string const& name);
  bool IsSynonym(std::string const& name);
  bool IsTransitiveRelRef(std::string const& name);
  bool IsValidInteger(std::string const& int_string);
  bool IsWildcard(std::string const& name);
  AttrType GetDefaultAttrTypeFromEntityType(EntityType entity_type);
  AttrType StringToAttrType(std::string const& string);
  EntityType StringToEntityType(std::string const& entity_string);
};
