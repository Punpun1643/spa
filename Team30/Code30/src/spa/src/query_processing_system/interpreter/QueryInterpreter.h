#pragma once

#include <map>

#include "../common/PqlDeclaration.h"


typedef std::map<std::string, std::shared_ptr<PqlDeclaration>> DECLARATION_MAP_TYPE;

class DeclarationExpression;

class QueryInterpreter {
  public:
    void interpret(std::unique_ptr<DeclarationExpression> declarationExpression);
    std::unique_ptr<DECLARATION_MAP_TYPE> getDeclarations();
  private:
    std::unique_ptr<DECLARATION_MAP_TYPE> declarations;
};
