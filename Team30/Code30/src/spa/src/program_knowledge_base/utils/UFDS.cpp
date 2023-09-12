#include "UFDS.h"

UFDS::UFDS(int n) {
  numSets = n;
  rank = std::vector<int>(n, 0);
  sizes = std::vector<int>(n, 1);
  p = std::vector<int>();

  for (int i = 0; i < n; i++) {
    p.push_back(i);
  };
}

int UFDS::findSet(int i) { return (p[i] == i) ? i : p[i] = findSet(p[i]); }

bool UFDS::isSameSet(int i, int j) { return findSet(i) == findSet(j); }

void UFDS::unionSets(int i, int j) {
  if (!isSameSet(i, j)) {
    int x = findSet(i), y = findSet(j);
    if (rank[x] > rank[y]) {
      sizes[findSet(x)] += sizes[findSet(y)];
      p[y] = x;
    } else {
      sizes[findSet(y)] += sizes[findSet(x)];
      p[x] = y;
      if (rank[x] == rank[y]) rank[y]++;
    }
    numSets--;
  }
}

int UFDS::sizeOf(int i) { return sizes[findSet(i)]; }

int UFDS::numDisjointSets() { return numSets; }