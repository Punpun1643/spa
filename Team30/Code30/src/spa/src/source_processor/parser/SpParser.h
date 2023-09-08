#pragma once

#include <string>
#include "../../shared/tokenizer/token/Token.h"
#include "../../shared/parser/AParser.h"
#include "../node/ProgramNode.h"
#include "../node/ProcedureNode.h"
#include "../node/StmtLstNode.h"
#include "../node/stmt_node/ReadNode.h"
#include "../node/stmt_node/PrintNode.h"
#include "../node/stmt_node/CallNode.h"

class SpParser : public AParser {
public:
    SpParser(std::vector <std::shared_ptr<Token>> tokens);

    std::shared_ptr <ProgramNode> parseProgram();

    std::shared_ptr <ProcedureNode> parseProcedure();

    std::shared_ptr <StmtLstNode> parseStmtLst();

    std::shared_ptr <PrintNode> parsePrint();

    std::shared_ptr <ReadNode> parseRead();
//

//
//    std::shared_ptr <CallNode> parseCall();

    void parse() override;

    ~SpParser() = default;


};