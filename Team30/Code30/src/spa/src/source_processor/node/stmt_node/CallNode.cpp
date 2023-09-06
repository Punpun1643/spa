#include "CallNode.h"

CallNode::CallNode(int stmtIndex, StmtType stmtType, std::string procName) : StmtNode(stmtIndex, StmtType::CALL_STMT),
                                                                             procName(procName) {}