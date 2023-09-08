#include "ReadNode.h"
#include "../../design_extractor/IDesignExtractor.h"

ReadNode::ReadNode(int stmtIndex, StmtType stmtType, std::string varName) : StmtNode(stmtIndex, StmtType::READ_STMT),
                                                                            varName(varName) {};

void ReadNode::accept(IDesignExtractor &designExtractor) {
    auto thisReadNodePtr = std::dynamic_pointer_cast<ReadNode>(shared_from_this());
    designExtractor.extractFromRead(thisReadNodePtr);
}