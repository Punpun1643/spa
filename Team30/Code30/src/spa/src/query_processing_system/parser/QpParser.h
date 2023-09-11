#pragma once

#include <queue>

#include "../common/SelectClause.h"
#include "../common/SuchThatClause.h"
#include "../../shared/parser/AParser.h"
#include "../../shared/tokenizer/token/Token.h"

class QpParser : public AParser {
  public:
    QpParser(std::vector <std::shared_ptr<Token>> tokens);

    std::queue<std::shared_ptr<Clause>> parseQuery();
    void parse() override;
};
