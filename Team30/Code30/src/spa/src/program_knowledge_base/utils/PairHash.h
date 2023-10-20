struct PairHash {
  template <class T1, class T2>
  std::size_t operator()(std::pair<T1, T2> const& p) const {
    auto h1 = std::hash<T1>{}(p.first);
    auto h2 = std::hash<T2>{}(p.second);

    // Combine the hash values using a simple bitwise operation
    return h1 ^ h2;
  }
};
