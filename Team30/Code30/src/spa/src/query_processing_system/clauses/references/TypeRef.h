#pragma once
#include "StmtRef.h"
#include "EntityRef.h"
#include "../PqlDeclaration.h"

class TypeRef: StmtRef, EntityRef {
private:
    PqlDeclaration stmt_type;

public:
    bool isMatching(Node node);
};
