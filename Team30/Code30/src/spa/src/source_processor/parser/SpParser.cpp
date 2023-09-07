#include "SpParser.h"

SpParser::SpParser(std::vector <std::shared_ptr<Token>> tokens) : AParser(tokens) {}

std::shared_ptr <ProgramNode> SpParser::parseProgram() {
    std::vector <std::shared_ptr<ProcedureNode>> procedures;

//    while (getCurrToken->getType() == TokenType::PROC) {
//        procedures.push_back(parseProcedure());
//    }

    return std::make_shared<ProgramNode>(procedures);
}