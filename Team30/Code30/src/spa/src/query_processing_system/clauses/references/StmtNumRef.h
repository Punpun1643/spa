#pragma once
#include "StmtRef.h"

class StmtNumRef: StmtRef {
private:
    int stmt_num;

public:
    bool isMatching(StmtNode stmt_node);
};
