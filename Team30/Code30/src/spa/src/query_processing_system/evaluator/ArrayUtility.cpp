#include "ArrayUtility.h"

#include <algorithm>
#include <unordered_set>

void ArrayUtility::removeDuplicates(std::vector<std::vector<std::string>>& vec) {
  /**
   * Removes duplicates from the given vector in-place.
   */
  std::sort(vec.begin(), vec.end());
  vec.erase(std::unique(vec.begin(), vec.end()), vec.end());
}
