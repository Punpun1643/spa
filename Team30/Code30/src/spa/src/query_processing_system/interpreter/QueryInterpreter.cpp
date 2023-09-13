#include "QueryInterpreter.h"

#include "../expression/DeclarationExpression.h"


void QueryInterpreter::interpret(std::unique_ptr<DeclarationExpression> declarationExpression) {
  std::string designEntity = declarationExpression->getDesignEntity();
  std::string synonym = declarationExpression->getSynonym();
  EntityType entityType;

  if (designEntity == "stmt") {
    entityType = EntityType::STMT;
  }
  std::shared_ptr<PqlDeclaration> declaration = std::make_shared<PqlDeclaration>(std::make_shared<std::string>(synonym), entityType);
  this->declarations->insert(make_pair(synonym, declaration));
}

std::unique_ptr<DECLARATION_MAP_TYPE> QueryInterpreter::getDeclarations() {
  return std::move(this->declarations);
}
