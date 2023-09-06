#pragma once
#include "StmtRef.h"
#include "EntityRef.h"

class WildRef: EntityRef, StmtRef {
public:
    bool isMatching(Node node);
};
