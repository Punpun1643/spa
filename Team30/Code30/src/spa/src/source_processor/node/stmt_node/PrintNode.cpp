#include "PrintNode.h"

PrintNode::PrintNode(int stmtIndex, StmtType stmtType, std::string varName) : StmtNode(stmtIndex, StmtType::PRINT_STMT),
                                                                              varName(varName) {}