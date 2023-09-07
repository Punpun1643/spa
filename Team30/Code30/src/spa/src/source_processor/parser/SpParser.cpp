#include "SpParser.h"

SpParser::SpParser(std::vector <std::shared_ptr<Token>> tokens) : AParser(tokens) {}

std::shared_ptr <ProgramNode> SpParser::parseProgram() {
    std::vector <std::shared_ptr<ProcedureNode>> procedures;

    while (peekToken()->getTokenType() != TokenType::EOF_TOKEN) {
        //current token
        std::shared_ptr<Token> currToken = getCurrToken();
        if (currToken->getTokenType() == TokenType::WORD_TOKEN && currToken->getTokenVal() == "procedure") {
            // incrementIndex token to get next token
            nextToken();
            // parse procedure
            procedures.push_back(parseProcedure());
        } else {
            throw std::invalid_argument("Invalid procedure");
        }
    }

    return std::make_shared<ProgramNode>(procedures);
}

std::shared_ptr <ProcedureNode> SpParser::parseProcedure() {
    // dummy return
    return std::make_shared<ProcedureNode>(getCurrToken()->getTokenVal(), nullptr);
}

void SpParser::parse() {
    // dummy
}