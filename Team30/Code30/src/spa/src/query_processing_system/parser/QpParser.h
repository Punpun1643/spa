#pragma once

#include "../../shared/parser/AParser.h"
#include "../../shared/tokenizer/token/Token.h"
#include "../common/EntityType.h"

class QpParser : public AParser {
 public:
  QpParser(std::vector<std::shared_ptr<Token>> tokens);


  // Constants
  static std::string const AND;
  static std::string const BOOLEAN;
  static std::string const CALLS;
  static std::string const CALLS_STAR;
  static std::string const FOLLOWS;
  static std::string const FOLLOWS_STAR;
  static std::string const MODIFIES;
  static std::string const PATTERN;
  static std::string const PARENT;
  static std::string const PARENT_STAR;
  static std::string const SELECT;
  static std::string const SUCH;
  static std::string const USES;

  virtual ~QpParser() = default;

 protected:
  bool IsEntRef(std::string const& name);
  bool IsQuotedIdentifier(std::string const& name);
  bool IsStmtRef(std::string const& name);
  bool IsIdentifier(std::string const& name);
  bool IsSynonym(std::string const& name);
  bool IsTransitiveRelRef(std::string const& name);
  bool IsRelRef(std::string const& name);
  bool IsWildcard(std::string const& name);
  EntityType StringToEntityType(std::string const& entity_string);
};
