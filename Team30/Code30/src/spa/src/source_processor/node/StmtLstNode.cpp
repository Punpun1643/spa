#include "StmtLstNode.h"
#include "../design_extractor/IDesignExtractor.h"

StmtLstNode::StmtLstNode(std::vector <std::shared_ptr<StmtNode>> stmts) : stmts(stmts) {}

void StmtLstNode::accept(IDesignExtractor &designExtractor) {
    auto thisStmtLstNodePtr = std::dynamic_pointer_cast<StmtLstNode>(shared_from_this());
    designExtractor.extractFromStmtLst(thisStmtLstNodePtr);
}