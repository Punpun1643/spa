#pragma once
#include "EntityType.h"
#include <string>

class PqlDeclaration {
private:
    std::string name;
    EntityType entity_type;

public:
    EntityType getType();
};
