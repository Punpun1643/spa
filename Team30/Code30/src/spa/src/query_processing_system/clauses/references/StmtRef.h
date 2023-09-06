#pragma once
#include "EntityRef.h"

class StmtNode: Node {}; // TEMP definition until parser team makes theirs

class StmtRef {
public:
    virtual bool isMatching(StmtNode stmt_node) = 0;
};
