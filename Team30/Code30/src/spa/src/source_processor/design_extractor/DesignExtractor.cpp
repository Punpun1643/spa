#include "DesignExtractor.h"

#include <program_knowledge_base/PKBSPInterface.h>
#include <stdio.h>

#include <iostream>
#include <string>
#include <vector>

DesignExtractor::DesignExtractor(PKBSPInterface& pkb) : pkb(pkb) {}

void DesignExtractor::extractFromProgram(std::shared_ptr<ProgramNode> node) {}

void DesignExtractor::extractFromProcedure(
    std::shared_ptr<ProcedureNode> node) {}

void DesignExtractor::extractFromStmtLst(std::shared_ptr<StmtLstNode> node) {}

void DesignExtractor::extractFromCall(std::shared_ptr<CallNode> node) {}

void DesignExtractor::extractFromPrint(std::shared_ptr<PrintNode> node) {}

void DesignExtractor::extractFromRead(std::shared_ptr<ReadNode> node) {}

void DesignExtractor::extractFromWhile(std::shared_ptr<WhileNode> node) {}

void DesignExtractor::extractFromIf(std::shared_ptr<IfNode> node) {}

void DesignExtractor::extractFromAssign(std::shared_ptr<AssignNode> node) {}
