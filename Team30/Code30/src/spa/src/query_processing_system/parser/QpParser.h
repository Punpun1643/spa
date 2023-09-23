#pragma once

#include "../../shared/parser/AParser.h"
#include "../../shared/tokenizer/token/Token.h"
#include "../common/EntityType.h"

class QpParser : public AParser {
 public:
  QpParser(std::vector<std::shared_ptr<Token>> tokens);

  virtual ~QpParser() = default;

 protected:
  bool IsEntRef(std::string const& name);
  bool IsIdentifier(std::string const& name);
  bool IsStmtRef(std::string const& name);
  bool IsSynonym(std::string const& name);
  bool IsTransitiveRelRef(std::string const& name);
  bool IsRelRef(std::string const& name);
  bool IsWildcard(std::string const& name);
  EntityType StringToEntityType(std::string const& entity_string);
};
