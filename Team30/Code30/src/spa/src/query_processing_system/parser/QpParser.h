#pragma once

/* #include <queue> */

/* #include "../common/SelectClause.h" */
/* #include "../common/SuchThatClause.h" */
/* #include "../expression/AExpression.h" */

#include "../../shared/parser/AParser.h"
#include "../../shared/tokenizer/token/Token.h"
#include "../common/EntityType.h"

class QpParser : public AParser {
 public:
  QpParser(std::vector<std::shared_ptr<Token>> tokens);

 protected:
  bool IsTokenType(std::shared_ptr<Token> token, TokenType token_type);
  bool IsWordToken(std::shared_ptr<Token> token);
  bool IsIntegerToken(std::shared_ptr<Token> token);
  bool IsSpecialCharToken(std::shared_ptr<Token> token);
  bool IsEOFToken(std::shared_ptr<Token> token);
  bool IsSynonym(std::string name);
  bool IsRelRef(std::string name);
  bool IsTransitiveRelRef(std::string name);
  bool IsStmtRef(std::string name);
  EntityType StringToEntityType(std::string entity_string);
};
