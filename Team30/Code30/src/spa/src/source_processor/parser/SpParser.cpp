#include <string>

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
}


std::shared_ptr <PrintNode> SpParser::parsePrint() {
    // check if valid print
    std::shared_ptr<Token> currToken = getCurrToken();

    if (currToken->getTokenType() == TokenType::WORD_TOKEN) {
        std::string varName = currToken->getTokenVal();
        // increment index token to get next token
        nextToken();
        // check if valid print
        if (getCurrToken()->getTokenVal() == ";") {
            // increment index token to get next token
            nextToken();
            return std::make_shared<PrintNode>(1, StmtType::PRINT_STMT, varName);
        } else {
            throw std::invalid_argument("Invalid print 1");
        }
    } else {
        throw std::invalid_argument("Invalid print 2");
    }
}

std::shared_ptr <ReadNode> SpParser::parseRead() {
    std::shared_ptr<Token> currToken = getCurrToken();

    if (currToken->getTokenType() == TokenType::WORD_TOKEN) {
        std::string varName = currToken->getTokenVal();
        nextToken();
        if (getCurrToken()->getTokenVal() == ";") {
            nextToken();
            return std::make_shared<ReadNode>(1, StmtType::READ_STMT, varName);
        } else {
            throw std::invalid_argument("Invalid read 1");
        }
    } else {
        throw std::invalid_argument("Invalid read 2");
    }
}

std::shared_ptr <StmtLstNode> SpParser::parseStmtLst() {
    std::vector <std::shared_ptr<StmtNode>> stmts;

    while (getCurrToken()->getTokenVal() != END_PROCEDURE) {
        //current token
        std::shared_ptr<Token> currToken = getCurrToken();
        if (currToken->getTokenType() == TokenType::WORD_TOKEN && currToken->getTokenVal() == "print") {
            nextToken();
            // parse print
            stmts.push_back(parsePrint());
        } else if (currToken->getTokenType() == TokenType::WORD_TOKEN && currToken->getTokenVal() == "read") {
            nextToken();
            // parse read
            stmts.push_back(parseRead());
        } else {
            throw std::invalid_argument("Invalid StmtLst");
        }
    }

    return std::make_shared<StmtLstNode>(stmts);
}


void SpParser::parse() {
    // dummy
}
