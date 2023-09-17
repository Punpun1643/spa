#pragma once

#include "../../shared/parser/AParser.h"
#include "../../shared/tokenizer/token/Token.h"
#include "../common/EntityType.h"

class QpParser : public AParser {
 public:
  QpParser(std::vector<std::shared_ptr<Token>> tokens);

 protected:
  bool IsSynonym(std::string name);
  bool IsRelRef(std::string name);
  bool IsTransitiveRelRef(std::string name);
  bool IsStmtRef(std::string name);
  EntityType StringToEntityType(std::string entity_string);
};
