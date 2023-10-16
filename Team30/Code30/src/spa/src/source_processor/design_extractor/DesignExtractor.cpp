#include "DesignExtractor.h"

#include <stdio.h>

#include <iostream>
#include <string>
#include <vector>

#include "../../program_knowledge_base/PKBSPInterface.h"

DesignExtractor::DesignExtractor(PKBSPInterface& pkb) : pkb(pkb) {}

void DesignExtractor::ExtractFromProgram(std::shared_ptr<ProgramNode> node) {}

void DesignExtractor::ExtractFromProcedure(
    std::shared_ptr<ProcedureNode> node) {}

void DesignExtractor::ExtractFromStmtLst(std::shared_ptr<StmtLstNode> node) {}

void DesignExtractor::ExtractFromCall(std::shared_ptr<CallNode> node) {}

void DesignExtractor::ExtractFromPrint(std::shared_ptr<PrintNode> node) {}

void DesignExtractor::ExtractFromRead(std::shared_ptr<ReadNode> node) {}

void DesignExtractor::ExtractFromWhile(std::shared_ptr<WhileNode> node) {}

void DesignExtractor::ExtractFromIf(std::shared_ptr<IfNode> node) {}

void DesignExtractor::ExtractFromAssign(std::shared_ptr<AssignNode> node) {}
