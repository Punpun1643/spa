#include "UFDS.h"

UFDS::UFDS() {
  n = 0;
  rank = std::vector<int>();
  sizes = std::vector<int>();
  parent = std::vector<int>();
}

void UFDS::expandToSize(int i) {
  if (i <= n) {
    return;
  }

  for (int curr_size = n; curr_size < i; curr_size++) {
    rank.push_back(0);
    sizes.push_back(1);
    parent.push_back(curr_size);
  }

  n = i;
}

int UFDS::findSet(int i) {
  expandToSize(i + 1);
  return (parent[i] == i) ? i : parent[i] = findSet(parent[i]);
}

bool UFDS::isSameSet(int i, int j) {
  return findSet(i) == findSet(j);
}

void UFDS::unionSets(int i, int j) {
  if (!isSameSet(i, j)) {
    int x = findSet(i), y = findSet(j);
    if (rank[x] > rank[y]) {
      sizes[findSet(x)] += sizes[findSet(y)];
      parent[y] = x;
    } else {
      sizes[findSet(y)] += sizes[findSet(x)];
      parent[x] = y;
      if (rank[x] == rank[y])
        rank[y]++;
    }
  }
}

int UFDS::sizeOf(int i) {
  return sizes[findSet(i)];
}
