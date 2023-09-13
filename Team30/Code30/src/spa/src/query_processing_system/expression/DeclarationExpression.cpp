#include "DeclarationExpression.h"
#include "../interpreter/QueryInterpreter.h"


void DeclarationExpression::acceptInterpreter(std::unique_ptr<QueryInterpreter> interpreter) {
  interpreter->interpret(std::make_unique<DeclarationExpression>(*this));
}
