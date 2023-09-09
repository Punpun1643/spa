#include "PrintNode.h"
#include "../../design_extractor/IDesignExtractor.h"

PrintNode::PrintNode(int stmtIndex, StmtType stmtType, std::string varName) : StmtNode(stmtIndex, StmtType::PRINT_STMT),
                                                                              varName(varName) {}

void PrintNode::accept(IDesignExtractor &designExtractor) {
    auto thisPrintNodePtr = std::dynamic_pointer_cast<PrintNode>(shared_from_this());
    designExtractor.extractFromPrint(thisPrintNodePtr);
}