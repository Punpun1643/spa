#include <functional>
#include <string>
#include <unordered_set>
#include <utility>

struct PairHash2 {
  template <class T1, class T2>
  std::size_t operator()(std::pair<T1, T2> const& p) const {
    auto h1 = std::hash<T1>{}(p.first);
    auto h2 = HashUnorderedSet(p.second);

    // Combine the hash values using a simple bitwise operation
    return h1 ^ h2;
  }

 private:
  template <class U>
  std::size_t HashUnorderedSet(U const& unorderedSet) const {
    std::size_t hash = 0;
    for (auto const& element : unorderedSet) {
      // Combine the hash values of elements in the unordered set
      hash ^= std::hash<typename U::value_type>{}(element);
    }
    return hash;
  }
};
