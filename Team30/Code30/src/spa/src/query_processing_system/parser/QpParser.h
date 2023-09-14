#pragma once

#include <queue>

#include "../common/SelectClause.h"
#include "../common/SuchThatClause.h"
#include "../expression/AExpression.h"
#include "../../shared/parser/AParser.h"
#include "../../shared/tokenizer/token/Token.h"

class QpParser : public AParser {
  public:
    QpParser(std::vector <std::shared_ptr<Token>> tokens);

    /* std::unique_ptr<AExpression> getQueryExpression(); */

    std::queue<std::shared_ptr<Clause>> parseQuery();
    void parse() override;
  /* private: */
  /*   std::unique_ptr<AExpression> queryExpression; */
};
