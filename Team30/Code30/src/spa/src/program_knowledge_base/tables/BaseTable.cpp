#include "BaseTable.h"

#include <stdio.h>

#include <any>
#include <iostream>

bool BaseTable::insertData(std::any key, std::any value) { return true; }

std::any BaseTable::queryData(std::any key) { return -1; }
