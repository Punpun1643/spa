#include "CallNode.h"
#include "../../design_extractor/IDesignExtractor.h"

CallNode::CallNode(int stmtIndex, StmtType stmtType, std::string procName) : StmtNode(stmtIndex, StmtType::CALL_STMT),
                                                                             procName(procName) {}

void CallNode::accept(IDesignExtractor &designExtractor) {
    auto thisCallNodePtr = std::dynamic_pointer_cast<CallNode>(shared_from_this());
    designExtractor.extractFromCall(thisCallNodePtr);
}