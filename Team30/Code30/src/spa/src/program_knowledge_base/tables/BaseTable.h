#pragma once

#include <stdio.h>

#include <any>
#include <iostream>

class BaseTable {
  virtual bool insertData(std::any key, std::any value);
  virtual std::any queryData(std::any key);
};