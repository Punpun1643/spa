#pragma once

#include "../common/SelectClause.h"
#include "../common/SuchThatClause.h"
#include "../../shared/parser/AParser.h"
#include "../../shared/tokenizer/token/Token.h"

struct ParsedQuery {
  SelectClause selectClause;
  SuchThatClause* suchThatClause;
  ParsedQuery() : selectClause(nullptr), suchThatClause(nullptr) {}
};

class QpParser : public AParser {
  public:
    QpParser(std::vector <std::shared_ptr<Token>> tokens);

    std::shared_ptr <ParsedQuery> parseQuery();
    void parse() override;
};
