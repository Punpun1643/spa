#include <vector>

class UFDS {
 private:
  std::vector<int> p, rank, sizes;
  int numSets;

 public:
  UFDS(int n);
  ~UFDS() = default;

  int findSet(int i);
  bool isSameSet(int i, int j);
  void unionSets(int i, int j);
  int sizeOf(int i);
  int numDisjointSets();
};