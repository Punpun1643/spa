#include <any>
#include <iostream>
#include <stdio.h>

#include "BaseTable.h"

bool BaseTable::insertData(std::any key, std::any value) {
   return true;
}

std::any BaseTable::queryData(std::any key) {
   return -1;
}
