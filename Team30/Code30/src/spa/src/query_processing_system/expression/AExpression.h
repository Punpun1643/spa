#pragma once

#include "../interpreter/QueryInterpreter.h"


class QueryInterpreter;

class AExpression {
  public:
    virtual void acceptInterpreter(std::unique_ptr<QueryInterpreter> interpreter) = 0;
};
