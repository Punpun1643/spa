#pragma once

#include <vector>

class UFDS {
 private:
  int n;
  std::vector<int> parent, rank, sizes;
  void expandToSize(int i);

 public:
  UFDS();
  ~UFDS() = default;

  int findSet(int i);
  bool isSameSet(int i, int j);
  void unionSets(int i, int j);
  int sizeOf(int i);
};