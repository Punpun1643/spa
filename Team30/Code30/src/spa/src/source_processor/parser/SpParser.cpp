#include <string>
#include <iostream>

#include "SpParser.h"

const std::string START_PROCEDURE = "{";
const std::string END_PROCEDURE = "}";

SpParser::SpParser(std::vector <std::shared_ptr<Token>> tokens) : AParser(tokens) {}

std::shared_ptr <ProgramNode> SpParser::parseProgram() {
    std::vector <std::shared_ptr<ProcedureNode>> procedures;

    while (getCurrToken()->getTokenType() != TokenType::EOF_TOKEN) {
        //current token
        std::shared_ptr<Token> currToken = getCurrToken();
        if (currToken->getTokenType() == TokenType::WORD_TOKEN && currToken->getTokenVal() == "procedure") {
            // increment index token to get next token
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
    // check if valid procedureName
    std::shared_ptr<Token> currToken = getCurrToken();

    if (currToken->getTokenType() == TokenType::WORD_TOKEN) {
        std::string procedureName = currToken->getTokenVal();
        // increment index token to get next token
        nextToken();
        // check if valid procedure
        // check if it has {
        if (getCurrToken()->getTokenVal() == START_PROCEDURE) {
            // increment index token to get next token
            nextToken();
            // parse stmtLst
            std::shared_ptr<StmtLstNode> stmtLst = parseStmtLst();
            // check if valid procedure
            if (getCurrToken()->getTokenVal() == END_PROCEDURE) {
                // increment index token to get next token
                nextToken();
                return std::make_shared<ProcedureNode>(procedureName, stmtLst);
            } else {
                throw std::invalid_argument("Invalid procedure 1");
            }
        } else {
            throw std::invalid_argument("Invalid procedure 2");
        }
    } else {
        throw std::invalid_argument("Invalid procedure 4");
    }

    // dummy return
    // return std::make_shared<ProcedureNode>(getCurrToken()->getTokenVal(), nullptr);
}

std::shared_ptr <StmtLstNode> SpParser::parseStmtLst() {
    nextToken();
    std::vector<std::shared_ptr<StmtNode>> empty_stmts;
    auto  stmtLstNode = std::make_shared<StmtLstNode>(empty_stmts);
    return stmtLstNode;
}

void SpParser::parse() {
    // dummy
}