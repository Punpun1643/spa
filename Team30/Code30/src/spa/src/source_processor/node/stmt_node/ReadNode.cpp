#include "ReadNode.h"

ReadNode::ReadNode(int stmtIndex, StmtType stmtType, std::string varName) : StmtNode(stmtIndex, StmtType::READ_STMT),
                                                                            varName(varName) {};