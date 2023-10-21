#pragma once

#include <string>

class SPA {
 public:
  void process();
  ~SPA() = default;

 private:
  std::string filePath;
};
