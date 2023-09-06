#include "ReadNode.h"

ReadNode::ReadNode(int stmtIndex, StmtType stmtType, std::string varName) : StmtNode(stmtIndex, stmtType),
                                                                            varName(varName) {};