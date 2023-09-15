#pragma once

#include <string>

#include "../common/EntityType.h"
#include "../interpreter/QueryInterpreter.h"
#include "AExpression.h"

class DeclarationExpression : public AExpression {
 public:
  DeclarationExpression(EntityType entity_type, std::string synonym);
  void acceptInterpreter(QueryInterpreter& interpreter) override;
  EntityType getEntityType();
  std::string getSynonym();

 private:
  EntityType entity_type;
  std::string synonym;
};
