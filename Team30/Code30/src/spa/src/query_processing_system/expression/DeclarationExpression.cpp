#include "DeclarationExpression.h"

DeclarationExpression::DeclarationExpression(EntityType entity_type,
                                             std::string synonym)
    : entity_type(entity_type), synonym(synonym){};

void DeclarationExpression::acceptInterpreter(QueryInterpreter& interpreter) {
  interpreter.InterpretDeclarations(*this);
}

EntityType DeclarationExpression::getEntityType() { return this->entity_type; }

std::string DeclarationExpression::getSynonym() { return this->synonym; }
